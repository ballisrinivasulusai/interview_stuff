"""

SolenoidValve_Qrelay Unit test application.
These APIs can be reused for testing the SolenoidValve_Qrelay which
can be called from the some other file.

UID : 2bjK

"""

import integrate
from tinkerforge.bricklet_industrial_quad_relay_v2 import BrickletIndustrialQuadRelayV2
from TinkerForge.tforge_con import create_connection
from TinkerForge.Bricklet_ID_Configuration import SolenoidValve_Qrelay1_UID, SV1_NUM_RELAYS
from tinkerforge.ip_connection import IPConnection

reinit = False
def sv1_initialize_relay(ipcon):
    """
    Initializes the Industrial Quad Relay V2 Bricklet.

    Returns:
        BrickletIndustrialQuadRelayV2: The initialized relay instance.
        IPConnection: The Tinkerforge IP connection instance.
    """
    
    relay = BrickletIndustrialQuadRelayV2(SolenoidValve_Qrelay1_UID, ipcon)
    integrate.set_sv1_relay(relay)    
    return relay

def sv1_reinitialize_relay(uid, device_id, enum_type, ipcon):
    global  reinit
    if device_id == BrickletIndustrialQuadRelayV2.DEVICE_IDENTIFIER:
        if enum_type in (IPConnection.ENUMERATION_TYPE_CONNECTED, IPConnection.ENUMERATION_TYPE_AVAILABLE):
            print(f"💡Solenoid Valve Qrelay ({uid}) connected.")
            if reinit == True:
                print(f"🔄 Reinitializing Solenoid Valve Qrelay ({uid}).")
                sv1_initialize_relay(ipcon)
            reinit = True

def sv1_backflush_relay(relay):
    relay_states = [False,False,False,False]
    try:
        relay_states = list(relay.get_value())
        print(f"relay state og bkf : {relay_states}")
        backflush_states = [False,True,True,True]
        if relay_states == backflush_states:
            return True
    except Exception as e:
        print(f" Exception Backflash ON Relay States: {e}")
        return False
    
    return False


def sv1_get_relay_status(relay):
    """
    Fetches and displays the current relay states.

    Args:
        relay (BrickletIndustrialQuadRelayV2): The relay bricklet instance.

    Returns:
        list: The current relay states as a list of booleans.
    """
    #relay_states = relay.get_value()
    #print(f"\n📌 Current Relay Status: {relay_states}")
    #return relay_states

    relay_states = [False, False]
    try:
        relay_states = relay.get_value()
    except Exception as e:
        print(f"Error fetching on sv1 relay status: {e}")
        relay_states = [True, True]    
        
    return relay_states

def sv1_turn_on_relay(relay, relay_index):
    """
    Turns ON the selected relay while keeping others unchanged.

    Args:
        relay (BrickletIndustrialQuadRelayV2): The relay bricklet instance.
        relay_index (int): The index of the relay to turn ON.
    """
    try:
        relay_states = list(relay.get_value())  # Get current state
        relay_states[relay_index] = True  # Turn ON the selected relay
        relay.set_value(relay_states)  # Apply changes
        print(f"✅ Relay {relay_index} is now ON.")
    except Exception as e:
        print(f"Error turning on relay {relay_index}: {e}")
        return False
        
    return True

def sv1_turn_off_relay(relay, relay_index):
    """
    Turns OFF the selected relay while keeping others unchanged.

    Args:
        relay (BrickletIndustrialQuadRelayV2): The relay bricklet instance.
        relay_index (int): The index of the relay to turn OFF.
    """
    try:
        relay_states = list(relay.get_value())  # Get current state
        relay_states[relay_index] = False  # Turn OFF the selected relay
        relay.set_value(relay_states)  # Apply changes
        print(f"✅ Relay {relay_index} is now OFF.")
    except Exception as e:
        print(f"Error turning off relay {relay_index}: {e}")
        return False
    
    return True

# ================= BACKFLUSH FUNCTIONS =================

def sv1_backflush_on(relay):
    """
    Turns ON Backflush.
    SW0=OFF, SW1=ON, SW2=ON, SW3=ON
    """
    relay_states = [False, True, True, True]
    try:
        relay_states_temp = relay.get_value()
        relay.set_value(relay_states)
        print(" Backflush ON | Relay States:", relay_states)
    except Exception as e:
        print(f" Exception Backflash ON Relay States: {e}")
        return False

    return True

def sv1_backflush_off(relay):
    """
    Turns OFF Backflush.
    SW0=ON, SW1=OFF, SW2=OFF, SW3=OFF
    """
    relay_states = [False, False, False, False]
    try:
        relay_states_temp = relay.get_value()
        relay.set_value(relay_states)
        print(" Backflush OFF | Relay States:", relay_states)
    except Exception as e:
        print(f" Exception Backflash ON Relay States: {e}")
        return False

    return True

# =================  =================  =================

def sv1_relay_control_menu(relay):
    """
    Displays the user menu for relay control and executes the selected action.

    Args:
        relay (BrickletIndustrialQuadRelayV2): The relay bricklet instance.
    """
    while True:
        # Show current status before giving user options
        sv1_get_relay_status(relay)

        print("\n🔹 Relay Control Menu:")
        for i in range(SV1_NUM_RELAYS):
            print(f"{i*2+1}. Turn ON Relay {i}")
            print(f"{i*2+2}. Turn OFF Relay {i}")
        print(f"{SV1_NUM_RELAYS*2+1}. Exit")

        choice = input(f"Enter your choice (1-{SV1_NUM_RELAYS*2+1}): ")

        try:
            choice = int(choice)
            if 1 <= choice <= SV1_NUM_RELAYS * 2:
                relay_index = (choice - 1) // 2
                if choice % 2 == 1:
                    sv1_turn_on_relay(relay, relay_index)
                else:
                    sv1_turn_off_relay(relay, relay_index)
            elif choice == SV1_NUM_RELAYS * 2 + 1:
                print("🚪 Exiting program.")
                break
            else:
                print(f"❌ Invalid choice! Please enter a number between 1 and {SV1_NUM_RELAYS*2+1}.")
        except ValueError:
            print("❌ Invalid input! Please enter a valid number.")

def sv1_main():
    """
    Main function to initialize and run the user menu for relay control.
    """
    ip_con = create_connection()

    relay = sv1_initialize_relay(ip_con)
    
    sv1_relay_control_menu(relay)
    
    ip_con.disconnect()
    print("✅ Relay Control Completed.")

if __name__ == "__main__":
    sv1_main()
