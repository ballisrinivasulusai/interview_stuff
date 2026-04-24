"""
main.py

Main execution file for relay-based heater control.
"""

import time
import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

import integrate
from TinkerForge.tforge_con_temp import create_connection
from temp_config import CONTROL_LOOP_INTERVAL_SEC
from temp_config import REQUIRED_TEMPERATURE_C
from temp_config import TEMP_HISTORY_LENGTH
from heater_control import HeaterController
from heater_control import HEATER_ON
from heater_control import HEATER_OFF
from TinkerForge.dual_relay_test import dual_turn_on_relay, dual_turn_off_relay


def read_temperature_sensor() -> float:
    """
    Read current water temperature from the sensor.

    Replace this function with your actual sensor reading code.

    Returns:
        Current water temperature in degree Celsius.
    """
    # Example placeholder value

    Value =  integrate.get_sample_temperature()
    return Value


def heater_relay_on() -> None:
    """
    Turn ON the heater relay.

    Replace this function with your actual GPIO relay ON code.
    """
    print("Heater Relay ON")


    dual_relay = integrate.get_dual_relay()

    if dual_relay is None:
        print("Relay not available")
        return

    print(f"Dual Relay Object: {dual_relay}")
    dual_turn_on_relay(dual_relay, 0)

    


def heater_relay_off() -> None:
    """
    Turn OFF the heater relay.

    Replace this function with your actual GPIO relay OFF code.
    """
    print("Heater Relay OFF")
    dual_relay = integrate.get_dual_relay()

    if dual_relay is None:
        print("❌ Relay not available")
        return

    print(f"Dual Relay Object: {dual_relay}")
    dual_turn_off_relay(dual_relay, 0)


def main() -> None:
    """
    Main control loop.
    """
    controller = HeaterController(
        history_length=TEMP_HISTORY_LENGTH,
        k_predict_sec=100.0,
        hysteresis=0.5,
        min_early_margin=0.2,
        max_early_cut=5.0,
    )

    ip_con,missing_id = create_connection()


    while True:
        current_temp = read_temperature_sensor()

        print(f"Current Temperature: {current_temp:.2f} C")

        heater_state = controller.update(
            current_temp=current_temp,
            required_temp=REQUIRED_TEMPERATURE_C,
        )

        if heater_state == HEATER_ON:
            heater_relay_on()
        else:
            heater_relay_off()

        print(
            f"Current Temp: {current_temp:.2f} C | "
            f"Target Temp: {REQUIRED_TEMPERATURE_C:.2f} C | "
            f"Heater: {'ON' if heater_state else 'OFF'}"
        )

        time.sleep(CONTROL_LOOP_INTERVAL_SEC)


if __name__ == "__main__":
    main()

