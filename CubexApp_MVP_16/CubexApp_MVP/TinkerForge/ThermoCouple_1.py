"""

ThermoCouple-2 Unit test application.
These APIs can be reused for testing the ThermoCouple-2 which
can be called from the some other file.

UID : 29Hn

"""

import time
import integrate
from tinkerforge.bricklet_thermocouple_v2 import BrickletThermocoupleV2
from TinkerForge.tforge_con import create_connection
from TinkerForge.Bricklet_ID_Configuration import ThermoCouple1_UID
from tinkerforge.ip_connection import IPConnection

reinit = False
# ------------------------------
# Callback: Temperature updates
# ------------------------------
def cb_sample_temperature(temperature):
    """
    Callback when the thermocouple measures a new temperature.
    """
    temp_c = temperature / 100.0
    integrate.set_sample_temperature(temp_c)
    print(f"ThermoCouple  {ThermoCouple1_UID} Temperature: {temp_c:.2f} °C")

def tc1_initialize_thermocouple(ipcon):
    """
    Initializes the Thermocouple Bricklet.
    
    Returns:
        BrickletThermocoupleV2: The initialized thermocouple instance.
        IPConnection: The Tinkerforge IP connection instance.
    """
    thermocouple = BrickletThermocoupleV2(ThermoCouple1_UID, ipcon)

    # Register temperature callback to function cb_sample_temperature
    thermocouple.register_callback(thermocouple.CALLBACK_TEMPERATURE, cb_sample_temperature)
    
    # Set period for temperature callback to 1s (1000ms) without a threshold
    thermocouple.set_temperature_callback_configuration(1000, False, "x", 0, 0)
    
    return thermocouple

def tc1_reinitialize(uid, device_id, enum_type, ipcon):
    global  reinit
    if device_id == BrickletThermocoupleV2.DEVICE_IDENTIFIER:
        if enum_type in (IPConnection.ENUMERATION_TYPE_CONNECTED, IPConnection.ENUMERATION_TYPE_AVAILABLE):
            print(f"💡 ThermoCouple ({uid}) connected.")
            if reinit:
                print(f"🔄 Reinitializing ThermoCouple ({uid}).")
                tc1_initialize_thermocouple(ipcon)
            reinit = True

def tc1_read_ambient_temperature(thermocouple):
    """
    Reads and prints the ambient temperature.

    Args:
        thermocouple (BrickletThermocoupleV2): The thermocouple instance.
    """
    val = None
    try:
        val = thermocouple.get_temperature()

        temp = val / 100.0
    except Exception  as e:
        print(f"Error in {ThermoCouple1_UID} : {e}")
        temp = 0.0

    if val is None:
        temp = 0.0    
    
    #temp = thermocouple.get_temperature() / 100.0  # Convert to Celsius

    #print(f"🌡️ Ambient Temperature: {temp}°C")
    return temp

def tc1_start_continuous_reading(thermocouple):
    """
    Continuously reads and prints the temperature every 2 seconds until stopped.
    
    Args:
        thermocouple (BrickletThermocoupleV2): The thermocouple instance.
    """
    print("📡 Starting continuous temperature monitoring... Press Ctrl+C to stop.")
    try:
        while True:
            temp = thermocouple.get_temperature() / 100.0
            print(f"🌡️ Current Temperature: {temp}°C")
            time.sleep(2)
    except KeyboardInterrupt:
        print("\n🛑 Stopped temperature monitoring.")

def tc1_set_target_temperature(thermocouple):
    """
    Monitors temperature until it reaches the user-defined target.

    Args:
        thermocouple (BrickletThermocoupleV2): The thermocouple instance.
    """
    try:
        target_temp = float(input("🎯 Enter target temperature (°C): "))
        print(f"Monitoring until temperature reaches {target_temp}°C...")

        while True:
            current_temp = thermocouple.get_temperature() / 100.0
            print(f"🌡️ Current Temperature: {current_temp}°C")
            
            if current_temp >= target_temp:
                print(f"⚠️ Target Temperature {target_temp}°C reached! ⚠️")
                break
            time.sleep(2)
    except ValueError:
        print("❌ Invalid input! Please enter a valid number.")

def tc1_main():
    """
    Main function to manage user input for Thermocouple control.
    
    It provides options to read ambient temperature, start continuous monitoring, 
    or set a target temperature alert.
    """
    ip_con = create_connection()

    thermocouple = tc1_initialize_thermocouple(ip_con)

    while True:
        print("\n🔹 Thermocouple (29Ha) Test Application Options:")
        print("1. Read Ambient Temperature")
        print("2. Start Continuous Temperature Monitoring")
        print("3. Set Target Temperature Alert")
        print("4. Exit")

        choice = input("Enter your choice (1/2/3/4): ")

        if choice == "1":
            tc1_read_ambient_temperature(thermocouple)
        elif choice == "2":
            tc1_start_continuous_reading(thermocouple)
        elif choice == "3":
            tc1_set_target_temperature(thermocouple)
        elif choice == "4":
            print("🚪 Exiting program.")
            break
        else:
            print("❌ Invalid choice! Please enter 1, 2, 3, or 4.")

    ip_con.disconnect()
    print("✅ Disconnected from Tinkerforge.")

if __name__ == "__main__":
    tc1_main()

