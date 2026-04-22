# Copyright 2024-2025 Advanced Instrumentation Systems, LLC
# The software is a replacement for pyserial "serial" code, and subject to this license:
# Copyright (c) 2001-2020 Chris Liechti <cliechti@gmx.net>
# And uses code from Tinkerforge released under the CC0 1.0 Universal License

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_rs485 import BrickletRS485
import time
'''
Usage:
       self._serial = tk_serial(port=port, baudrate=baudrate,
                                 bytesize=rs485.WORDLENGTH_8,
                                 parity = rs485.PARITY_NONE,
                                 stopbits = rs485.STOPBITS_1,
                                 timeout=0.01,
                                 mode = rs485.DUPLEX_HALF,
                                 )
port is a string including HOST ip or name, port and Tinkerforge RS485 bricklet ID in the form:
"host/port/uid", e.g. "localhost/4223/DJT"
'''
class tk_serial:
    def __init__(self, port, baudrate=115200, timeout=0.01,  **kwargs):
        self.host, port_, self.uid = port.split('/')
        self.port = int(port_)
        try:
            self.ipcon = IPConnection() # Create IP connection
            self.rs485 = BrickletRS485(self.uid, self.ipcon) # Create device object
        except:
            raise Exception("Failed to connect to RS485 bricklett.")
        self.baudrate = baudrate
        self.parity = self.rs485.PARITY_NONE
        self.stopbits = self.rs485.STOPBITS_1
        self.mode = self.rs485.DUPLEX_HALF
        self.wordlength = self.rs485.WORDLENGTH_8
        self.timeout = timeout
        self.is_open = False

    # Open or connect to Tinkerforge RS485 bricklet
    def open(self):
        self.ipcon.connect(self.host, self.port)  # Connect to brickd
        # Don't use device before ipcon is connected

        # Enable half-duplex mode on the bricklet itself setting up
        # the appropriate dip switches.
        self.rs485.set_rs485_configuration(self.baudrate, self.parity, self.stopbits,
                                      self.wordlength, self.mode)
        self.is_open = True
    # Flush is not needed.
    def flush(self):
        pass #self.rs485.read(1000)
    # Flush input from unread bytes
    def flushInput(self):
        self.rs485.read(1000)
        time.sleep(0.01)
    # Write to port
    def write(self,data):
        self.rs485.write(bytes(data))
    # Read from port
    def read(self, nchars=1):
        data = self.rs485.read(2048) #nchars)
        '''
        if len(data)==0 :
            time.sleep(self.timeout)
            data = self.rs485.read(256)
        '''
        '''
        data = ()
        while True:
            data_ = self.rs485.read(2048)
            if len(data_)==0 :
                break
            else:
                data = data + data_
        '''
        data = list(data)
        for ii in range(len(data)):
            data[ii] = ord(data[ii])
        return data #self.rs485.read(nchars)
    # not needed.
    def inWaiting(self):
        #self.buffer = self.rs485.read(1000)
        #return len(self.buffer)
        return 0 # any number
    # close port
    def close(self):
        self.ipcon.disconnect()
        self.is_open = False
