#! python
# -*- coding: utf-8 -*-
# (C) Copyright 2024-2025 Advanced Instrumentation Systems, LLC, All Rights Reserved
#
# Python bindings to interface to Sensirion device
# Functions invoke identical functions in file sensirion_uart_sfx6xxx, device.py
# Sensirion BSD3 License file included and must be kept with Sensirion source code.
#
import time

from TinkerForge.Bricklet_ID_Configuration import mfc1_serial_port
from MFC_def import Sfc6
# setup serial_port string  "hostname/port/Tinkerforge ID"


if __name__ == "__main__":
    # setup ser_port string  "hostname/port/Tinkerforge ID"
    

	
    id = mfc1_serial_port
    mfc_sfc6 = Sfc6()

    rc = mfc_sfc6.connect(mfc1_serial_port, baudrate=115200)
    if rc != None:
        print(rc)
        exit(0)
    
    mfc_sfc6.set_calibration(id, 4)


    # User menu for LED control
    while True:
        print("\n MFC Test Options:")
        print("1. MFC 0% Closed")
        print("2. MFC 30% Closed")
        print("3. MFC 50% Closed")
        print("4. MFC 100% Closed")
        print("5. Exit")
        
        choice = input("Enter your choice (1/2/3): ")
        
        if choice == "1":
            mfc_sfc6.setpoint_percentage(mfc1_serial_port, 0)
            # Wait until flow stabilizes within 0.002
            for ii in range(10):
                if abs( mfc_sfc6.read_average_value(mfc1_serial_port, 5) - flow)>0.002:
                    time.sleep(0.5)
                else:
                    break
            print(f'flow = {mfc_sfc6.read_average_value(mfc1_serial_port, 5)}')
        elif choice == "2":
        
            # Set Flow Rate to 30% of Max Flow
            mfc_sfc6.setpoint_percentage(mfc1_serial_port, 30)
            print(f"Flow Rate Set to {target_flow} SCCM (30%)")

            # Wait for flow to stabilize
            time.sleep(2)
            print(f'Raw Flow = {mfc_sfc6.measure_raw_flow(mfc1_serial_port)}')
            print(f'flow = {mfc_sfc6.read_average_value(mfc1_serial_port, 5)}')
        elif choice == "3":
        
            # Set Flow Rate to 50% of Max Flow
            mfc_sfc6.setpoint_percentage(mfc1_serial_port, 50)
            print(f"Flow Rate Set to {target_flow} SCCM (50%)")

            # Wait for flow to stabilize
            time.sleep(2)
            print(f'Raw Flow = {mfc_sfc6.measure_raw_flow(mfc1_serial_port)}')
            print(f'flow = {mfc_sfc6.read_average_value(mfc1_serial_port, 5)}')
        elif choice == "4":
        
            # Set Flow Rate to 100% of Max Flow
            mfc_sfc6.setpoint_percentage(mfc1_serial_port, 100)
            print(f"Flow Rate Set to {target_flow} SCCM (100%)")

            # Wait for flow to stabilize
            time.sleep(2)
            print(f'Raw Flow = {mfc_sfc6.measure_raw_flow(mfc1_serial_port)}')
            print(f'flow = {mfc_sfc6.read_average_value(mfc1_serial_port, 5)}')
        elif choice == "5":
            print("Exiting program.")
            break
        else:
            print("Invalid choice! Please enter 1, 2, 3, 4 and 5.")

