"""
Industrial Dual AC Relay sample and tubing heater
UID : UaD
"""

import integrate
from tinkerforge.bricklet_industrial_dual_ac_relay import BrickletIndustrialDualACRelay
from TinkerForge.tforge_con_temp import create_connection
from TinkerForge.Bricklet_ID_Configuration import Dual_Relay_UID, DUAL_NUM_RELAYS
from tinkerforge.ip_connection import IPConnection

reinit = False
relay_global = None          # <-- NEW: To keep updated relay reference
Dual_Relay_UID = "Unt"

# ------------------------------------------------------------
# NEW: Validation check to prevent NoneType errors
# ------------------------------------------------------------
def is_relay_valid(relay):
    if relay is None:
        print("❌ Relay is None (not initialized or disconnected).")
        return False
    try:
        relay.get_identity()      # Quick check
        return True
    except Exception:
        print("❌ Relay Bricklet lost connection.")
        return False


# ------------------------------------------------------------
# Initialize Dual Relay
# ------------------------------------------------------------
def dual_initialize_relay(ipcon):
    """
    Initializes the Industrial Dual AC Relay Bricklet.
    """
    try:
        dualrelay = BrickletIndustrialDualACRelay(Dual_Relay_UID, ipcon)
        print(f"Dual Relay Object INIT: {dualrelay}")
        integrate.set_dual_relay(dualrelay)
        print("✅ Dual Relay initialized successfully.")
        return dualrelay
    except Exception as e:
        print(f"❌ Failed to initialize Dual Relay: {e}")
        return None


# ------------------------------------------------------------
# Reinitialize relay when enumeration happens
# ------------------------------------------------------------
def dual_reinitialize_relay(uid, device_id, enum_type, ipcon):
    global reinit, relay_global

    if device_id == BrickletIndustrialDualACRelay.DEVICE_IDENTIFIER:
        if enum_type in (
            IPConnection.ENUMERATION_TYPE_CONNECTED,
            IPConnection.ENUMERATION_TYPE_AVAILABLE,
        ):
            print(f"💡Dual Relay ({uid}) connected.")

            if reinit:
                print(f"🔄 Reinitializing dual relay ({uid}).")
                relay_global = dual_initialize_relay(ipcon)   # <-- FIXED

            reinit = True


# ------------------------------------------------------------
# Get relay status (SAFE)
# ------------------------------------------------------------
def dual_get_relay_status(relay):
    if not is_relay_valid(relay):
        return [False, False]

    try:
        relay_states = relay.get_value()
        print(f"✅ get Relay {relay_states}.")
        return list(relay_states)
    except Exception as e:
        print(f"Error fetching relay status: {e}")
        return [False, False]


# ------------------------------------------------------------
# Turn ON a relay (SAFE)
# ------------------------------------------------------------
def dual_turn_on_relay(relay, relay_index):
    if not is_relay_valid(relay):
        return

    try:
        if relay_index not in [0, 1]:
            raise ValueError("Relay index must be 0 or 1")

        relay_states = dual_get_relay_status(relay)

        
        if relay_states[relay_index] is False:
            relay_states[relay_index] = True
            relay.set_value(relay_states[0], relay_states[1])
            print(f"✅ Relay {relay_states} is now ON.")
    except Exception as e:
        print(f"Error turning on relay {relay_index}: {e}")


# ------------------------------------------------------------
# Turn OFF relay (SAFE)
# ------------------------------------------------------------
def dual_turn_off_relay(relay, relay_index):
    if not is_relay_valid(relay):
        return

    try:
        if relay_index not in [0, 1]:
            raise ValueError("Relay index must be 0 or 1")

        relay_states = dual_get_relay_status(relay)

        if relay_states[relay_index] is True:
            relay_states[relay_index] = False
            relay.set_value(relay_states[0], relay_states[1])
            print(f"✅ Relay {relay_states} is OFF now.")
    except Exception as e:
        print(f"Error turning off relay {relay_index}: {e}")


# ------------------------------------------------------------
# Turn ON both relays
# ------------------------------------------------------------
def dual_turn_on_both_relays(relay):
    if not is_relay_valid(relay):
        return

    try:
        relay.set_value(True, True)
        print("✅ Both relays are now ON.")
    except Exception as e:
        print(f"Error turning on both relays: {e}")


# ------------------------------------------------------------
# Turn OFF both relays
# ------------------------------------------------------------
def dual_turn_off_both_relays(relay):
    if not is_relay_valid(relay):
        return

    try:
        relay.set_value(False, False)
        print("✅ Both relays are now OFF.")
    except Exception as e:
        print(f"Error turning off both relays: {e}")


# ------------------------------------------------------------
# User Control Menu
# ------------------------------------------------------------
def dual_relay_control_menu(relay):
    while True:
        dual_get_relay_status(relay)

        print("\n🔹 Relay Control Menu:")
        for i in range(DUAL_NUM_RELAYS):
            print(f"{i*2+1}. Turn ON Relay {i}")
            print(f"{i*2+2}. Turn OFF Relay {i}")
        print(f"{DUAL_NUM_RELAYS*2+1}. Exit")

        choice = input(f"Enter your choice (1-{DUAL_NUM_RELAYS*2+1}): ")

        try:
            choice = int(choice)
            if 1 <= choice <= DUAL_NUM_RELAYS * 2:
                relay_index = (choice - 1) // 2
                if choice % 2 == 1:
                    dual_turn_on_relay(relay, relay_index)
                else:
                    dual_turn_off_relay(relay, relay_index)
            elif choice == DUAL_NUM_RELAYS * 2 + 1:
                print("🚪 Exiting program.")
                break
            else:
                print("❌ Invalid choice! Try again.")
        except ValueError:
            print("❌ Invalid input! Please enter a number.")


# ------------------------------------------------------------
# MAIN FUNCTION
# ------------------------------------------------------------
def dual_main():
    global relay_global

    ip_con = create_connection()

    relay_global = dual_initialize_relay(ip_con)

    if relay_global is None:
        print("❌ Relay not initialized. Exiting.")
        return

    dual_relay_control_menu(relay_global)

    ip_con.disconnect()
    print("✅ Relay Control Completed.")


if __name__ == "__main__":
    dual_main()



