import time
from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_nfc import BrickletNFC

# Connection details
HOST = "localhost"
PORT = 4223
UID = "2aAy"  # Replace with your NFC Bricklet UID

# Connect to NFC Bricklet
ipcon = IPConnection()
nfc = BrickletNFC(UID, ipcon)
ipcon.connect(HOST, PORT)

# Set NFC Bricklet to Reader Mode
nfc.set_mode(BrickletNFC.MODE_SIMPLE)

# Request NFC Tag ID
print("Requesting NFC Tag...")
nfc.reader_request_tag_id()

# Wait until the tag is detected
while True:
    state, idle = nfc.reader_get_state()
    
    if state == BrickletNFC.READER_STATE_REQUEST_TAG_ID_READY:
        print("NFC Tag Detected!")
        break
    elif state == BrickletNFC.READER_STATE_ERROR:
        print("Error detecting NFC Tag.")
        break
    
    time.sleep(0.1)

# Read the NFC Tag ID
tag_type, tag_id = nfc.simple_get_tag_id(0)
print(f"Tag Type: {tag_type}")
print(f"Tag ID: {tag_id}")

# Disconnect
ipcon.disconnect()
