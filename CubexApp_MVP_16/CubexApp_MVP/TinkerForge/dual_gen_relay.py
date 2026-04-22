"""

Industrial Dual AC Relay sample and tubing heater

UID : UhS

"""
from tinkerforge.bricklet_industrial_dual_ac_relay import BrickletIndustrialDualACRelay
from TinkerForge.tforge_con import create_connection
from TinkerForge.Bricklet_ID_Configuration import Dual_Gen_Relay_UID, DUAL_GEN_NUM_RELAYS


def dual_gen_initialize_relay(ipcon):
    """
    Initializes the Industrial Dual AC Relay Bricklet.

    Returns:
        IndustrialDualACRelay: The initialized dualrelay instance.
        IPConnection: The Tinkerforge IP connection instance.
    """
    dualrelay = BrickletIndustrialDualACRelay(Dual_Gen_Relay_UID, ipcon)
    return dualrelay

def dual_gen_get_relay_status(relay):
    """
    Fetches and displays the current relay states.

    Args:
        relay (BrickletIndustrialDualACRelay): The relay bricklet instance.

    Returns:
        list: The current relay states as a list of booleans.
              Index 0 = Relay 1, Index 1 = Relay 2
    """
    relay_states = relay.get_value()
    #print(f"\n📌 Current Relay Status: {relay_states}")
    return relay_states


def dual_gen_turn_on_relay(relay, relay_index):
    """
    Turns ON the selected relay while keeping others unchanged.

    Args:
        relay (BrickletIndustrialDualACRelay): The relay bricklet instance.
        relay_index (int): The index of the relay to turn ON (0 or 1).
    """
    if relay_index not in [0, 1]:
        raise ValueError("Relay index must be 0 or 1 (corresponding to Relay 1 or Relay 2)")

    # Get current relay states
    relay_states = list(dual_gen_get_relay_status(relay))
    relay_states[relay_index] = True  # Turn ON selected relay

    # Apply both relay states back
    #relay.set_value(relay_states)
    relay.set_value(relay_states[0], relay_states[1])
    #print(f"✅ Relay {relay_index} is now ON.")


def dual_gen_turn_off_relay(relay, relay_index):
    """
    Turns OFF the selected relay while keeping others unchanged.

    Args:
        relay (BrickletIndustrialDualACRelay): The relay bricklet instance.
        relay_index (int): The index of the relay to turn OFF (0 or 1).
    """
    if relay_index not in [0, 1]:
        raise ValueError("Relay index must be 0 or 1 (corresponding to Relay 1 or Relay 2)")

    # Get current relay states
    relay_states = list(dual_gen_get_relay_status(relay))
    relay_states[relay_index] = False  # Turn OFF selected relay

    # Apply both relay states back
    #relay.set_value(relay_states)
    relay.set_value(relay_states[0], relay_states[1])
    #print(f"✅ Relay {relay_index} is now OFF.")
    

def dual_gen_turn_on_both_relays(relay):
    """
    Turns ON both relays (Relay 1 and Relay 2) using set_value().
    """
    relay.set_value(True, True)
    print("✅ Both relays are now ON.")


def dual_gen_turn_off_both_relays(relay):
    """
    Turns OFF both relays (Relay 1 and Relay 2) using set_value().
    """
    relay.set_value(False, False)
    print("✅ Both relays are now OFF.")
    
def dual_gen_relay_control_menu(relay):
    """
    Displays the user menu for Dual AC Relay control and executes the selected action.

    Args:
        relay (BrickletIndustrialDualACRelay): The relay bricklet instance.
    """

    while True:
        # Show current status
        dual_gen_get_relay_status(relay)

        print("\n🔹 Relay Control Menu:")
        for i in range(DUAL_GEN_NUM_RELAYS):
            print(f"{i*2+1}. Turn ON Relay {i}")
            print(f"{i*2+2}. Turn OFF Relay {i}")
        print(f"{DUAL_GEN_NUM_RELAYS*2+1}. Exit")

        choice = input(f"Enter your choice (1-{DUAL_GEN_NUM_RELAYS*2+1}): ")

        try:
            choice = int(choice)
            if 1 <= choice <= DUAL_GEN_NUM_RELAYS * 2:
                relay_index = (choice - 1) // 2
                if choice % 2 == 1:
                    dual_gen_turn_on_relay(relay, relay_index)
                else:
                    dual_gen_turn_off_relay(relay, relay_index)
            elif choice == DUAL_GEN_NUM_RELAYS * 2 + 1:
                print("🚪 Exiting program.")
                break
            else:
                print(f"❌ Invalid choice! Please enter a number between 1 and {DUAL_GEN_NUM_RELAYS*2+1}.")
        except ValueError:
            print("❌ Invalid input! Please enter a valid number.")

    
def dual_gen_main():
    """
    Main function to initialize and run the user menu for relay control.
    """
    ip_con = create_connection()

    relay = dual_gen_initialize_relay(ip_con)
    
    dual_gen_relay_control_menu(relay)
    
    ip_con.disconnect()
    print("✅ Relay Control Completed.")

if __name__ == "__main__":
    dual_gen_main()    



