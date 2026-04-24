"""

This is the common connection function.

"""

import time
from tinkerforge.ip_connection import IPConnection
from TinkerForge.config import HOST, PORT
#from tinkerforge.bricklet_air_quality import BrickletAirQuality
from SharedState import SharedState
from TinkerForge.Bricklet_ID_Configuration import (MasterBrick1_UID,MasterBrick2_UID,MasterBrick3_UID,AirQuality_UID,
MFC1_UID,MFC2_UID,MFC3_UID,MFC4_UID,SolenoidValve_Qrelay1_UID,ThermoCouple1_UID,ThermoCouple2_UID,Dual_Relay_UID,Vial_Base_UID)

ipcon = None
shared_state = SharedState()
mvp_version = shared_state.get_MVP_Device_Version().rsplit("_", 1)[-1]
ThermoCouple1_UID = "2f8A"
Dual_Relay_UID = "Unt"

MVPBrick_IDs = [False,"6e9d2q", False,"28BN", False,"28Bi", False,"28BA", False,"28Cb", False,"6k4qx5", False,"5XaqSC", False,"2bjH", False,"2ed3", False,"Unt", False,"2f8A", False,"29Hi"]


# Connection & Enumeration Callbacks
def cb_connected(reason):
    global ipcon
    print(f"🔗 Connected (reason: {reason})")
    ipcon.enumerate()  # Trigger device discovery


def cb_disconnected(reason):
    print(f"❌ Disconnected (reason: {reason})")


def cb_enumerate(uid, connected_uid, position, hwv, fwv, device_id, enum_type):
    global ipcon, MVPBrick_IDs   

    if enum_type in (IPConnection.ENUMERATION_TYPE_CONNECTED,IPConnection.ENUMERATION_TYPE_AVAILABLE):
        print(f"Bricklet {uid} connected.")

        for i in range(0, len(MVPBrick_IDs), 2):
            if MVPBrick_IDs[i + 1] == uid:
                MVPBrick_IDs[i] = True
                break
 
        if uid == Dual_Relay_UID:
            print(f"🔍 Enumerate: UID: {uid}, Device ID: {device_id}, Enum Type: {enum_type}")
            from TinkerForge.dual_relay_test import dual_initialize_relay
            dual_initialize_relay(ipcon)

        if uid == ThermoCouple1_UID:
            print(f"🔍 Enumerate: UID: {uid}, Device ID: {device_id}, Enum Type: {enum_type}")
            from TinkerForge.ThermoCouple_1_test import tc1_initialize_thermocouple 
            tc1_initialize_thermocouple(ipcon)   

        
    elif enum_type == IPConnection.ENUMERATION_TYPE_DISCONNECTED:
        print(f"Bricklet {uid} disconnected.")    

        # for i in range(0, len(MVPBrick_IDs), 2):
        #     if MVPBrick_IDs[i + 1] == uid:
        #         MVPBrick_IDs[i] = False
        #         break
            
def get_bricklet_connection_status():
    """Returns the connection status of all required Bricklets."""
    global MVPBrick_IDs
    Missing_Bricklet = False
    ID = 0

    for i in range(0, len(MVPBrick_IDs), 2):
       if not MVPBrick_IDs[i]: 
           ID = MVPBrick_IDs[i+1]
           Missing_Bricklet = True       
           break
    
    return Missing_Bricklet, ID
   
def create_connection():
    """Creates and returns a Tinkerforge IP connection."""
    global ipcon, MVPBrick_IDs
    Missing_Bricklet = False
    ID = 0


    for i in range(0, len(MVPBrick_IDs), 2):
       MVPBrick_IDs[i] = False     

    ipcon = IPConnection()
    ipcon.connect(HOST, PORT)

    # Register common callbacks
    ipcon.register_callback(IPConnection.CALLBACK_CONNECTED, cb_connected)
    ipcon.register_callback(IPConnection.CALLBACK_DISCONNECTED, cb_disconnected)
    ipcon.register_callback(IPConnection.CALLBACK_ENUMERATE, cb_enumerate)    
    
    
    time.sleep(2)
    for i in range(0, len(MVPBrick_IDs), 2):
        if not MVPBrick_IDs[i]:
            ID = MVPBrick_IDs[i+1] 
            Missing_Bricklet = True       
            break
    
    # if Missing_Bricklet:
    #     print("❌ One or more required Bricklets are not connected. Please check the connections.")    
    #     ipcon.disconnect()
    #     ipcon = None
    # else:
    #     print("✅ All required Bricklets are connected.")
        
    return ipcon, ID




