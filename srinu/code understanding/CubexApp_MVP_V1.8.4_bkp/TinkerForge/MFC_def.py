#! python
# -*- coding: utf-8 -*-
# (C) Copyright 2024-2025 Advanced Instrumentation Systems, LLC, All Rights Reserved
#
# Python bindings to interface to Sensirion device
# Functions invoke identical functions in file sensirion_uart_sfx6xxx, device.py
# Sensirion BSD3 License file included and must be kept with Sensirion source code.
#
import time
import sys
import os
from sensirion_driver_adapters.shdlc_adapter.shdlc_channel import ShdlcChannel
from sensirion_uart_sfx6xxx.device import Sfx6xxxDevice
from sensirion_uart_sfx6xxx.commands import StatusCode
from sensirion_tinkerforge_driver import ShdlcSerialPort
from sensirion_tinkerforge_driver.errors import ShdlcDeviceError

venv_path = os.getenv("VIRTUAL_ENV")  # Detect virtual env
if venv_path:
    site_packages_path = os.path.join(venv_path, "Lib", "site-packages")
    if site_packages_path not in sys.path:
        sys.path.append(site_packages_path)

# setup serial_port string  "hostname/port/Tinkerforge ID"

# Full Scale Flow Rate (FSFR) for Argon Gas (2 slm or 2000 sccm)
FSFR_ARGON_SLM = 0.5  # Max Flow Rate for Argon in slm

# Flow rate percentages to test
FLOW_PERCENTAGES = [0, 30, 50, 75, 100]

class Sfc6:
    h = {} # Dictionary for storing classes by id when created
    def __init__(self):
        return
    def get_handle(self, id):
        try:
            if id=='' or id is None:
                raise ValueError('Device ID required.')
            elif id in Sfc6.h.keys():
                return Sfc6.h[id]
            else:
                raise ValueError(f'Invalid ID "{id}", {Sfc6.h}')
        except Exception as e:
            print(f"[get_handle] Error: {e}")    

    def connect(self, serial_port, baudrate=115200, additional_response_time=0.01):
        try:
            id = serial_port
            if id == '' or id is None:
                raise ValueError('Device ID required.')
            elif id in Sfc6.h.keys():
                raise ValueError('Unique ID required')
            #Sfc6.h[id] = sfc6xx()
            sfc = sfc6xx()
            Sfc6.h[id] = sfc
            rc = sfc.connect(serial_port, baudrate, additional_response_time)
            if rc is not None:
                del Sfc6.h[id]
            return rc
        except Exception as e:
            print(f"[connect] Error: {e}")    
            

    def channel(self, id):
        try:
            sfc = self.get_handle(id)
        except Exception as e:
            print(f"[channel] Error: {e}")    
            return None
        return sfc.get_channel()    
            
    def serial_number(self, id):
        try:
            sfc = self.get_handle(id)            
        except Exception as e:
            print(f"[serial_number] Error: {e}")
            return None
        return sfc.get_serial_number()    

    def product_name(self, id):
        try:     
            sfc = self.get_handle(id)
        except Exception as e:
            print(f"[product_name] Error: {e}")
            return None
        return sfc.get_product_name() 
    
    def product_type(self, id):
        try:
            sfc = self.get_handle(id)            
        except Exception as e:
            print(f"[product_type] Error: {e}")
            return None
        return sfc.get_product_type()
    
    def version(self, id):
        try:
            sfc = self.get_handle(id)            
        except Exception as e:
            print(f"[version] Error: {e}")
            return None
        return sfc.get_version()

    def setpoint(self, id, level):
        try:
            sfc = self.get_handle(id)
            sfc.setpoint(level)
        except Exception as e:
            print(f"[setpoint] Error: {e}")

    def setpoint_percentage(self, id, precentage):
        sfc = self.get_handle(id)
        try:
            precentage = float(precentage)
        except ValueError:
            print(f"❌ ERROR: Invalid percentage value '{precentage}'. Must be a number.")
            return 0

        desired_flow_slm = FSFR_ARGON_SLM * (precentage / 100.0) 
        if 0 <= desired_flow_slm <= FSFR_ARGON_SLM:
            print(f"Setting Flow SLM to '{desired_flow_slm}'")
            sfc.setpoint(int(desired_flow_slm))
        else:
            print(f"❌ ERROR: Flow rate {desired_flow_slm:.3f} slm is out of range (0 - {FSFR_ARGON_SLM} slm).")
            return 0
        return 1


    def measured_value(self, id,):
        try:
            sfc = self.get_handle(id)
            rc = sfc.read_measured_value()   
        except Exception as e:
            print(f"[measured_value] Error: {e}")
            rc = None
        return rc
     
    def read_average_value(self, id, number_of_values=50):
        try:    
            sfc = self.get_handle(id)
            rc = sfc.read_average_value(number_of_values)
        except Exception as e:
            print(f"[read_average_value] Error: {e}")
            rc = None
        return rc
    def set_setpoint_and_read_measured_value(self, id, level):
        try:
            sfc = self.get_handle(id)
            return sfc.set_setpoint_and_read_measured_value(level)
        except Exception as e:
            print(f"[set_setpoint_and_read_measured_value] Error: {e}")
            return None

    def set_user_controller_gain(self, id, gain):
        try:
            sfc = self.get_handle(id)
            rc = sfc.set_user_controller_gain(gain)
            return rc
        except Exception as e:
            print(f"[set_user_controller_gain] Error: {e}")

    def get_user_controller_gain(self, id):
        try:
            sfc = self.get_handle(id)
            gain = sfc.get_user_controller_gain()       
        except Exception as e:
            print(f"[get_user_controller_gain] Error: {e}")
            gain = None
            
        return gain
    def set_user_init_step(self, id, step):
        try:
            sfc = self.get_handle(id)
            rc = sfc.set_user_init_step(step)
            return rc
        except Exception as e:
            print(f"[set_user_init_step] Error: {e}")

    def get_user_init_step(self, id):
        try:
            sfc = self.get_handle(id)
            step = sfc.get_user_init_step()            
        except Exception as e:
            print(f"[get_user_init_step] Error: {e}")
            step = None
            
        return step
    def measure_raw_flow(self, id):
        try:
            sfc = self.get_handle(id)
            raw_flow = sfc.measure_raw_flow()
            return raw_flow
        except Exception as e:
            print(f"[measure_raw_flow] Error: {e}")

    def measure_raw_thermal_conductivity_with_closed_valve(self, id):
        try:
            sfc = self.get_handle(id)
            tc = sfc.measure_raw_thermal_conductivity_with_closed_valve()
            return tc
        except Exception as e:
            print(f"[measure_raw_thermal_conductivity_with_closed_valve] Error: {e}")

    def temperature(self, id):
        try:
            sfc = self.get_handle(id)
            tc = sfc.measure_temperature()    
        except Exception as e:
            print(f"[temperature] Error: {e}")
            tc = None
        return tc
    
    def get_number_of_calibrations(self, id):
        try:
            sfc = self.get_handle(id)
            nc = sfc.get_number_of_calibrations()            
        except Exception as e:
            print(f"[get_number_of_calibrations] Error: {e}")
            nc = None            
        return nc
    
    def get_calibration_validity(self, id, index):
        try:
            sfc = self.get_handle(id)
            rc = sfc.get_calibration_validity(index)            
        except Exception as e:
            print(f"[get_calibration_validity] Error: {e}")
            rc = None
        return rc
    
    def get_calibration_gas_id(self, id, index):
        try:
            sfc = self.get_handle(id)
            rc = sfc.get_calibration_gas_id(index)            
        except Exception as e:
            print(f"[get_calibration_gas_id] Error: {e}")
            rc = None
        return rc    

    def get_calibration_gas_unit(self, id, index):
        try:
            sfc = self.get_handle(id)
            rc = sfc.get_calibration_gas_unit(index)            
        except Exception as e:
            print(f"[get_calibration_gas_unit] Error: {e}")
            rc = None            
        return rc

    def calibration_fullscale(self, id, index):
        try:
            sfc = self.get_handle(id)
            rc = sfc.get_calibration_gas_unit(index)            
        except Exception as e:
            print(f"[calibration_fullscale] Error: {e}")
            rc = None            
        return rc
    
    def get_current_gas_id(self, id):
        try:
            sfc = self.get_handle(id)
            cid = sfc.get_current_gas_id()
        except Exception as e:
            print(f"[get_current_gas_id] Error: {e}")
            cid = None
        return cid

    def get_current_gas_unit(self, id):
        try:
            sfc = self.get_handle(id)
            gcu = sfc.get_current_gas_unit()
        except Exception as e:
            print(f"[get_current_gas_unit] Error: {e}")
            gcu = None
        return gcu

    def set_calibration_volatile(self, id, cal):
        try:
            sfc = self.get_handle(id)
            rc = sfc.set_calibration_volatile(cal)
        except Exception as e:
            print(f"[set_calibration_volatile] Error: {e}")
        return rc

    def set_calibration(self, id, cal):
        try:
            sfc = self.get_handle(id)
            rc = sfc.set_calibration(cal)
        except Exception as e:
            print(f"[set_calibration] Error: {e}")
        return rc

    def get_calibration(self, id):
        try:
            sfc = self.get_handle(id)
            cn = sfc.get_calibration()
        except Exception as e:
            print(f"[get_calibration] Error: {e}")
            cn = None
        return cn

    def set_slave_address(self, id, addr):
        try:
            sfc = self.get_handle(id)
            rc = sfc.set_slave(addr)
        except Exception as e:
            print(f"[set_slave_address] Error: {e}")
        return rc

    def get_slave_address(self, id):
        try:
            sfc = self.get_handle(id)
            addr = sfc.get_slave_address()
        except Exception as e:
            print(f"[get_slave_address] Error: {e}")
            addr = None
        return addr

    def device_reset(self, id):
        try:    
            sfc = self.get_handle(id)
            rc = sfc.device_reset()
        except Exception as e:
            print(f"[device_reset] Error: {e}")
        return rc

    def close_valve(self, id):
        try:
            sfc = self.get_handle(id)
            sfc.close_valve()
        except Exception as e:
            print(f"[close_valve] Error: {e}")

    def disconnect(self, id):
        try:
            sfc = self.get_handle(id)
            sfc.disconnect()
        except Exception as e:
            print(f"[disconnect] Error: {e}")

'''---------------------------------------------------------------------------------------'''
class sfc6xx:
    def __init__(self):
            self.port = None
            self.sensor = None
    def connect(self, serial_port, baudrate=115200, additional_response_time=0.01):
        if isinstance(baudrate, str):
            baudrate = int(baudrate)
        if isinstance(additional_response_time, str):
            additional_response_time = float(additional_response_time)
        try:
            self.port = ShdlcSerialPort(port=serial_port, baudrate=baudrate, additional_response_time=additional_response_time)
        except Exception as e:
            return "Failed to open serial port " + str(e)
        channel = ShdlcChannel(self.port, channel_delay=0.05)
        self.sensor = Sfx6xxxDevice(channel)
        self.sensor.device_reset()
        time.sleep(2.0)
        return None

    def get_channel(self):
        try:
            val = self.sensor.get_channel()
        except Exception as e:
            print(f"[get_channel] Error: {e}")
            val = None                
        return val

    def get_serial_number(self):
        try:
            sn = self.sensor.get_serial_number()
            time.sleep(0.5)
        except Exception as e:
            print(f"[get_serial_number] Error: {e}")
            sn = None
        return sn

    def get_product_name(self):
        try:
            pn = self.sensor.get_product_name()
        except Exception as e:
            print(f"[get_product_name] Error: {e}")
            pn = None
        return pn

    def get_version(self):
        try:
            vn = self.sensor.get_version()
        except Exception as e:
            print(f"[get_version] Error: {e}")
            vn = None
        return vn

    def get_product_type(self):
        try:
            pt = self.sensor.get_product_type()
        except Exception as e:
            print(f"[get_product_type] Error: {e}")
            pt = None
        return pt

    def setpoint(self, level):
        try:
            self.sensor.set_setpoint(float(level))
        except Exception as e:
            print(f"[setpoint] Error: {e}")

    def read_measured_value(self):
        try:
            value = self.sensor.read_measured_value()
        except Exception as e:
            print(f"[read_measured_value] Error: {e}")
            value = None
        return value

    def read_average_value(self, number_of_values=50):
        averaged_measured_value = -1.0
        try:
            averaged_measured_value = self.sensor.read_averaged_measured_value(int(number_of_values))
        except ShdlcDeviceError as e:
            if e.error_code == StatusCode.SENSOR_MEASURE_LOOP_NOT_RUNNING_ERROR.value:
                print("Most likely the valve was closed due to overheating "
                      "protection.\nMake sure a flow is applied and start the "
                      "script again.")
                self.port.close()
                averaged_measured_value = -1
        except BaseException:
            self.port.close()
        return averaged_measured_value

    def set_setpoint_and_read_measured_value(self, level):
        try:
            value = self.sensor.set_setpoint_and_read_measured_value(float(level))
        except Exception as e:
            print(f"[set_setpoint_and_read_measured_value] Error: {e}")
            value = None
        return value

    def set_user_controller_gain(self, gain):
        try:
            rc = self.sensor.set_user_controller_gain(gain)
        except Exception as e:
            print(f"[set_user_controller_gain] Error: {e}")
            rc = None
        return rc

    def get_user_controller_gain(self):
        try:
            gain = self.sensor.get_user_controller_gain()
        except Exception as e:
            print(f"[get_user_controller_gain] Error: {e}")
            gain = None
        return gain

    def set_user_init_step(self, step):
        try:
            rc = self.sensor.set_user_init_step(step)
        except Exception as e:
            print(f"[set_user_init_step] Error: {e}")
            rc = None
        return rc

    def get_user_init_step(self):
        try:
            step = self.sensor.get_user_init_step()
        except Exception as e:
            print(f"[get_user_init_step] Error: {e}")
            step = None
        return step

    def measure_raw_flow(self):
        try:
            raw_flow = self.sensor.measure_raw_flow()
        except Exception as e:
            print(f"[measure_raw_flow] Error: {e}")
            raw_flow = None
        return raw_flow

    def measure_raw_thermal_conductivity_with_closed_valve(self):
        try:
            tc = self.sensor.measure_raw_thermal_conductivity_with_closed_valve()
        except Exception as e:
            print(f"[measure_raw_thermal_conductivity_with_closed_valve] Error: {e}")
            tc = None
        return tc

    def measure_temperature(self):
        try:
            tc = self.sensor.measure_temperature()
        except Exception as e:
            print(f"[measure_temperature] Error: {e}")
            tc = None
        return tc

    def get_number_of_calibrations(self):
        try:
            nc = self.sensor.get_number_of_calibrations()
        except Exception as e:
            print(f"[get_number_of_calibrations] Error: {e}")
            nc = None
        return nc

    def get_calibration_validity(self, index):
        try:
            rc = self.sensor.get_calibration_validity(index)
        except Exception as e:
            print(f"[get_calibration_validity] Error: {e}")
            rc = None
        return rc

    def get_calibration_gas_id(self, index):
        try:
            rc = self.sensor.get_calibration_gas_id(index)
        except Exception as e:
            print(f"[get_calibration_gas_id] Error: {e}")
            rc = None
        return rc
        return rc

    def get_calibration_gas_unit(self, index):
        try:
            rc = self.sensor.get_calibration_gas_unit(index)
        except Exception as e:
            print(f"[get_calibration_gas_unit] Error: {e}")
            rc = None
        return rc

    def get_calibration_fullscale(self, index):
        try:
            rc = self.sensor.get_calibration_fullscale(index)
        except Exception as e:
            print(f"[get_calibration_fullscale] Error: {e}")
            rc = None
        return rc

    def get_current_gas_id(self):
        try:
            cid = self.sensor.get_current_gas_id()
        except Exception as e:
            print(f"[get_current_gas_id] Error: {e}")
            cid = None
        return cid

    def get_current_gas_unit(self):
        try:
            cgu = self.sensor.get_current_gas_unit()
        except Exception as e:
            print(f"[get_current_gas_unit] Error: {e}")
            cgu = None
        return cgu

    def get_calibration(self):
        try:
            cal = self.sensor.get_calibration()
        except Exception as e:
            print(f"[get_calibration] Error: {e}")
            cal = None
        return cal

    def set_calibration(self, cal):
        try:
            rc = self.sensor.set_calibration(cal)
        except Exception as e:
            print(f"[set_calibration] Error: {e}")
            rc = None
        return rc

    def get_slave_address(self):
        try:
            addr = self.sensor.get_slave_address()
        except Exception as e:
            print(f"[get_slave_address] Error: {e}")
            addr = None
        return addr
        return addr

    def set_slave_address(self, addr):
        try:
            rc = self.sensor.set_slave_addr(addr)
        except Exception as e:
            print(f"[set_slave_address] Error: {e}")
            rc = None
        return rc

    def set_calibrationt_volatile(self, cal):
        try:
            rc = self.sensor.set_calibration_volatile(cal)
        except Exception as e:
            print(f"[set_calibrationt_volatile] Error: {e}")
            rc = None
        return rc

    def device_reset(self):
        try:
            addr = self.sensor.device_reset()
        except Exception as e:
            print(f"[device_reset] Error: {e}")
            addr = None
        return addr

    def close_valve(self):
        try:
            self.sensor.close_valve()
        except Exception as e:
            print(f"[close_valve] Error: {e}")

    def disconnect(self):
        try:
            self.sensor.close_valve()
            self.port.close()
        except Exception as e:
            print(f"[disconnect] Error: {e}")
            
    # #### Set Flow
    def set_flow(self, flow=0.02):
        try:
            self.sfc6.setpoint(self.serial_port, flow)
            # Wait until flow stabilizes within 0.002
            for ii in range(10):
                if abs( self.sfc6.read_average_value(self.serial_port, 5) - flow)>0.002:
                    time.sleep(0.5)
                else:
                    break
            print(f'flow = {self.sfc6.read_average_value(self.serial_port, 5)}')
        except Exception as e:
            print(f"[set_flow] Error: {e}")

