"""
heater_control.py

Predictive ON/OFF heater control for relay-based sample and coil heaters.

This file is designed as a drop-in replacement for the existing heater module.
It keeps the same public API:
    - HEATER_ON
    - HEATER_OFF
    - HeaterController
    - HeaterController.update(current_temp, required_temp)

No changes are required in integrate.py.

Key improvements:
1. One controller implementation works for both SAMPLE_HEATER and COIL_HEATER.
2. Filtered rate estimation with clamping and deadband.
3. Predictive early cut-off to reduce overshoot.
4. Hysteresis-based re-enable.
5. Minimum hold time to reduce rapid ON/OFF chatter.
6. Per-heater default tuning while still allowing constructor overrides.
"""

from collections import deque
from typing import Deque
import time


HEATER_ON = True
HEATER_OFF = False


class HeaterController:
    """
    Predictive ON/OFF heater controller for relay-based heating.

    This controller is suitable for systems where:
    - Heater is controlled only through relay ON/OFF
    - Temperature continues to rise after heater turns OFF
    - Small sensor noise should not cause relay chatter

    The controller supports both SAMPLE_HEATER and COIL_HEATER with
    different default tuning values, while preserving constructor
    compatibility with the existing code.
    """

    def __init__(
        self,
        name: str = "Heater",
        history_length: int = 3,
        k_predict_sec: float = 100.0,
        hysteresis: float = 0.5,
        min_early_margin: float = 0.2,
        max_early_cut: float = 5.0,
    ) -> None:
        """
        Initialize heater controller.

        Args:
            name:
                Heater/controller name. Used for logging and default tuning.
            history_length:
                Number of recent temperature samples to store.
            k_predict_sec:
                Predictive constant controlling early cut-off.
            hysteresis:
                Temperature drop below target required before heater turns ON again.
            min_early_margin:
                Minimum margin before target to force heater OFF.
            max_early_cut:
                Maximum allowed early cut-off below target.
        """
        self.name = name.strip().upper()
        self.temp_history: Deque[float] = deque(maxlen=max(3, int(history_length)))
        self.heater_state: bool = HEATER_OFF

        # Filter state
        self.filtered_rate = 0.0

        # Minimum relay state hold time support
        self.last_state_change_time = time.monotonic()

        # Start with constructor values, then apply per-heater defaults
        self.k_predict_sec = float(k_predict_sec)
        self.hysteresis = float(hysteresis)
        self.min_early_margin = float(min_early_margin)
        self.max_early_cut = float(max_early_cut)

        # Additional internal tuning defaults
        self.rate_alpha = 0.25
        self.rate_deadband = 0.004
        self.startup_margin = 1.0
        self.min_hold_sec = 3.0

        self._apply_name_based_defaults()

    def _apply_name_based_defaults(self) -> None:
        """
        Apply default tuning based on heater name.

        Existing constructor values from integrate.py are preserved when they
        are intentionally passed there. This helper mainly adds internal
        tuning for rate filtering and hold timing.
        """
        if self.name == "SAMPLE_HEATER":
            # Sample heater is slower and usually stores more heat.
            self.rate_alpha = 0.20
            self.rate_deadband = 0.003
            self.min_hold_sec = 8.0
            self.startup_margin = max(self.hysteresis, 0.8)
        elif self.name == "COIL_HEATER":
            # Coil heater is faster and benefits from slightly quicker response.
            self.rate_alpha = 0.30
            self.rate_deadband = 0.005
            self.min_hold_sec = 4.0
            self.startup_margin = max(self.hysteresis, 0.8)
        else:
            self.rate_alpha = 0.25
            self.rate_deadband = 0.004
            self.min_hold_sec = 5.0
            self.startup_margin = max(self.hysteresis, 1.0)

    def update(self, current_temp: float, required_temp: float) -> bool:
        """
        Update controller state and decide heater ON/OFF.

        Args:
            current_temp:
                Current measured temperature in degree Celsius.
            required_temp:
                Required target temperature in degree Celsius.

        Returns:
            True if heater should be ON, otherwise False.
        """
        current_temp = float(current_temp)
        required_temp = float(required_temp)

        self.temp_history.append(current_temp)

        if len(self.temp_history) < self.temp_history.maxlen:
            self.heater_state = self._startup_decision(
                current_temp=current_temp,
                required_temp=required_temp,
            )
            self._log_state(
                current_temp=current_temp,
                rate=0.0,
                predicted_off_temp=required_temp - self.min_early_margin,
            )
            return self.heater_state

        rate = self._calculate_rising_rate()

        predicted_off_temp = self._calculate_predicted_off_temp(
            required_temp=required_temp,
            rate=rate,
        )

        now_time = time.monotonic()

        # Hard stop at target
        if current_temp >= required_temp:
            self._set_heater_state(HEATER_OFF, now_time)
            self._log_state(current_temp=current_temp, rate=rate, predicted_off_temp=predicted_off_temp)
            return self.heater_state

        # OFF decision while heating
        if self.heater_state == HEATER_ON:
            if current_temp >= predicted_off_temp:
                if self._can_change_state(now_time):
                    self._set_heater_state(HEATER_OFF, now_time)

            self._log_state(current_temp=current_temp, rate=rate, predicted_off_temp=predicted_off_temp)
            return self.heater_state

        # ON decision while cooling
        turn_on_temp = required_temp - self.hysteresis
        if current_temp <= turn_on_temp:
            if self._can_change_state(now_time):
                self._set_heater_state(HEATER_ON, now_time)

        self._log_state(current_temp=current_temp, rate=rate, predicted_off_temp=predicted_off_temp)
        return self.heater_state

    def _startup_decision(self, current_temp: float, required_temp: float) -> bool:
        """
        Provide initial heater decision before enough samples are collected.

        Args:
            current_temp:
                Current measured temperature.
            required_temp:
                Required target temperature.

        Returns:
            True if heater should be ON, otherwise False.
        """
        if current_temp < (required_temp - self.startup_margin):
            return HEATER_ON
        return HEATER_OFF

    def _calculate_rising_rate(self) -> float:
        """
        Calculate filtered temperature rise rate.

        Returns:
            Filtered temperature rise rate in degree Celsius per sample interval.
        """
        old_temp = self.temp_history[0]
        new_temp = self.temp_history[-1]
        interval_count = len(self.temp_history) - 1

        if interval_count <= 0:
            return 0.0

        raw_rate = (new_temp - old_temp) / float(interval_count)

        # Clamp unrealistic spikes/noise bursts
        if raw_rate > 0.2:
            raw_rate = 0.2
        elif raw_rate < -0.2:
            raw_rate = -0.2

        # Low-pass filter
        self.filtered_rate = (
            self.rate_alpha * raw_rate
            + (1.0 - self.rate_alpha) * self.filtered_rate
        )

        # Deadband close to zero to reduce chatter
        if abs(self.filtered_rate) < self.rate_deadband:
            self.filtered_rate = 0.0

        return self.filtered_rate

    def _calculate_predicted_off_temp(self, required_temp: float, rate: float) -> float:
        """
        Calculate predicted heater OFF temperature.

        Args:
            required_temp:
                Target temperature.
            rate:
                Filtered temperature rise rate.

        Returns:
            Predicted OFF temperature in degree Celsius.
        """
        # If temperature is not meaningfully rising, do not cut too early.
        if rate <= 0.0:
            predicted_off_temp = required_temp - self.min_early_margin
        else:
            predicted_rise = self.k_predict_sec * rate

            if predicted_rise < self.min_early_margin:
                predicted_rise = self.min_early_margin
            elif predicted_rise > self.max_early_cut:
                predicted_rise = self.max_early_cut

            predicted_off_temp = required_temp - predicted_rise

        min_off_temp = required_temp - self.max_early_cut
        max_off_temp = required_temp - self.min_early_margin

        if predicted_off_temp < min_off_temp:
            predicted_off_temp = min_off_temp
        elif predicted_off_temp > max_off_temp:
            predicted_off_temp = max_off_temp

        return predicted_off_temp

    def _can_change_state(self, now_time: float) -> bool:
        """
        Check whether relay state is allowed to change.

        Args:
            now_time:
                Current monotonic time in seconds.

        Returns:
            True if enough hold time has elapsed, else False.
        """
        return (now_time - self.last_state_change_time) >= self.min_hold_sec

    def _set_heater_state(self, new_state: bool, now_time: float) -> None:
        """
        Update heater state and remember transition time.

        Args:
            new_state:
                Desired heater state.
            now_time:
                Current monotonic time in seconds.
        """
        if self.heater_state != new_state:
            self.heater_state = new_state
            self.last_state_change_time = now_time

    def _log_state(self, current_temp: float, rate: float, predicted_off_temp: float) -> None:
        """
        Print controller state in the same style used by the current project.

        Args:
            current_temp:
                Current measured temperature.
            rate:
                Filtered rise rate.
            predicted_off_temp:
                Predicted temperature where heater should turn OFF.
        """
        print(
            f"name={self.name}, "
            f"temp={current_temp:.2f}, "
            f"rate={rate:.3f}, "
            f"predicted_off={predicted_off_temp:.2f}, "
            f"heater={'ON' if self.heater_state else 'OFF'}"
        )
