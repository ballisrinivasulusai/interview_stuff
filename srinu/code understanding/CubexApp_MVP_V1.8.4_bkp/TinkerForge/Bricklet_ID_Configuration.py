import json
from pathlib import Path
from PyQt5.QtCore import QObject
from SharedState import SharedState
import os
import sys


class Bricklet_ID_Config(QObject):

    def __init__(self):
        super().__init__()

        # ---------------- Shared State ----------------
        self.shared_state = SharedState()

        # Get full MVP version string
        self.mvp_version_full = self.shared_state.get_MVP_Device_Version()
        print("Full MVP Version String:", self.mvp_version_full)

        # Extract only version number (e.g., "1.0", "1.5")
        try:
            self.mvp_version = self.mvp_version_full.split("_")[-1]
        except Exception:
            print("Error extracting MVP version, defaulting to 1.0")
            self.mvp_version = "1.0"

        print("MVP Version only:", self.mvp_version)

        # ---------------- Config File Selection ----------------
        base_path = Path(get_app_path())  
        config_path = base_path / f"Config{self.mvp_version}.json"

        print(f"Looking for config file at: {config_path}")

        # Fallback if config file does not exist
        if not config_path.exists():
            print(f"Config file not found for MVP {self.mvp_version}, loading default config1.0.json")
            config_path = base_path / "Config1.0.json"

        self.config_file = config_path
        print("Loading configuration from:", self.config_file)

        # ---------------- Load JSON ----------------
        try:
            with open(self.config_file, "r", encoding="utf-8") as f:
                self.config = json.load(f)
        except FileNotFoundError:
            print("ERROR: Configuration file not found:", self.config_file)
            self.config = {}
        except json.JSONDecodeError as e:
            print("ERROR: Invalid JSON format:", e)
            self.config = {}
        except Exception as e:  
            print("ERROR: Unexpected error while loading config:", e)
            self.config = {}

        print("Configuration loaded successfully")


def get_app_path():
    base_path = (
        os.path.dirname(sys.executable)
        if getattr(sys, 'frozen', False)
        else os.path.dirname(os.path.abspath(__file__))
    )

    if getattr(sys, 'frozen', False):
        while not base_path.endswith("CubexApp"):
            base_path = os.path.dirname(base_path)

    return base_path   # ✅ return str, convert to Path where needed


# ---------------- GLOBAL CONFIG LOAD ----------------

brick_config = Bricklet_ID_Config()

AirQuality_UID = brick_config.config.get("airQ", {}).get("UID")
AirQuality_Callback_Period = brick_config.config.get("airQ", {}).get("Callback_Period")

Dual_Gen_Relay_UID = brick_config.config.get("dual_gen", {}).get("UID")
DUAL_GEN_NUM_RELAYS = brick_config.config.get("dual_gen", {}).get("Number_of_relay")

Dual_Relay_UID = brick_config.config.get("dual_relay", {}).get("UID")
DUAL_NUM_RELAYS = brick_config.config.get("dual_relay", {}).get("Number_of_relay")

N2_GAS_UID = brick_config.config.get("N2_Gas", {}).get("N2_Gas_UID")
N2_GAS_PWM_FREQUENCY = brick_config.config.get("N2_Gas", {}).get("N2_GAS_PWM_FREQUENCY")
N2_GAS_ACCELERATION = brick_config.config.get("N2_Gas", {}).get("N2_GAS_ACCELERATION")
N2_GAS_DECELERATION = brick_config.config.get("N2_Gas", {}).get("N2_GAS_DECELERATION")
N2_GAS_VELOCITY = brick_config.config.get("N2_Gas", {}).get("N2_GAS_VELOCITY")
N2_GAS_FREQ_STEP = brick_config.config.get("N2_Gas", {}).get("N2_GAS_FREQ_STEP")

# MFC UIDs
MFC1_UID = brick_config.config.get("MFC", {}).get("MFC1", {}).get("UID")
MFC2_UID = brick_config.config.get("MFC", {}).get("MFC2", {}).get("UID")
MFC3_UID = brick_config.config.get("MFC", {}).get("MFC3", {}).get("UID")
MFC4_UID = brick_config.config.get("MFC", {}).get("MFC4", {}).get("UID")

mfc1_serial_port = f'localhost/4223/{MFC1_UID}'
mfc2_serial_port = f'localhost/4223/{MFC2_UID}'
mfc3_serial_port = f'localhost/4223/{MFC3_UID}'
mfc4_serial_port = f'localhost/4223/{MFC4_UID}'

SolenoidValve_Qrelay1_UID = brick_config.config.get("solenoid", {}).get("UID")
SV1_NUM_RELAYS = brick_config.config.get("solenoid", {}).get("Number_of_relay")

ThermoCouple1_UID = brick_config.config.get("Thermocouple", {}).get("Thermocouple1", {}).get("UID")
ThermoCouple2_UID = brick_config.config.get("Thermocouple", {}).get("Thermocouple2", {}).get("UID")

MasterBrick1_UID = brick_config.config.get("Master_Brick", {}).get("Master_Brick1", {}).get("UID")
MasterBrick2_UID = brick_config.config.get("Master_Brick", {}).get("Master_Brick2", {}).get("UID")
MasterBrick3_UID = brick_config.config.get("Master_Brick", {}).get("Master_Brick3", {}).get("UID")


# ---------------- VIAL BASE CONFIG ----------------

Vial_Base_UID = brick_config.config.get("vialbase", {}).get("Vial_Base_UID")

VIAL_STEPS_PER_TURN = brick_config.config.get("vialbase", {}).get("VIAL_STEPS_PER_TURN")
VIAL_LEAD_SCREW_PITCH = brick_config.config.get("vialbase", {}).get("VIAL_LEAD_SCREW_PITCH")
VIAL_STEP_RESOLUTION = brick_config.config.get("vialbase", {}).get("VIAL_STEP_RESOLUTION")

VIAL_STEPS_PER_MM = brick_config.config.get("vialbase", {}).get("VIAL_STEPS_PER_MM")

VIAL_UPPER_LIMIT = brick_config.config.get("vialbase", {}).get("VIAL_UPPER_LIMIT")
VIAL_LOWER_LIMIT = brick_config.config.get("vialbase", {}).get("VIAL_LOWER_LIMIT")
VIAL_1_3RD_POSITION = brick_config.config.get("vialbase", {}).get("VIAL_1_3RD_POSITION")

VIAL_MOTOR_CONFIG = brick_config.config.get("vialbase", {}).get("VIAL_MOTOR_CONFIG", {})

VIAL_MAX_VELOCITY = VIAL_MOTOR_CONFIG.get("max_velocity")
VIAL_ACCELERATION = VIAL_MOTOR_CONFIG.get("acceleration")
VIAL_DECELERATION = VIAL_MOTOR_CONFIG.get("deceleration")
VIAL_RUN_CURRENT = VIAL_MOTOR_CONFIG.get("run_current")
VIAL_STANDSTILL_CURRENT = VIAL_MOTOR_CONFIG.get("standstill_current")
VIAL_POWER_DOWN_TIME = VIAL_MOTOR_CONFIG.get("power_down_time")
VIAL_STEALTH_THRESHOLD = VIAL_MOTOR_CONFIG.get("stealth_threshold")
VIAL_COOLSTEP_THRESHOLD = VIAL_MOTOR_CONFIG.get("coolstep_threshold")
VIAL_CLASSIC_THRESHOLD = VIAL_MOTOR_CONFIG.get("classic_threshold")

if __name__ == "__main__":
    print("AirQuality_UID :", AirQuality_UID)
    print("AirQuality_Callback_Period :", AirQuality_Callback_Period)

