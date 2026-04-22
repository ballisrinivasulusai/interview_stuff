"""
config.py

General application configuration for heater temperature control.
"""


# Target temperature in degree Celsius
REQUIRED_TEMPERATURE_C = 50.0

# Main loop interval in seconds
CONTROL_LOOP_INTERVAL_SEC = 1.0

# Number of samples used for rate calculation
TEMP_HISTORY_LENGTH = 3
