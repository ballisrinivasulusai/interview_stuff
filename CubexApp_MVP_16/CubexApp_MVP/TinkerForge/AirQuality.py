"""
AirQuality Bricklet Unit Test Application.
These APIs can be reused for testing the AirQuality Bricklet which
can be called from other files.

UID : Defined in config
"""

from tinkerforge.bricklet_air_quality import BrickletAirQuality
from TinkerForge.tforge_con import create_connection
import integrate
from TinkerForge.Bricklet_ID_Configuration import AirQuality_UID
from tinkerforge.ip_connection import IPConnection

reinit = False
ipcon = None
def aq_get_iaq_index(aq):
    iaq_index = aq.get_all_values()[0] / 100.0
    print(f"\n🌫️ IAQ Index: {iaq_index:.2f}")
    return iaq_index


def aq_get_accuracy(aq):
    accuracy = aq.get_all_values()[1]
    print(f"\n🎯 Accuracy: {accuracy} (0=Low, 3=High)")
    return accuracy


def aq_get_temperature(aq):
    temperature = 0.0
    try:
        temperature = aq.get_all_values()[2] / 100.0
        print(f"\n🌡️ Temperature: {temperature:.2f} °C")
    except Exception as e:
        print(f"Error in {AirQuality_UID} : {e}")

    if temperature is None:
        temperature = 0.0
    return temperature


def aq_get_humidity(aq):
    humidity = 0.0
    try:
        humidity = aq.get_all_values()[3] / 100.0
        print(f"\n💧 Humidity: {humidity:.2f} %RH")
    except Exception as e:
        print(f"Error in {AirQuality_UID} : {e}")

    if humidity is None:
        humidity = 0.0
    return humidity

def aq_get_air_pressure(aq):
    """
    Gets and displays the air pressure from the Air Quality Bricklet.

    Args:
        aq (BrickletAirQuality): The initialized Air Quality Bricklet.

    Returns:
        float: Air Pressure in hPa.
    """
    air_pressure = 0.0
    try:
        air_pressure = aq.get_all_values()[3] / 100.0
        print(f"\n💧 Humidity: {air_pressure:.2f} %RH")
    except Exception as e:
        print(f"Error in {AirQuality_UID} : {e}")

    if air_pressure is None:
        air_pressure = 0.0
    return air_pressure   
    

# Callback function for all values callback
def cb_all_values(iaq_index, iaq_index_accuracy, temperature, humidity, air_pressure):
    print("IAQ Index: " + str(iaq_index))

    if iaq_index_accuracy == BrickletAirQuality.ACCURACY_UNRELIABLE:
        print("IAQ Index Accuracy: Unreliable")
    elif iaq_index_accuracy == BrickletAirQuality.ACCURACY_LOW:
        print("IAQ Index Accuracy: Low")
    elif iaq_index_accuracy == BrickletAirQuality.ACCURACY_MEDIUM:
        print("IAQ Index Accuracy: Medium")
    elif iaq_index_accuracy == BrickletAirQuality.ACCURACY_HIGH:
        print("IAQ Index Accuracy: High")

    integrate.set_air_quality_temperature(temperature / 100.0)
    integrate.set_air_quality_humidity(humidity / 100.0)
    integrate.set_air_quality_pressure(air_pressure / 100.0)
    
    print("AIRQ Temperature: " + str(temperature / 100.0) + " °C")
    print("AIRQ Humidity: " + str(humidity / 100.0) + " %RH")
    print("Air Pressure: " + str(air_pressure/100.0) + " hPa")
    



def aq_initialize(ipcon):
    aq = BrickletAirQuality(AirQuality_UID, ipcon)

    aq.register_callback(aq.CALLBACK_ALL_VALUES, cb_all_values)
    aq.set_all_values_callback_configuration(2000, False)

    return aq

def aq_reinitialize(uid, device_id, enum_type, ipcon):
    global  reinit
    if device_id == BrickletAirQuality.DEVICE_IDENTIFIER:
        if enum_type in (IPConnection.ENUMERATION_TYPE_CONNECTED, IPConnection.ENUMERATION_TYPE_AVAILABLE):
            print(f"💡 Air Quality Bricklet ({uid}) connected.")
            if reinit:
                print(f"🔄 Reinitializing Air Quality Bricklet ({uid}).")
                aq_initialize(ipcon)
            reinit = True

def aq_get_all_readings(aq):
    iaq_index, accuracy, temperature, humidity, air_pressure = aq.get_all_values()

    print("\n📈 Current Air Quality Readings:")
    print(f"  🌫️ IAQ Index     : {iaq_index / 100.0:.2f}")
    print(f"  🎯 Accuracy       : {accuracy} (0=Low, 3=High)")
    print(f"  🌡️ Temperature    : {temperature / 100.0:.2f} °C")
    print(f"  💧 Humidity       : {humidity / 100.0:.2f} %RH")
    print(f"  🌀 Air Pressure   : {air_pressure / 1000.0:.3f} hPa")


def aq_display_menu(aq):
    while True:
        print("\n🔹 Air Quality Menu:")
        print("1. Show current readings")
        print("2. Exit")

        choice = input("Enter your choice (1-2): ")

        if choice == "1":
            aq_get_all_readings(aq)
        elif choice == "2":
            print("🚪 Exiting Air Quality Bricklet application.")
            break
        else:
            print("❌ Invalid choice. Please select 1 or 2.")

def aq_main():
    global ipcon
    ipcon = create_connection()
    try:
        # Connect only if not already connected
        if ipcon.get_connection_state() != IPConnection.CONNECTION_STATE_CONNECTED:
            ipcon.connect("192.168.150.15", 4223)
        else:
            print("ℹ️ Already connected to Brick Daemon.")

        aq = aq_initialize(ipcon)

        # Start calling cb_connected every 1 second for unit testing
        #start_periodic_connected_callback(ipcon)

        aq_display_menu(aq)
    except Exception as e:
        print(f"❌ Error: {e}")
    finally:
        try:
            ipcon.disconnect()
        except Exception as e:
            print("Exception" , e)
            
        print("✅ Disconnected from Brick Daemon.")



if __name__ == "__main__":
    aq_main()



