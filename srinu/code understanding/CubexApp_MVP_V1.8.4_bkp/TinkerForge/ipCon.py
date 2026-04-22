import time
from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_rs485 import BrickletRS485

HOST = "localhost"
PORT = 4223
UID_RS485 = "28Du"

ipcon = IPConnection()
rs485 = BrickletRS485(UID_RS485, ipcon)
ipcon.connect(HOST, PORT)

rs485.set_rs485_configuration(115200, 0, 1, 8, 1)

# Try sending dummy bytes and reading response
try:
    rs485.write([0x01, 0x02, 0x03])
    time.sleep(0.1)
    resp = rs485.read(10)
    print("Received:", resp)
except Exception as e:
    print("Error communicating:", e)

ipcon.disconnect()


