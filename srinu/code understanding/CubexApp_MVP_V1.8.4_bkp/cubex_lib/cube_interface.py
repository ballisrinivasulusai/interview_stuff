#!/usr/bin/env python
# Copyright (c) 2023-2024 VOC Health, Inc.
# Serge Lafontaine, 20241217
import logging
import os  # required until FirstView fixes their software
#os.environ['PATH'] = "/opt/homebrew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"
os.environ['PATH'] = "/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"

import subprocess
import sys # required until FirstView fixes their software
import numpy as np # required for reading wave files
import time
# Wave imports
sys.path.append(os.getcwd()+'/gen_py') # Simplifies importing waverpc files
from cubex_lib.waverpc import WaveIntFunctions
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from cubex_lib.wavedecode import ReadMeasBinFile
class Cube:
    h = {} # Dictionary for storing classes by id when created
    # Default parameters
    username = 'root'
    password = 'voc@123'
    d_time = 50    # ms, must be >20 to prevent DAC glitches
    d_step = 0.02  # V
    g_step = 0.02  # V

    def __init__(self):
        # self.logger('Start Logging.')
        return
    def get_handle(self, id):
        if id=='' or id is None:
            raise ValueError('Device ID required.')
        elif id in Cube.h.keys():
            return Cube.h[id]
        else:
            raise ValueError(f'Invalid ID "{id}", {Cube.h}')
    """ def logger(self, msg):
        with open("cube.log","a") as file:
            file.write(msg+"\n")
        return """

    def Connect(self, ip_address='192.168.1.7'):
        id = ip_address
        
        # Validate IP address
        if id == '' or id is None:
            logging.error("Device ID (IP address) is required.")
            raise ValueError('Device ID required.')
        
        # Check if the device is already connected
        if id in Cube.h.keys():
            logging.warning(f"Device with ID {id} is already connected. Disconnecting the device and connecting it again with the new ID.")
            self.Disconnect(id)
            time.sleep(1)
            #return True  # Return False instead of raising an error
        
        # Attempt to connect
        try:
            cubex = CubeX(ip_address)
            #Cube.h[id] = cubex
            rc = cubex.connect()
            
            if not rc:
                logging.error(f"Failed to connect to device {id}")
                #Sdel Cube.h[id]  # Clean up if connection fails
                return rc
            Cube.h[id] = cubex
            return rc
        except Exception as e:
            logging.error(f"Error connecting to device {id}: {e}")
            if id in Cube.h.keys():
                del Cube.h[id]  # Clean up if an exception occurs
            return False

    def GetDeviceStatus(self,id):
        # self.logger(id)
        cube = self.get_handle(id)
        return cube.GetDeviceStatus()

    def SetDwell_Time_Value(self, id, d_time):
        cube = self.get_handle(id)
        rc = cube.SetDwell_Time_Value(d_time)
        return rc


    def Setd_step0Value(self, id, d_step):
        cube = self.get_handle(id)
        rc = cube.Setd_step0Value(d_step)
        return rc
        # Set g_step


    def Setg_step0Value(self, id, g_step):
        cube = self.get_handle(id)
        rc = cube.Setg_step0Value(g_step)
        return rc
        # Now apply DAC values


    def ApplyDACChannels(self, id):
        cube = self.get_handle(id)
        rc = cube.ApplyDACChannels()
        return rc
        # Set DAC1 values


    def SetDAC1ChannelValue(self, id, dac_channel, dac_value):
        cube = self.get_handle(id)
        rc = cube.SetDAC1ChannelValue(dac_channel, dac_value)
        return rc
        # Get DAC1 value


    def GetDAC1ChannelValue(self, id, channel):
        cube = self.get_handle(id)
        rc = cube.GetDAC1ChannelValue(channel)
        return rc
        # Set DAC2 value


    def SetDAC2ChannelValue(self, id, dac_channel=0, dac_value=0.):
        cube = self.get_handle(id)
        rc = cube.SetDAC2ChannelValue(dac_channel, dac_value)
        return rc
        # Get DAC2 value


    def GetDAC2ChannelValue(self, id, channel):
        cube = self.get_handle(id)
        rc = cube.GetDAC2ChannelValue(channel)
        return rc
        # Get all DAC values


    def GetDACChannelValues(self, id):
        cube = self.get_handle(id)
        return cube.GetDACChannelValues()

        # Start data acquisition
    def StartDataAcq(self, id, sampling_time):
        cube = self.get_handle(id)
        return cube.StartDataAcq(sampling_time)
        # Stop data acquisition

    def AbortMeasurement(self, id):
        cube = self.get_handle(id)
        return cube.AbortMeasurement()
        # Transfer measurement file


    def StartTransferMeasurementFile(self, id, filename ):
        #cmd = F'sshpass -p voc@123 scp -O -o StrictHostKeyChecking=no root@{id}:/media/media/measurement/tmp/rawtext {filename}'
        #cmd = f'/usr/local/bin/sshpass -p voc@123 scp -O -o StrictHostKeyChecking=no root@{id}:/media/media/measurement/tmp/rawtext {filename}'
        #cmd = f'/opt/homebrew/bin/sshpass -p voc@123 scp -O -o StrictHostKeyChecking=no root@{id}:/media/media/measurement/tmp/rawtext {filename}'
        #cmd = F'sshpass -p voc@123 scp -O -v -o StrictHostKeyChecking=no root@{id}:/media/media/measurement/tmp/rawtext {filename}'
        #cmd = f'sshpass -p voc@123 scp -O root@{id}:/media/media/measurement/tmp/rawtext {filename}'
        #cmd = (f"sshpass -p voc@123 scp -O " f"-o ConnectTimeout=10" f"-o ServerAliveInterval=5" f"-o ServerAliveCountMax=3" f"root@{id}:/media/media/measurement/tmp/rawtext {filename}")
        cmd = ["sshpass", "-p", "voc@123","scp", "-O","-o", "StrictHostKeyChecking=no","-o", "ConnectTimeout=10","-o", "ServerAliveInterval=5","-o", "ServerAliveCountMax=3",f"root@{id}:/media/media/measurement/tmp/rawtext",filename]        
        #result = subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE) 
        try:
            result = subprocess.run(cmd,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        except subprocess.TimeoutExpired:
            print("SCP timed out")
            return False, -1, cmd

        flag = False
        if result.returncode == 0:
            print("Command executed successfully.")
            flag = True
        else:
            print("Command failed.")
            print("Error:", result.stderr.decode())
            flag = False            
        return flag,result.returncode,cmd
    
    # Read the binary file
    def ReadCubeFile(self, filename='data/rawtext'):
        metadata, temp_humid, data = ReadMeasBinFile(filename)
        keys = list(metadata.keys())
        metadata_values = list(metadata.values())
        temp_humid = temp_humid.tolist()
        data = np.transpose(data).tolist()
        return ((keys, metadata_values), temp_humid, data)
        # Close - Disconnect from Cube


    def Disconnect(self, id):
        try:
            print(f"Disconnecting device at IP: {id}")
            cube = self.get_handle(id)
            rc = cube.CloseConnection()
            
            # Remove the device from the dictionary
            if id in Cube.h.keys():
                del Cube.h[id]
            
            return rc
        except Exception as e:
            logging.error(f"Error disconnecting device {id}: {e}")
            return False


'''__________________________________________________________________________________________________________________'''
class CubeX:
    def __init__(self, ip_address='192.168.10.2'):
        self.ip_address = ip_address
        self.username = 'root'
        self.password = 'voc@123'
        self.directory = os.getcwd()
        self.d_time = 50 # ms
        self.d_step = 0.02 # V
        self.g_step = 0.02 # V

    def connect(self):
        global tprotocol
        try:
            print(f"Connecting to {self.ip_address} IP address")
            transport = TSocket.TSocket(self.ip_address, 9090)
            transport.setTimeout(20000)
            transport = TTransport.TBufferedTransport(transport)
            tprotocol = TBinaryProtocol.TBinaryProtocol(transport)
            transport.open()
            self.client = WaveIntFunctions.Client(tprotocol)
            rc = True # no error if it gets here
        except Exception:
            rc = False
        return rc
    
    # Get Device Status
    def GetDeviceStatus(self):
        print("GetDeviceStatus Cube_interface")
        struct = self.client.GetDeviceStatus()
        return struct.ready_status

    # Set dwell time
    def SetDwell_Time_Value(self, d_time=None):
        if d_time is None:
            d_time = self.d_time
        rc = self.client.SetDwell_Time_Value(int(d_time))
        return rc
    # Set d_step
    def Setd_step0Value(self, d_step=None):
        if d_step is None:
            d_step = self.d_step
        rc = self.client.Setd_step0Value(float(d_step))
        return rc
    # Set g_step
    def Setg_step0Value(self, g_step=None):
        if g_step is None:
            g_step = self.g_step
        rc = self.client.Setg_step0Value(float(g_step))
        return rc
    # Now apply DAC values
    def ApplyDACChannels(self):
        rc = self.client.ApplyDACChannels()
        return rc
    # Set DAC1 values
    def SetDAC1ChannelValue(self, dac_channel=0, dac_value=0.):
        rc = self.client.SetDAC1ChannelValue(int(dac_channel),  float(dac_value))
        return rc
    # Get DAC1 value
    def GetDAC1ChannelValue(self, channel):
        rc = self.client.GetDAC1ChannelValue(int(channel))
        return rc
    # Set DAC2 value
    def SetDAC2ChannelValue(self, dac_channel=0, dac_value=0.):
        rc = self.client.SetDAC2ChannelValue(int(dac_channel),  float(dac_value))
        return rc
    # Get DAC2 value
    def GetDAC2ChannelValue(self, channel):
        rc = self.client.GetDAC2ChannelValue(int(channel))
        return rc
    # Get all DAC values
    def GetDACChannelValues(self):
        dac_values = self.client.GetDACChannelValues()
        dac1_config = list(dac_values.dac1Config.values())
        dac2_config = list(dac_values.dac2Config.values())
        d_setup = dac_values.set_d_setup
        g_setup = dac_values.set_g_setup
        dwell = dac_values.t_dwell
        thrift = dac_values.thrift_spec
        return (dac1_config, dac2_config, d_setup, g_setup, dwell, thrift)
    # Start data acquisition
    def StartDataAcq(self, sampling_time=0):
        if sampling_time > 0 :
            rc = self.client.StartDataAcq(int(sampling_time))
        else:
            rc = 1
        return rc
    # Stop data acquisition
    def AbortMeasurement(self):
        rc = self.client.AbortMeasurement()
        return rc
    # Transfer measurement file
    def StartTransferMeasurementFile(self, filename='rawtext'):
        cmd = F'sshpass -p voc@123 scp -o StrictHostKeyChecking=no root@{self.ip_address}:/media/media/measurement/tmp/rawtext ./{filename}'
        rc = os.system(cmd)
        return rc
    # Read the binary file
    def ReadCubeFile(self, filename='data/rawtext'):
        metadata, temp_humid, data = ReadMeasBinFile(filename)
        keys = list(metadata.keys())
        metadata_values = list(metadata.values())
        temp_humid = temp_humid.tolist()
        data = np.transpose(data).tolist()
        return ((keys, metadata_values), temp_humid, data)
    # Close - Disconnect from Cube
    def CloseConnection(self):
        rc = self.client.CloseConnection()
        return rc


if __name__ == "__main__":
    cube = Cube()
    # Setup the correct ip accress.
    id = '192.168.1.8'
    rc = cube.Connect(id)
    if not rc:
        print('Fail to connect to cube.')

    # Get Device Status
    status = cube.GetDeviceStatus(id)
    if not status:
        print('Device not ready')
    else:
        print('Device ready.')
 

