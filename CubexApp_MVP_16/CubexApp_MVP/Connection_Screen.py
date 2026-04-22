import logging
import os
import sys
sys.path.append(os.path.dirname(os.path.abspath(__file__)))
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..'))
from TitleBar import TitleBar
import threading
import re
import paramiko
import subprocess
import shutil
import time
import ipaddress
from TinkerForge.tforge_con import create_connection,get_bricklet_connection_status
from PyQt5.QtWidgets import (
    QCheckBox, QLabel, QLineEdit, QPushButton, QGridLayout, QVBoxLayout, QHBoxLayout, QWidget, QMessageBox,QApplication
)
from PyQt5.QtCore import Qt, QTimer, pyqtSlot, pyqtSignal, QObject, QThread
import integrate
import faulthandler
from datetime import datetime, timedelta
from ping3 import ping
from scp import SCPClient

from SharedState import BOARD_PORT, BOARD_USER, BOARD_PASSWORD


faulthandler.enable()

class Worker(QObject):
    finished = pyqtSignal()
    update_status = pyqtSignal(str, str)

    def __init__(self):
        super().__init__()
        self.should_stop = False
        self.connecting_dots = 0

    def run(self):
        """
        This function is used to run the worker process in background.

        It will continuously update the connection status of the device
        by adding dots (.) in the text like "Connecting...".

        This loop will run until the stop() function is called.

        Returns:
            None
        """        
        self.should_stop = False        
        while not self.should_stop:
            time.sleep(1)
            self.connecting_dots = (self.connecting_dots + 1) % 4
            connecting_text = "Device1: Connecting" + "." * self.connecting_dots
            self.update_status.emit(connecting_text, "Both")
            print(f"Stop Status {self.should_stop}")
        self.finished.emit()

    def stop(self):
        """
        This function is used to stop the worker process.

        It will set the stop flag to True so that the loop
        in run() function will exit safely.

        Returns:
            None
        """        
        self.should_stop = True
        
class ConnectionWorker(QObject):
    """
    This class is used to handle device connection checking in background.

    It will continuously monitor the network status, device reachability,
    and measurement state. Based on the condition, it will update UI signals,
    handle disconnection, and control screen switching.

    It runs in a separate thread to avoid blocking the main UI.

    Signals:
        finished: Emitted when thread is stopped
        update_cancel_button: Used to update cancel button state in UI
    """    
    finished = pyqtSignal()
    update_cancel_button = pyqtSignal(bool)

    def __init__(self, parent_screen):
        """
        Initialize the connection worker.

        Args:
            parent_screen: Reference to the main screen object to access shared state and UI signals
        """        
        super().__init__()
        self.parent = parent_screen  
        self.running = True
        self.unreachable_count = 0  

    def is_valid_ip(self, ip):
        """
        Check whether the given IP address is valid.

        This function uses ipaddress module to verify the IP format.

        Args:
            ip (str): IP address to validate

        Returns:
            bool: True if valid IP, False otherwise
        """        
        try:
            ipaddress.ip_address(ip)
            return True
        except ValueError:
            return False
        

    def run(self):
        """
        Main worker function which runs in loop to monitor connection.

        This function will:
        - Check network availability
        - Ping the device IP
        - Monitor measurement state
        - Handle connection / disconnection logic
        - Control UI updates and screen switching

        It keeps running until stop() is called.

        Returns:
            None
        """        
        time.sleep(5)
        network_error = True
        while self.running:
            current_ip = self.parent.shared_state.get_device_ip_address()
            reachable = False
            Brick_ID = 0
            measurement_started = False
            screen_name = self.parent.main_window.get_current_screen_name()    
            

            # We cannot ping the device, Network is down
            # So check the Linux ethernet connection.
            if network_error and ((screen_name == "Connect") or (screen_name == "Login") or (screen_name == "AdvanceSettings")):
                #self.parent.connect_button_control_signal.emit(False)
                #print("<CT> Wait until Network is Up")
                while True:
                    netisup = self.parent.shared_state.is_eth_up()
                    if netisup:
                        break

                    integrate.set_exit_measurement_thread1(True)
                    print("Network is Down")
                    self.parent.connect_button_control_signal.emit(False) 
                    time.sleep(1)
                   
                network_error = False
                
            #print(f"Network is Up")

            #tnow = datetime.now()
            #print(f"<CT> {tnow.strftime('%Y-%m-%d %H:%M:%S')} : Connection Thread is Running...")


            #print("<CT> Checking Status")
            # Check whether the Measurement thread is reqesting the 
            # Starting the measurement
            rc = self.parent.shared_state.get_continue_measurement()
            if rc:
                #print("<CT> Measurement is going on")
                self.parent.shared_state.set_measurement_granted(True)
                measurement_started = True
            else:
                #print("<CT> Measurement is Stopped")
                self.parent.shared_state.set_measurement_granted(False)
                measurement_started = False

            # Check for the Device Connection (Not in measurement state)
            # Check for the IP connection
            print("<CT> [PING] Network", measurement_started)
            if current_ip and self.validate_ip(current_ip) and not measurement_started:
                try:
                    print(f"<CT> [PING] IP address {current_ip}")
                    if (self.is_valid_ip(current_ip)):
                        response = ping(current_ip, timeout=1)
                    else:
                        response = None

                    reachable = response is not None

                    print(f"<CT> [PING] Network status {reachable}")
                    if not reachable:
                        netisup = self.parent.shared_state.is_eth_up()
                        print(f"<CT> [PING] Network status isnetup {netisup}")
                        if not netisup:
                            print("<CT> [PING] Network is not reachable")
                            network_error = True
                            #continue

                except Exception as e:
                    print("<CT> [PING] Network unreachable Exception", e)
                    integrate.set_exit_measurement_thread1(True)
                    network_error = True
                    continue
            # App closing        
            if integrate.get_Appclose():
                integrate.set_ConnectionThread_Closed(True)
                break                      

            #print("<CT> Bricklet Enable", integrate.get_enable_bricklet())
            #print("<CT> Device Connected State ", self.parent.shared_state.get_device_connection_state())
            #print("<CT> Measurement Startd ", measurement_started)
            # Check the Connection state
            if reachable and integrate.get_enable_bricklet() and self.parent.shared_state.get_device_connection_state() and not measurement_started and self.parent.shared_state.get_PowerupBricklets_onetime_initialization():
                # Check for the Bricklet connection status             
                rc, Brick_ID = get_bricklet_connection_status()
                if rc:
                    print("<CT> Some of the Bricklet Not connected Canceling the connection")
                    self.parent.shared_state.set_PowerupBricklets_onetime_initialization(False)
                    reachable = False
                else:
                    reachable = True

            if measurement_started and self.parent.shared_state.get_device_connection_state():
                # This condition will execute only during the Measurement
                # Check whether any thrift/network failure happened during the Measurement
                if self.parent.shared_state.get_measurement_failed():
                    print("<CT> Measurement is Cancelled due to the Network Failure")                
                    reachable = False
                else:
                    reachable = True

                # Check for the Cancel button flag
                if reachable:             
                    if self.parent.shared_state.get_Cancel_event():
                        print("<CT> User Pressed Cancel button")                
                        reachable = False
                    else:
                        reachable = True
            
                # Check for the Temperature Reached
                if reachable and integrate.get_enable_bricklet():
                    if integrate.get_check_temperature():
                        print("<CT> Temperature Not reached Canceling the connection")
                        reachable = False  
                    else:
                        reachable = True                    

                                        
            # Lock this
            #print("<CT> : Acquiring Lock")

            integrate._disconnect_lock.acquire()
            # Get the current Screen name
            screen_name = self.parent.main_window.get_current_screen_name()
            #print("<CT> Current Screen ", screen_name)

            if (screen_name is None):
                print("<CT> Current Screen is None")
                time.sleep(1)
                continue

            elif (screen_name == "Login"):
                print("<CT> You are in Login Screen")
                
            # 1. if the current screen is in Connection Screen
            elif (screen_name == "Connect") or (network_error and screen_name != "Connect"):
                print("<CT> You are in Connect screen")
                if reachable:
                    # Enable connection button
                    print("<CT> Enable Connection button")
                    self.parent.connect_button_control_signal.emit(True)                               
                else:
                    # Disable Connection button and disconnect the deivce
                    print("<CT> Disable Connection button", self.parent.shared_state.get_device_connection_state())
                    self.parent.connect_button_control_signal.emit(False)

                    if self.parent.shared_state.get_device_connection_state():

                        """
                           Calibration : Ethernet Disconnect
                        """
                        print("Cleanup signal emitted 1",screen_name)    
                        if screen_name == "Sensor_Calibration":
                            if self.parent.shared_state.get_Sensor_calibration_measurment_goingon():
                                self.parent.shared_state.set_Sensor_calibration_Exit_Flag(True)
                                while not self.parent.shared_state.get_Sensor_calibration_measurment_completed():
                                    print("Waiting for Sensor Calibration measurement to complete")
                                    time.sleep(1)
                                                       
                        """
                            Calibration : Ethernet Disconnect Ends
                        """
                        print("<CT> Disconnection happened on the connection page.")
                        self.parent.shared_state.set_failed_status("Network Error")
                        self.parent.shared_state.set_Disconnect_Cubedevice(True)
                        
                        integrate.set_stopmeasurement1(True) 
                        print("<CT> UI Disconnected")
                        tnow = datetime.now()
                        timeout = timedelta(seconds=20)
                        while not integrate.get_exit_measurement_thread1():
                            if (datetime.now() - tnow) > timeout:
                                break
                            time.sleep(0.25)
                        self.parent.disconnect_device1()    
                        self.parent.shared_state.set_device_connection_state(False)
                        integrate.set_stopmeasurement1(False)
                        #integrate.set_exit_measurement_thread1(False)
                        # Deactivate temperature monitoring
                        integrate.set_temperature_monitor_active(False) 
                        self.parent.shared_state.device1_ip = None     
                        
                        if Brick_ID:
                            # Update the Status in connection screen
                            self.parent.bricklets_status_signal.emit(f"Missing Bricklet ID {Brick_ID}")                                 
                        
                        # cleanuup  
                        Sensor_calibrarion = self.parent.main_window.screens["Sensor_Calibration"]
                        Sensor_calibrarion.cleanup_signal.emit()                                 

                        if (screen_name != "Connect"):
                            if not integrate.get_screen_switching():
                                integrate.set_screen_switching(True)
                            self.parent.Screen_Switch_signal.emit() # Switching back to connect screen
                            #  Preventing Screen switch
                            while True:
                                if not integrate.get_screen_switching():
                                    break
                                print("<CT> Connect Screen switching in progress", integrate.get_screen_switching())
                                time.sleep(0.5)
                                
                            self.parent.shared_state.set_Sensor_calibration_measurment_goingon(False)
                            self.parent.shared_state.set_Sensor_calibration_Exit_Flag(False)
                            self.parent.shared_state.set_Sensor_calibration_measurment_completed(False)
                            print("Cleanup signal emitted 2")
                            Sensor_calibrarion = self.parent.main_window.screens["Sensor_Calibration"]
                            Sensor_calibrarion.cleanup_signal.emit()                                 
                                                                
            # 2. else if the current screen is not in Measurement Screen
            elif (screen_name != "Measure") and (screen_name != "AdvanceSettings"):
                print("<CT> You are not in Measurement / AdvanceSettings Screen")
                if not reachable:
                    # Goto connection page and disconnect thrift interface
                    print("Not a measurement / AdvanceSettings Screen")
                    if self.parent.shared_state.get_device_connection_state():
                        self.parent.shared_state.set_failed_status("Network Error")
                        print("Cleanup signal emitted 1",screen_name)    
                        if screen_name == "Sensor_Calibration":
                            if self.parent.shared_state.get_Sensor_calibration_measurment_goingon():
                                self.parent.shared_state.set_Sensor_calibration_Exit_Flag(True)
                                while not self.parent.shared_state.get_Sensor_calibration_measurment_completed():
                                    print("Waiting for Sensor Calibration measurement to complete")
                                    time.sleep(1)
                                
                                self.parent.shared_state.set_Sensor_calibration_measurment_goingon(False)
                                self.parent.shared_state.set_Sensor_calibration_Exit_Flag(False)
                                self.parent.shared_state.set_Sensor_calibration_measurment_completed(False)
                                print("Cleanup signal emitted 2")
                                Sensor_calibrarion = self.parent.main_window.screens["Sensor_Calibration"]
                                Sensor_calibrarion.cleanup_signal.emit()  
                                                        
                        if not integrate.get_screen_switching():
                            integrate.set_screen_switching(True)
                            
                        self.parent.Connection_Screen_Switch_signal.emit()   
                        
                        time.sleep(1) 
                           
                        #  Preventing Screen switch
                        while True:
                            if not integrate.get_screen_switching():
                                break;
                            print("<CT> Not M/Adv Screen switching in progress", integrate.get_screen_switching())
                            time.sleep(0.5)
                        
               
            
            # 3. if the current screen is in Measurement Screen
            elif (screen_name == "Measure") or (screen_name == "AdvanceSettings"):
                #print("<CT> You are in Measurement / AdvanceSettings Screen")
                if not reachable:
                    # Set disconnect flag to true
                    # Wait for all the thread to come to one state
                    # Disconnect the thrift interface
                    # Goto connection page
                    
                    # If it is not in measurement (It is AdvanceSettings Screen without Measurement)
                    if self.parent.shared_state.get_measurement_failed():
                        self.parent.connect_button_control_signal.emit(False)
                        # Switch to the connection screen
                        print("<CT> Measurement / AdvanceSetting Screen-Switching to Connection Screen due to Meaurment Failed or no connection")
                        integrate.set_stopmeasurement1(True)
                        self.parent.shared_state.set_Disconnect_Cubedevice(True)
                        integrate.set_Cancel_Measurement(True)
                        
                        tnow = datetime.now()
                        timeout = timedelta(seconds=20)
                        #ZNKB 
                        # Waiting to set all the thread in proper state
                        tnow = datetime.now()
                        while True:
                            time.sleep(0.5)
                                
                            # Checking all the closed thread flags
                            if integrate.get_MeasurmentThread_Closed()  and integrate.get_BrickletThread_Closed() and integrate.get_TemperaturThread_Closed() and integrate.get_TimerThread_Closed() :
                                print("<CT> All the threads came to Initial State")
                                break
                                
                            # Waiting 15 Seconds to break the while
                            if datetime.now() > tnow + timedelta(seconds=15):    
                                print("<CT> Ping Timeout waiting for threads to close.")
                                break
                                
                        tnow = datetime.now()
                        timeout = timedelta(seconds=20)
                        while not integrate.get_exit_measurement_thread1():
                            print(f"<CT> Disconnect Exit Thread loop : {integrate.get_exit_measurement_thread1()}")
                            if (datetime.now() - tnow) > timeout:
                                print(f"<CT> Failed Disconnect Exit Thread loop : {integrate.get_exit_measurement_thread1()}")
                                break
                            time.sleep(0.25)                        

                        self.parent.shared_state.set_measurement_failed(False)
                        integrate.set_Cancel_Measurement(False)
                        integrate.set_stopmeasurement1(False)
                        if not integrate.get_screen_switching():
                            integrate.set_screen_switching(True)
                        self.parent.Screen_Switch_signal.emit() # Switching back to connect screen                        
                        #  Preventing Screen switch
                        while True:
                            if not integrate.get_screen_switching():
                                break;
                            print("1. <CT> M/Adv Screen switching in progress", integrate.get_screen_switching())
                            time.sleep(0.5)
                        self.parent.connect_button_control_signal.emit(False)
                        integrate._disconnect_lock.release()
                        integrate.set_Cancel_Measurement(False)
                        integrate.set_MeasurmentThread_Closed(False)
                        integrate.set_BrickletThread_Closed(False)
                        integrate.set_TemperaturThread_Closed(False)
                        integrate.set_TimerThread_Closed(False)
                        self.parent.shared_state.set_Cancel_event(False)
                        self.parent.shared_state.set_measurement_failed(False)  
                        #integrate.set_temperature_monitor_active(False) 
                        self.parent.shared_state.set_continue_measurement(False)
                        continue
                    
                    print("<CT> Initiate Disconnect")
                    #self.parent.shared_state.set_Disconnect_Cubedevice(True)
                    #self.parent.shared_state.set_device_connection_state(False)
                    #integrate.set_stopmeasurement1(True) 
                    
                    if self.parent.shared_state.get_Cancel_event() or (integrate.get_enable_bricklet() and not integrate.get_check_temperature()):
                        integrate.set_Cancel_Measurement(True)
                    else:
                        integrate.set_stopmeasurement1(True) 
 
                     # Waiting to set all the thread in proper state
                    tnow = datetime.now()
                    while True:
                        time.sleep(0.5)
                            
                        # Checking all the closed thread flags
                        if integrate.get_MeasurmentThread_Closed()  and integrate.get_BrickletThread_Closed() and integrate.get_TemperaturThread_Closed() and integrate.get_TimerThread_Closed() :
                            print("<CT> All the threads came to Initial State")
                            break
                            
                        # Waiting 15 Seconds to break the while
                        if datetime.now() > tnow + timedelta(seconds=15):    
                            print("<CT> Ping Timeout waiting for threads to close.")
                            break
                                                        

                    #if not integrate.get_check_temperature() and integrate.get_enable_bricklet():
                    #    print("<CT> Stay in the same screen Check Temperature")   
                    #    self.update_cancel_button.emit(False)
                    if self.parent.shared_state.get_Cancel_event():
                        print("<CT> Switch to Setting Screen")
                        # Show the setting Screen
                        self.parent.shared_state.set_Cancel_event(False)

                        # If there is no ethernet connection
                        netisup = self.parent.shared_state.is_eth_up()
                        if not netisup:
                            integrate.set_exit_measurement_thread1(True)
                            self.parent.shared_state.set_failed_status("Network Failure")                            
                            if not integrate.get_screen_switching():
                                integrate.set_screen_switching(True)                            
                            # Switch to the connection screen
                            self.parent.Screen_Switch_signal.emit() # Switching back to connect screen
                            #  Preventing Screen switch
                            while True:
                                if not integrate.get_screen_switching():
                                    break;
                                print("<CT> 2. M/Adv Screen switching in progress", integrate.get_screen_switching())
                                time.sleep(0.5)
                        else:
                            if not integrate.get_screen_switching():
                                integrate.set_screen_switching(True)
                            self.parent.Switch_to_Settings_Sreen_signal.emit()   
                            #  Preventing Screen switch
                            while True:
                                if not integrate.get_screen_switching():
                                    break;
                                print("<CT> 3. M/Adv Screen switching in progress", integrate.get_screen_switching())
                                time.sleep(0.5)
                    else:                        
                        print("<CT> Switching to Connection Screen due to no connection")   
                        #print the failed status in connection screen 
                        self.parent.shared_state.set_Disconnect_Cubedevice(True)
                        self.parent.update_status_signal.emit("Disconnected","red")
                        if not integrate.get_screen_switching():
                            integrate.set_screen_switching(True)
                        # Switch to the connection screen
                        self.parent.Screen_Switch_signal.emit() # Switching back to connect screen
                        #  Preventing Screen switch
                        while True:
                            if not integrate.get_screen_switching():
                                break;
                            print("<CT> Fail : Screen switching in progress", integrate.get_screen_switching())
                            time.sleep(0.5)
                        
                    # Resetting All the Flags
                    integrate.set_Cancel_Measurement(False)
                    integrate.set_MeasurmentThread_Closed(False)
                    integrate.set_BrickletThread_Closed(False)
                    integrate.set_TemperaturThread_Closed(False)
                    integrate.set_TimerThread_Closed(False)
                    self.parent.shared_state.set_Cancel_event(False)
                    self.parent.shared_state.set_measurement_failed(False)  
                    #integrate.set_temperature_monitor_active(False) 
                    self.parent.shared_state.set_continue_measurement(False)
                  

            # UnLock this
            #print("<CT> Releasing Lock")
            integrate._disconnect_lock.release()
            
            time.sleep(1)
              
        self.finished.emit()

    def ping_ip(self, ip):
        """
        Ping the given IP address to check reachability.

        Args:
            ip (str): IP address to ping

        Returns:
            bool: True if reachable, False otherwise
        """        
        response = ping(ip, timeout=1)
        return response is not None

    def validate_ip(self, ip):
        """
        Validate IP address format using regex.

        This checks basic IPv4 format like xxx.xxx.xxx.xxx

        Args:
            ip (str): IP address string

        Returns:
            bool: True if format is correct, False otherwise
        """        
        ip_regex = r'^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$'
        return re.match(ip_regex, ip) is not None

    def stop(self):
        """
        Stop the connection worker thread.

        This will safely exit the running loop in run() function.

        Returns:
            None
        """        
        self.running = False


class ConnectionScreen(TitleBar):
    
    update_status_signal = pyqtSignal(str, str)
    show_popup_signal = pyqtSignal(str)
    bricklets_status_signal = pyqtSignal(str)
    update_device1_ui_signal = pyqtSignal(bool)  # Signal for Device 1 UI updates
    Screen_Switch_signal = pyqtSignal()
    update_connected_ui_signal = pyqtSignal()
    Connection_Screen_Switch_signal = pyqtSignal()
    Settings_Screen_signal = pyqtSignal()
    Settings_Screen_checkbox_controls_signal = pyqtSignal(bool)
    toggle_sidebar_signal = pyqtSignal(bool)
    connect_button_control_signal = pyqtSignal(bool)
    disconnect_button_control_signal = pyqtSignal(bool)
    logout_button_control_signal = pyqtSignal(bool)
    device1_input_control_signal = pyqtSignal(bool)
    Switch_to_Settings_Sreen_signal = pyqtSignal()
    update_ui_after_disconnect_signal = pyqtSignal()
    Shutdown_signal = pyqtSignal(str,str,str,str)
    Log_signal = pyqtSignal()
    
    def on_text_changed(self, text):
        """
        Handle text change event for device IP input.

        This function will be called whenever the user changes
        the IP address text in the input field.

        It updates the shared state with the entered IP address
        after removing extra spaces and prints it for debugging.

        Args:
            text (str): Entered IP address text

        Returns:
            None
        """        
        self.shared_state.set_device_ip_address(text.strip())
        print("IP Address:", text)

    def __init__(self, main_window, username, shared_state):
        super().__init__(main_window, username, "Device Connection")
        print("Creating ConnectionScreen")
        self.shared_state = shared_state
        self.setFixedHeight(800)
        self.device1_status = QLabel()
        self.bricklets_status = QLabel("MVP Bricklets: Not Initialized")
        self.bricklets_status.setStyleSheet("font-size: 18px; color: red; font-weight: bold; background-color: #242746;")
        self.bricklets_status_signal.connect(self.bricklets_status.setText)

        self.connecting_timer = QTimer()
        self.ip_con = None
        self.init_connection_ui()
        self.update_status_signal.connect(self.update_status)
        self.bricklets_status_signal.connect(self.on_bricklets_status_signal)
        self.show_popup_signal.connect(self.show_continue_popup)
        self.Screen_Switch_signal.connect(self.Connect_Screen_change) # Signal
        self.Connection_Screen_Switch_signal.connect(self.connectionscreen_change_only)
        self.toggle_sidebar_signal.connect(self.Toggling_sidebar)# Signal
        self.Settings_Screen_signal.connect(self.set_default_tablevalue) # Signal
        self.Settings_Screen_checkbox_controls_signal.connect(self.checkbox_controls_on_settingspage) # Signal        
        self.connect_button_control_signal.connect(self.Connect_button_control)# Signal
        self.disconnect_button_control_signal.connect(self.disconnect_button_control)# Signal
        self.logout_button_control_signal.connect(self.logout_button_control)# Signal        
        self.device1_input_control_signal.connect(self.device1_input_control)# Signal 
        self.update_connected_ui_signal.connect(self.Connected_ui)  # Connect to enable UI signal      
        self.update_device1_ui_signal.connect(self.update_device1_ui)  # Connect Device 1 UI signal
        self.Switch_to_Settings_Sreen_signal.connect(self.switch_to_settings_screen)  
        self.update_ui_after_disconnect_signal.connect(self.update_ui_after_disconnect)
        self.Shutdown_signal.connect(self.Shutdown_device)
        self.Log_signal.connect(self.fetch_log_device1)
        self.connection_in_progress = False
        self.shutdown_Flag = False
        self.device1_connecting = False
        self.init_dots = 0
        self.connecting_dots = 0
        self.continue_popup_status = False
        self.continue_popup_selected = QMessageBox.No
        self.get_device_status1 = False

        # Initialize worker but don't start it
        self.worker = Worker()
        self.worker_thread = QThread()
        self.worker.moveToThread(self.worker_thread)
        self.worker_thread.started.connect(self.worker.run)
        self.worker.finished.connect(self.worker_thread.quit)
        self.worker.update_status.connect(self.update_device1_status)
        
        self.connection_thread = QThread()
        self.connection_worker = ConnectionWorker(self)
        self.connection_worker.moveToThread(self.connection_thread)
        self.connection_thread.started.connect(self.connection_worker.run)
        self.connection_worker.finished.connect(self.connection_thread.quit)
        self.connection_worker.finished.connect(self.connection_worker.deleteLater)
        self.connection_thread.finished.connect(self.connection_thread.deleteLater) 
        self.connection_thread.start()    

       # self.connection_worker.update_cancel_button.connect(
        #    main_window.screens["Measure"].terminal_dialog.cancel_button.setEnabled)        
 

    @pyqtSlot()
    def connectionscreen_change_only(self):
        """
        Handle screen switch when other screens are not reachable.

        This function checks whether screen switching is allowed.
        If allowed, it brings back the connection screen and updates
        UI based on current state.

        It also checks bricklet connection status and shows error
        message if any bricklet is missing.

        Returns:
            None
        """        
        if not integrate.get_screen_switching():
            return        
        
        print("Connect Screen Change signal due to otherscreens not ping")
        self.main_window.show_normal_sidebar() 
        # only For Display DI on sample screen
        SampleScreen = self.main_window.screens["Sample"]        
        if SampleScreen.display_di_flag:   
            SampleScreen.handle_display_di_i()  

        Screen_name = self.main_window.get_current_screen_name()
        if Screen_name == "Settings":
            rc, Brick_ID = get_bricklet_connection_status()
            if rc:
                print("<CT> Some of the Bricklet Not connected Canceling the connection")
                Settings_Screen = self.main_window.screens["Settings"]  
                Settings_Screen.backflush_status_label.setText(f"Status : Missing Bricklet ID is {Brick_ID}")
        else:
            ConnectionScreen = self.main_window.screens["Connect"]   
            ConnectionScreen.main_window.change_screen("Connect") 
        
        integrate.set_screen_switching(False)   # allow  connection thread again


    @pyqtSlot()
    def Connect_Screen_change(self):
        """
        Handle switching to connection screen when device is not reachable.

        This function will disconnect the device, reset measurement flags,
        stop temperature monitoring and enable required UI buttons.

        Finally, it switches the UI to the connection screen.

        Returns:
            None
        """        
        if not integrate.get_screen_switching():
            return
        
        print("Connect Screen Change signal due to not reachable not in measurment screen")

        self.disconnect_device1()    
        integrate.set_stopmeasurement1(False)
        #integrate.set_exit_measurement_thread1(False)
        # Deactivate temperature monitoring
        integrate.set_temperature_monitor_active(False) 
        self.shared_state.device1_ip = None
            
        #QMetaObject.invokeMethod(self, "update_ui_after_disconnect", Qt.QueuedConnection)         

        self.main_window.show_normal_sidebar() 
        # Enable all the required buttons
        self.logout_button.setEnabled(True)
        self.toggle_sidebar_signal.emit(True)
        self.main_window.set_sweep_button.setEnabled(True)
        self.main_window.sweep_button.setEnabled(True)
        self.main_window.measure_button.setEnabled(True)
        self.main_window.set_measure_button.setEnabled(True)
        self.main_window.display_di_i_button.setEnabled(True)
        # Fix E712
        if self.shared_state.Enable_Bricklets:
            self.main_window.enable_bricklet_check.setEnabled(True)
        self.main_window.presweep_check.setEnabled(True)     
        time.sleep(1)        
        ConnectionScreen = self.main_window.screens["Connect"]   
        ConnectionScreen.main_window.change_screen("Connect") 
        
        integrate.set_screen_switching(False)   # allow  connection thread again
          
    
    @pyqtSlot(bool)
    def Toggling_sidebar(self, Flag):
        """
        Enable or disable the sidebar.

        This function will toggle the sidebar visibility based on
        the given flag value.

        Args:
            Flag (bool): True to enable sidebar, False to disable

        Returns:
            None
        """        
        self.main_window.toggle_sidebar(Flag)
        
    @pyqtSlot()
    def set_default_tablevalue(self):
        """
        Set default values in settings table.

        This function calls the settings screen method to reset
        and apply default values in the table.

        Returns:
            None
        """        
        settings_screen = self.main_window.screens["Settings"]
        settings_screen.on_set_button_clicked()        

    @pyqtSlot(bool)
    def checkbox_controls_on_settingspage(self,Flag):
        """
        Enable or disable checkbox controls in settings screen.

        This function updates all checkboxes in the settings page
        based on the given flag.

        Args:
            Flag (bool): True to enable, False to disable

        Returns:
            None
        """        
        settings_screen = self.main_window.screens["Settings"]
        settings_screen.reset_checkboxes(Flag)          
        
    @pyqtSlot(bool)
    def Connect_button_control(self, Flag):
        """
        Control the connect button based on device status.

        This function enables or disables the connect button
        depending on whether the device is reachable and connection
        is already in progress or not.

        It also updates UI elements like labels and other buttons.

        Args:
            Flag (bool): True if device reachable, False otherwise

        Returns:
            None
        """        
        # Device reachable
        print("IP",self.shared_state.get_device_ip_address())
        if self.shutdown_Flag:
            Flag = False
        if Flag:
            print("IP2:",self.shared_state.get_device_ip_address())
            device_connected = self.shared_state.get_device_connection_state()
            no_connection_progress = not self.connection_in_progress
            disconnect_disabled = not self.disconnect_button.isEnabled()
            self.Ping_label.setText("Reachable")
            self.Ping_label.setStyleSheet("font-size: 18px; color: green; font-weight: bold; background-color: #242746;")
            print("No Connection and disconnect_disabled", no_connection_progress, disconnect_disabled)
            # --- Enable connect button ---
            if no_connection_progress and disconnect_disabled:
                if not self.shared_state.get_PowerupBricklets_onetime_initialization() or not device_connected:
                    self.connect_button.setEnabled(True)
                    self.device1_input_control_signal.emit(True)
                    self.Reboot1_button.setEnabled(True)
                    self.log_button.setEnabled(True)
            elif device_connected:
                self.Reboot1_button.setEnabled(True)
                self.log_button.setEnabled(True)
                self.device1_input_control_signal.emit(False)

        # Device unreachable → disable everything
        else:
            print("ip not reached")
            self.connect_button.setEnabled(False)            
            self.Reboot1_button.setEnabled(False)
            self.log_button.setEnabled(False)
            self.disconnect_button.setEnabled(False)
            self.Ping_label.setText("Not Reachable")
            self.Ping_label.setStyleSheet("font-size: 18px; color: red; font-weight: bold; background-color: #242746;")

            

    @pyqtSlot(bool)
    def disconnect_button_control(self, Flag):

        """
        Control the disconnect button state.

        This function enables or disables the disconnect button
        based on the given flag.

        Args:
            Flag (bool): True to enable, False to disable

        Returns:
            None
        """       
        # Do NOT enable disconnect button if device is unreachable
            #if not self.shared_state.get_device_connection_state():
            #	Flag = False

        self.disconnect_button.setEnabled(Flag)
       
 
    @pyqtSlot(bool)
    def logout_button_control(self, Flag):
        self.logout_button.setEnabled(Flag) 

    @pyqtSlot(bool)
    def device1_input_control(self, Flag):
        self.device1_input.setEnabled(Flag) 
               
    def switch_to_settings_screen(self):
        if not integrate.get_screen_switching():
            return
        #if integrate.get_mainmeasurement():
        self.logout_button.setEnabled(True)
        self.main_window.toggle_sidebar(True)
        self.main_window.change_screen("Settings")
        self.main_window.sidebar_stack.setCurrentIndex(0)
        integrate.set_screen_switching(False)   # allow  connection thread again
  

    @pyqtSlot()
    def Connected_ui(self):
        self.device1_status.setText("Device 1 : Connected")
        self.device1_status.setStyleSheet("font-size: 18px; color: green; font-weight: bold; background-color: #242746;")
               
    def init_connection_ui(self):
        # --------------------- Device 1 IP --------------------------
        self.device1_label = QLabel("Device 1 IP")
        self.device1_label.setStyleSheet("font-size: 18px; color: white; font-weight: bold; background-color: #242746;")
        self.device1_label.setFixedSize(120, 50)

        mvp_version = self.shared_state.get_MVP_Device_Version().rsplit("_", 1)[-1]
        if mvp_version == "1.0":
            self.device1_input = QLineEdit("192.168.150.16")
        elif mvp_version == "1.5":
            self.device1_input = QLineEdit("192.168.150.17")

        self.device1_input.setPlaceholderText("Enter Device 1 IP")
        self.device1_input.setFixedSize(300, 50)
        self.device1_input.textChanged.connect(self.on_text_changed)
        self.apply_input_style(self.device1_input)
        self.Ping_label = QLabel("Not Reachable")
        self.Ping_label.setStyleSheet("font-size: 18px; color: red; font-weight: bold; background-color: #242746;")
        self.Ping_label.setFixedSize(160, 50)
        
        #Update the IP address
        self.shared_state.set_device_ip_address(self.device1_input.text().strip())
        
        # --------------------- Status Labels --------------------------
        self.status_label = QLabel("Status:")
        self.status_label.setStyleSheet("font-size: 18px; color: white; font-weight: bold; background-color: #242746;")
        self.device1_status.setText("Device 1 : Not Connected")
        self.device1_status.setStyleSheet("font-size: 18px; color: red; font-weight: bold; background-color: #242746;")
        self.bricklets_status.setStyleSheet("font-size: 18px; color: red; font-weight: bold; background-color: #242746;")

        for w in [self.status_label, self.device1_status, self.bricklets_status]:
            w.setFixedHeight(30)

        # --------------------- Buttons --------------------------
        self.connect_button = QPushButton("Connect")
        self.connect_button.setStyleSheet("""
            QPushButton { background-color: #66ccff; color: black; font-size: 16px; border-radius: 10px; }
            QPushButton:hover { background-color: #66ccff; }
            QPushButton:disabled { background-color: gray; }
        """)
        self.connect_button.setFixedSize(200, 50)
        self.connect_button.clicked.connect(self.connect_devices)

        self.disconnect_button = QPushButton("Disconnect")
        self.disconnect_button.setStyleSheet("""
            QPushButton { background-color: red; color: white; font-size: 16px; border-radius: 10px; }
            QPushButton:hover { background-color: #FF4500; }
            QPushButton:disabled { background-color: gray; }
        """)
        self.disconnect_button.setFixedSize(200, 50)
        self.disconnect_button.clicked.connect(self.disconnect_devices)
        self.disconnect_button.setEnabled(False)

        self.log_button = QPushButton("Log")
        self.log_button.setStyleSheet("""
            QPushButton { background-color: #66ccff; color: black; font-size: 16px; border-radius: 10px; }
            QPushButton:hover { background-color: #66ccff; }
            QPushButton:disabled { background-color: gray; }
        """)
        self.log_button.setFixedSize(200, 50)
        self.log_button.clicked.connect(self.Device_log_fetch)

        button_layout = QHBoxLayout()
        button_layout.setSpacing(20)
        button_layout.setContentsMargins(0, 0, 0, 0)
        button_layout.addWidget(self.connect_button)
        button_layout.addWidget(self.disconnect_button)
        button_layout.addWidget(self.log_button)

        # ------------------ Reboot buttons ------------------
        self.Reboot1_button = QPushButton("Shutdown")
        self.Reboot1_button.setStyleSheet("""
            QPushButton { background-color: #66ccff; color: black; font-size: 16px; border-radius: 10px; }
            QPushButton:hover { background-color: #66ccff; }
            QPushButton:disabled { background-color: gray; }
        """)
        self.Reboot1_button.setFixedSize(150, 30)
        self.Reboot1_button.clicked.connect(
            lambda: threading.Thread(
                target=self.Device_Shutdown,
                args=(self.device1_input.text(), "root", "voc@123","1"),
                daemon=True
            ).start()
        )        

        # ------------------ Grid 1 ------------------
        grid1 = QGridLayout()
        grid1.setHorizontalSpacing(0)
        grid1.setVerticalSpacing(0)
        grid1.setContentsMargins(0, 0, 0, 0)

        grid1.addWidget(self.device1_label, 0, 0, Qt.AlignLeft)
        grid1.addWidget(self.device1_input, 0, 1, Qt.AlignLeft)
        grid1.addWidget(self.Ping_label, 0, 2, Qt.AlignLeft)   
        grid1.setHorizontalSpacing(15)     
        grid1.setColumnStretch(0, 0)
        grid1.setColumnStretch(1, 1)
        grid1.setColumnMinimumWidth(0, self.device1_label.sizeHint().width())
        grid1.setColumnStretch(2, 2)
        grid1.setColumnMinimumWidth(1, self.Ping_label.sizeHint().width())
        

        container1 = QWidget()
        container1.setLayout(grid1)
        container1.setContentsMargins(0, 0, 0, 0)
        container1.layout().setContentsMargins(0, 0, 0, 0)

        # ------------------ Grid 2 ------------------
        grid2 = QGridLayout()
        grid2.setContentsMargins(0, 0, 0, 0)
        grid2.setVerticalSpacing(5)

        grid2.addWidget(self.status_label, 0, 1, 1, 2, Qt.AlignLeft)
        grid2.addWidget(self.device1_status, 1, 1, 1, 2, Qt.AlignLeft)
        grid2.addWidget(self.Reboot1_button, 1, 1, 1, 2, Qt.AlignRight)        
        grid2.addWidget(self.bricklets_status, 3, 1, 1, 2, Qt.AlignLeft)
        
        self.enable_bricklets_checkbox = QCheckBox("Enable Bricklets")
        self.enable_bricklets_checkbox.setChecked(True)
        self.enable_bricklets_checkbox.setEnabled(True)
        self.enable_bricklets_checkbox.setStyleSheet("""
            QCheckBox {
                font-size: 18px;
                color: white;
                font-weight: bold;
                background-color: #242746;
                padding: 5px;
            }
        """)
        
        self.enable_bricklets_checkbox.stateChanged.connect(self.on_enable_bricklets_changed)

        grid2.addWidget(self.enable_bricklets_checkbox, 4, 1, 1, 2, Qt.AlignLeft)

        container2 = QWidget()
        container2.setLayout(grid2)
        container2.setContentsMargins(0, 0, 0, 0)
        container2.layout().setContentsMargins(0, 0, 0, 0)

        # ------------------ Grid 3 ------------------
        grid3 = QGridLayout()
        grid3.setContentsMargins(0, 0, 0, 0)
        grid3.setSpacing(0)
        grid3.addLayout(button_layout, 0, 0, 1, 2, Qt.AlignLeft)

        container3 = QWidget()
        container3.setLayout(grid3)
        container3.setContentsMargins(0, 0, 0, 0)
        container3.layout().setContentsMargins(0, 0, 0, 0)

        # ------------------ Final Layout ------------------
        final_layout = QVBoxLayout()
        final_layout.setSpacing(5)
        final_layout.setContentsMargins(0, 0, 0, 0)
        final_layout.addWidget(container1)
        final_layout.addWidget(container2)
        final_layout.addWidget(container3)

        main_container = QWidget()
        main_container.setLayout(final_layout)
        main_container.setFixedSize(1000, 700)

        central_widget = QWidget()
        central_layout = QVBoxLayout(central_widget)
        central_layout.setContentsMargins(0, 0, 0, 0)
        central_layout.setSpacing(0)
        central_layout.addStretch()
        central_layout.addWidget(main_container, alignment=Qt.AlignCenter)
        central_layout.addStretch()

        self.layout().addWidget(central_widget)

    def on_enable_bricklets_changed(self, state):
        if self.enable_bricklets_checkbox.isChecked():
            print("Enable Bricklets: Checked")
            self.shared_state.Enable_Bricklets = True
            integrate.set_enable_bricklet(True)
        else:
            print("Enable Bricklets: Unchecked")
            self.shared_state.Enable_Bricklets = False  
            integrate.set_enable_bricklet(False)

    def Device_Shutdown(self, ip, username, password, Deviceno):
        # Immediately disable all control buttons to prevent user interaction
        self.shutdown_Flag = True
        self.connect_button.setEnabled(False)
        self.disconnect_button.setEnabled(False)
        self.log_button.setEnabled(False)
        self.Reboot1_button.setEnabled(False)
        self.device1_input.setEnabled(False)
        self.enable_bricklets_checkbox.setEnabled(False)
        self.Ping_label.setText("Not Reachable")
        self.Ping_label.setStyleSheet("font-size: 18px; color: red; font-weight: bold; background-color: #242746;")
        QApplication.processEvents()
        time.sleep(2)
        # Update status with wide visible message
        self.device1_status.setText("Device 1 : Shutting Down...")
        self.device1_status.setStyleSheet("font-size: 18px; color: yellow; font-weight: bold; background-color: #242746;")
        QApplication.processEvents()
        time.sleep(2)
        # Emit signal to perform actual shutdown
        self.Shutdown_signal.emit(ip, username, password, Deviceno)
    
    @pyqtSlot(str, str, str, str)
    def Shutdown_device(self, ip, username, password, Deviceno):
        print(f"[INFO] Initiating shutdown for Device {Deviceno} at {ip}...")
        QApplication.processEvents()

        self.connect_button_control_signal.emit(False)
        if self.shared_state.get_device_connection_state():
            print("device1")
            integrate.set_Cancel_Measurement(True)
            while True:
                time.sleep(0.1)
                if integrate.get_BrickletThread_Closed() and integrate.get_TemperaturThread_Closed() and integrate.get_TimerThread_Closed():
                    print("All the threads are reinitialized properly")
                    break
            integrate.set_Cancel_Measurement(False)
            integrate.set_BrickletThread_Closed(False)
            integrate.set_TemperaturThread_Closed(False)
            integrate.set_TimerThread_Closed(False)

            if self.shared_state.get_device_connection_state():
                #integrate.startmeasurement1_event.set()
                integrate.set_stopmeasurement1(True)
                #integrate.startmeasurement1_event.set()
                while not integrate.get_exit_measurement_thread1():
                    time.sleep(0.25)
                integrate.set_stopmeasurement1(False)
                self.shared_state.device1_ip = None
                self.disconnect_device1()
                integrate.set_temperature_monitor_active(False)
                print("Cube device disconnected successfully")

                self.update_ui_after_disconnect_signal.emit()

        ssh = paramiko.SSHClient()
        ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        try:
            ssh.connect(ip, username=username, password=password, timeout=5)
            print(f"[INFO] Connected to Device {Deviceno} successfully.")
            print(f"[INFO] Sending 'shutdown -h now' command to Device {Deviceno}...")
            channel = ssh.invoke_shell()
            time.sleep(1)
            channel.recv(1024)
            channel.send("shutdown -h now\n")
            time.sleep(2)
            ssh.close()
            time.sleep(8)
            print(f"[INFO] Shutdown command executed successfully for Device {Deviceno}.")
            self.device1_status.setText("Device 1 : Shutdown has been completed successfully!")
            self.device1_status.setStyleSheet("font-size: 18px; color: green; font-weight: bold; background-color: #242746;")
            self.shutdown_Flag = False
            QApplication.processEvents()
            #ssh.close()
        except Exception as e:
            print(f"[ERROR] Failed to shutdown Device {Deviceno}: {e}")
            self.device1_status.setText("Device 1 : Failed to shutdown")
            self.device1_status.setStyleSheet("font-size: 18px; color: red; font-weight: bold; background-color: #242746;")
            QApplication.processEvents()
        finally:
            if Deviceno == '1':
                # Re-enable safe controls only if device is disconnected
                if not self.shared_state.get_device_connection_state():
                    self.connect_button.setEnabled(False)
                    self.device1_input.setEnabled(False)
                    self.enable_bricklets_checkbox.setEnabled(False)
                    self.log_button.setEnabled(False)
                # Do NOT re-enable Shutdown button until device is reachable again
                # self.Reboot1_button.setEnabled(True)  # Keep disabled

            self.update_ui_after_disconnect_signal.emit()


    def enable_ui(self):
        self.device1_input_control_signal.emit(True)
        self.connect_button_control_signal.emit(True)
        self.toggle_sidebar_signal.emit(True)

    def disable_ui(self):
        self.device1_input_control_signal.emit(False)
        self.connect_button_control_signal.emit(False)
        self.toggle_sidebar_signal.emit(False)

    def update_device1_status(self, text):
        self.device1_status.setText(f"{text}")
        self.device1_status.setStyleSheet("font-size: 18px; color: yellow; font-weight: bold; background-color: #242746;")

    @pyqtSlot(bool)
    def update_device1_ui(self, connected):
        print(f"*** UI UPDATE: connected={connected}, current_text='{self.device1_status.text()}'")
        #self.shared_state.connection_status["Device 1"] = connected
        self.shared_state.set_device_connection_state(connected)

        self.connection_in_progress = False
        if connected:
            self.device1_status.setText("Device 1 : Connected")
            self.device1_status.setStyleSheet("font-size: 18px; color: green; font-weight: bold; background-color: #242746;")
            self.enable_bricklets_checkbox.setEnabled(False)
            self.logout_button.setEnabled(True)
            self.log_button.setEnabled(False)
        else:     
            print(" Get Device Status and Device connection State ", self.get_device_status1,self.shared_state.get_device_connection_state())
            if not self.get_device_status1:
                if self.shared_state.get_failed_status() == "":
                    self.shared_state.set_failed_status("Device 1 : Not connected the thrift interface.")
                self.device1_status.setText(self.shared_state.get_failed_status())
                self.device1_status.setStyleSheet("font-size: 18px; color: red; font-weight: bold; background-color: #242746;")
            elif "No route to host" in self.device1_status.text():
                self.log_button.setEnabled(True)  
                self.device1_status.setText("Device 1 : No route to host (Not able to communicate)")
            #else:
            #    self.device1_status.setText("Device 1 : Not connected the thrift interface.") 
            self.device1_status.setStyleSheet("font-size: 18px; color: red; font-weight: bold; background-color: #242746;")
            self.enable_bricklets_checkbox.setEnabled(True)
            self.toggle_sidebar_signal.emit(True)
            self.logout_button.setEnabled(True)
            
    def Device_log_fetch(self):
        self.Log_signal.emit()

    @pyqtSlot()
    def fetch_log_device1(self):
        self.log_button.setEnabled(False)  
        remote_user = "root"
        remote_pass = "voc@123"
        remote_host = self.device1_input.text().strip()
        remote_dir = "/media/media/log"
        local_log_dir = "./log1"
        # Get latest log file name
        cmd_get_latest = f"sshpass -p {remote_pass} ssh {remote_user}@{remote_host} 'cd {remote_dir} && ls -t wave_log-*.log | head -n 1'"
        try:
            self.device1_status.setText("Device 1 : Fetching Device log...")
            self.device1_status.setStyleSheet("font-size: 18px; color: yellow; font-weight: bold; background-color: #242746;")
            self.log_button.setEnabled(False)
            QApplication.processEvents()            
            
            latest_file = subprocess.check_output(cmd_get_latest, shell=True, stderr=subprocess.STDOUT, timeout=8).decode().strip()
            print(f"Latest log file for device 1: {latest_file}")
            
            # Prepare local log directory
            if os.path.exists(local_log_dir):
                shutil.rmtree(local_log_dir)
            os.makedirs(local_log_dir)

            # Copy the latest log file
            cmd_scp = f"sshpass -p {remote_pass} scp -O {remote_user}@{remote_host}:{remote_dir}/{latest_file} {local_log_dir}/"
            print("cmd_scp",cmd_scp)
            subprocess.run(cmd_scp, shell=True, check=True, timeout=8)

            print(f"Copied latest log for device 1: {latest_file}")
            
            time.sleep(1)
            
            self.device1_status.setText("Device 1 : Log fetched successfully")
            self.device1_status.setStyleSheet("font-size: 18px; color: green; font-weight: bold; background-color: #242746;")
            self.log_button.setEnabled(True)
            QApplication.processEvents()

            
            time.sleep(5)
            if self.shared_state.get_device_connection_state():
                self.device1_status.setText("Device 1 : Connected")
                self.device1_status.setStyleSheet("font-size: 18px; color: green; font-weight: bold; background-color: #242746;")
                QApplication.processEvents()
            else:
                self.device1_status.setText("Device 1 : Not connected the thrift interface.") 
                self.device1_status.setStyleSheet("font-size: 18px; color: red; font-weight: bold; background-color: #242746;")        
                QApplication.processEvents()

        except subprocess.CalledProcessError as e:
            if e.returncode == 255 and "No route to host" in str(e):
                self.device1_status.setText("Device 1 : No route to host. Cannot fetch log.")
                self.device1_status.setStyleSheet("font-size: 18px; color: red; font-weight: bold; background-color: #242746;")
                print("SSH error for Device 1: No route to host")
                self.log_button.setEnabled(True)  
                QApplication.processEvents()
            else:
                self.device1_status.setText("Device 1 : Failed to retrieve log file.")
                self.device1_status.setStyleSheet("font-size: 18px; color: red; font-weight: bold; background-color: #242746;")
                print(f"Error retrieving log for Device 1: {e}")
                self.log_button.setEnabled(True)  
                QApplication.processEvents()
        except Exception as e:
            self.device1_status.setText(f"Device 1 : Unexpected error: {e}")
            self.device1_status.setStyleSheet("font-size: 18px; color: red; font-weight: bold; background-color: #242746;")
            print(f"Unexpected error for Device 1: {e}")
            self.log_button.setEnabled(True)  
                        
    def stop_connecting_animation(self):
        """Safely stop the connecting animation"""
        if self.worker_thread.isRunning():
            self.worker.stop()
            self.worker_thread.quit()
            self.worker_thread.wait(1000)
            print("Connecting animation stopped")

    def connect_devices(self):
        self.disable_ui()
        self.connection_in_progress = True
        self.device1_connecting = False

        device1_ip = self.device1_input.text().strip()

        if not device1_ip:
            self.device1_input.setPlaceholderText("IP is required!")
            self.connection_in_progress = False
            self.device1_input.setText("")
            self.enable_ui()
            return
        if not self.validate_ip(device1_ip):
            self.device1_input.setPlaceholderText("Invalid IP format!")
            self.connection_in_progress = False
            self.device1_input.setText("")
            self.enable_ui()
            return
        self.apply_input_style(self.device1_input)

        # Stop worker before starting connection
        if self.worker_thread.isRunning():
            self.stop_connecting_animation()

        self.disable_ui()
        self.connect_button.setEnabled(False)
        self.disconnect_button.setEnabled(False)
        self.enable_bricklets_checkbox.setEnabled(False)

        # Start worker for connecting animation
        self.worker_thread.start()

        self.device1_connecting = True
        threading.Thread(target=self.connect_device1, daemon=True).start()

    def validate_ip(self, ip):
        ip_regex = r'^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$'
        return re.match(ip_regex, ip) is not None

    """
        Zaid NewReq Get the Device unique id
    """
                        
    def get_soc_uid(self, ip_address):
        """
        Fetch soc_uid from remote board and copy it locally.

        On failure:
            Creates local file with content "Unknown"

        Args:
            ip_address (str): Target board IP address

        Returns:
            str: Local file path
        """

        remote_soc_uid = "/sys/devices/soc0/soc_uid"
        remote_tmp_file = "/tmp/soc_uid.txt"
        local_file = os.path.join(os.getcwd(), "soc_uid.txt")

        ssh = None
        connected = False

        try:
            # SSH connection
            ssh = paramiko.SSHClient()
            ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())

            ssh.connect(
                hostname=ip_address,
                port=BOARD_PORT,
                username=BOARD_USER,
                password=BOARD_PASSWORD,
                timeout=10,
                banner_timeout=10,
                auth_timeout=10
            )

            connected = True

            # Step 1: Create temp file on remote
            cmd = f"cat {remote_soc_uid} > {remote_tmp_file}"
            stdin, stdout, stderr = ssh.exec_command(cmd)
            exit_status = stdout.channel.recv_exit_status()

            if exit_status != 0:
                error = stderr.read().decode().strip()
                raise RuntimeError(f"Remote command failed: {error}")

            # Step 2: Copy via SCP
            with SCPClient(ssh.get_transport()) as scp:
                scp.get(remote_tmp_file, local_file)

            return local_file

        except paramiko.AuthenticationException:
            print(f"[ERROR] Authentication failed for {ip_address}")

        except paramiko.SSHException as error:
            print(f"[ERROR] SSH error for {ip_address}: {error}")

        except Exception as error:
            print(f"[ERROR] get_soc_uid failed for {ip_address}: {error}")

        # ✅ On any failure → write "Unknown"
        try:
            with open(local_file, "w", encoding="utf-8") as f:
                f.write("Unknown\n")
        except Exception as file_error:
            print(f"[ERROR] Failed to write fallback file: {file_error}")

        # Cleanup only if connected
        if ssh and connected:
            try:
                ssh.exec_command(f"rm -f {remote_tmp_file}")
            except Exception:
                pass
            ssh.close()

        return local_file

    
    def connect_device1(self):
        if self.shared_state.Enable_CUBEXAPI:
            self.logout_button_control_signal.emit(False)
            self.toggle_sidebar_signal.emit(False)
            print("Device Connect Thread Started")
            try:
                # By Default Set the T5 Values
                self.Settings_Screen_signal.emit()
                device1_ip = self.device1_input.text().strip()
                self.shared_state.device1_ip = device1_ip

                # Connect to the Thrift interface
                dev_con = self.shared_state.cube.Connect(device1_ip)
                print("Cube Connect state", dev_con)
                # Update the device status to the Shared variable
                self.shared_state.set_device_connection_state(dev_con)              

                if dev_con:
                    #integrate.set_temperature_monitor_active(True)
                    # Check device ready status
                    self.get_device_status1 = self.shared_state.cube.GetDeviceStatus(device1_ip)
                    if not self.get_device_status1:
                        print("Device1 Status: Not in Ready State Please chek the log")
                        self.shared_state.set_failed_status("Device1: is not in Ready State")
                        self.shared_state.set_device_connection_state(False)
                        self.shared_state.cube.Disconnect(device1_ip)
                        self.stop_connecting_animation()
                        self.update_device1_ui_signal.emit(False)  
                        self.logout_button_control_signal.emit(True)
                        self.toggle_sidebar_signal.emit(True)
                        time.sleep(1)
                        self.shared_state.set_failed_status("")
                        return False
                    else:

                        """
                        Zaid NewReq Get the Device unique id
                        """
                        self.get_soc_uid(device1_ip)

                        print("Device1 Connected")
                        # Close the Previous Measurement
                        try:
                            self.shared_state.cube.AbortMeasurement(device1_ip)
                        except Exception as e:
                            print(f"Error to Abort Measurement {e}")

                        # Stop connecting animation immediately
                        self.stop_connecting_animation()                       
                        self.update_connected_ui_signal.emit()
                        time.sleep(1)

                        # Activate temperature monitoring
                        if self.shared_state.get_PowerupBricklets_onetime_initialization():
                            integrate.set_temperature_monitor_active(False)

                        bricklet_success = True
                        if self.shared_state.Enable_Bricklets:
                            bricklet_success = self.start_bricklet_initialization()
                            
                            if not bricklet_success:
                                print("Bricklet Initialization Failed during connection")
                                self.update_Bricklets_status("Initialization Failed")
                                if self.shared_state.get_device_connection_state():
                                    print("Diconnecting the device due to bricklet failed")
                                    self.disconnect_device1()
                                self.logout_button_control_signal.emit(True)
                                self.toggle_sidebar_signal.emit(True)
                                return False
                            else:   
                                print("Bricklet Initialization Completed during connection")
                                self.update_Bricklets_status("Initialization completed")
                            

                        self.update_device1_ui_signal.emit(True)  
                        self.connect_button_control_signal.emit(False)
                        self.disconnect_button_control_signal.emit(True)                    
                        self.toggle_sidebar_signal.emit(True)
                        self.logout_button_control_signal.emit(True)
                        self.shared_state.set_device_connection_state(True)
                        self.shared_state.set_Disconnect_Cubedevice(False)  
  
                        self.main_window.screens["Measure"].startAllThreads()                  


                        return True
                else: # Check the connection status of the two devices
                    print("Device1 Disconnected")
                    self.shared_state.set_failed_status("Device1: Not connected to Thrift Interface")
                    self.stop_connecting_animation()
                    time.sleep(0.5)
                    self.update_device1_ui_signal.emit(False)
                    time.sleep(0.5) 
                    self.connect_button_control_signal.emit(True)   
                    self.disconnect_button_control_signal.emit(False)                
                    self.update_device1_ui_signal.emit(False)
                    self.enable_ui()
                    time.sleep(1)
                    self.shared_state.set_failed_status("")
                    return False
            except Exception as e:
                logging.error(f"Error connecting Device 1: {e}")
                self.stop_connecting_animation()
                self.update_status_signal.emit("Device 1", "Not Connected")
                self.update_device1_ui_signal.emit(False)
                # Update the device status to the Shared variable
                self.shared_state.set_device_connection_state(False)  
                return False            

            finally:
                self.device1_connecting = False
        else:
            self.stop_connecting_animation()
            time.sleep(0.5)
            #self.update_status_signal.emit("Device 1", "Connected")
            time.sleep(2)
            # Activate temperature monitoring
            integrate.set_temperature_monitor_active(False)
            print("Temperature monitoring activated")
            self.start_bricklet_initialization()
            return True

    def start_bricklet_initialization(self):
        if self.shared_state.get_PowerupBricklets_onetime_initialization():
            integrate.set_temperature_monitor_active(False)
            self.update_Bricklets_status("Initialization completed")
            return True

        self.toggle_sidebar_signal.emit(False)
        print("T6 Sidebar toggled off")
        self.logout_button.setEnabled(False)
        self.disconnect_button.setEnabled(False)
        self.update_Bricklets_status("Initializing bricklets please wait")
        
        Bricklet_Initialization = self.initialize_bricklets()
        if not Bricklet_Initialization:
            print("Bricklet Initialization Failed")
            self.shared_state.set_PowerupBricklets_onetime_initialization(False)
            return False
        else:
            print("Bricklet Initialization Successful")
            self.shared_state.set_PowerupBricklets_onetime_initialization(True)    
            return True
        

    def update_Bricklets_status(self, text: str):
        # This might be called from a worker / Tinkerforge thread.
        # DO NOT touch Qt widgets here, just emit.
        self.bricklets_status_signal.emit(text)
        
    def on_bricklets_status_signal(self, text: str):
        full_text = f"MVP Bricklets : {text}"

        if "down" in text or "Failure" in text or  "Error" in text or "Failed" in text or "not connected" in text or "Exception" in text or "Missing Bricklet" in text or "Cancelling" in text:
            color = "red"
        elif "Initialization completed" in text:
            color = "green"
        else:
            color = "yellow"

        self.bricklets_status.setText(full_text)
        self.bricklets_status.setStyleSheet(
            f"font-size: 18px; color: {color}; font-weight: bold; background-color: #242746;"
        )
        
        
    def initialize_bricklets(self):
        try:
            if self.shared_state.get_device_connection_state():
                device_ip = self.shared_state.device1_ip
                print("device_ip for bricklet init:", device_ip)
            else:
                print("No devices connected for bricklet initialization")
                return False

            self.ip_con, ID = create_connection()

            if self.ip_con is None:
                print("Failed to create connection to device")
                # Display the error message in the connection screen lable
                self.update_Bricklets_status(f"Bricklet is not connected {ID}")
                return False
                

            self.disconnect_button.setEnabled(False)
            Powerup_Sequence = integrate.Power_UP_Bricklet(self.ip_con, self.update_Bricklets_status)
            if not Powerup_Sequence:
                self.update_Bricklets_status("Initialization Failed")
                self.disconnect_button.setEnabled(True)
                self.shared_state.set_PowerupBricklets_onetime_initialization(False)
                return False           

            if Powerup_Sequence == "AirQ Failed":
                self.update_Bricklets_status("Initialization Failed in Air Quality Bricklet")
                self.disconnect_button.setEnabled(True)
                self.shared_state.set_PowerupBricklets_onetime_initialization(False)
                return False

            #:self.update_Bricklets_status("Initialization completed")
            self.disconnect_button.setEnabled(True)
            time.sleep(2)
            self.toggle_sidebar_signal.emit(True)
            print("T7 Sidebar toggled on")
            self.logout_button.setEnabled(True)            
            if self.shared_state.get_device_connection_state():
                self.update_status_signal.emit("Device 1", "Connected")   
            return True
        except Exception as e:
            print(f"Error during bricklet initialization: {e}")
            self.disconnect_button.setEnabled(True)
            self.update_Bricklets_status("Initialization Failed")
            self.shared_state.set_PowerupBricklets_onetime_initialization(False)
            self.toggle_sidebar_signal.emit(True)
            return False

    @pyqtSlot(str)
    def show_continue_popup(self, device):
        msg = QMessageBox()
        msg.setIcon(QMessageBox.Warning)
        msg.setWindowTitle(device)
        msg.setText("Do you want to continue with a single device connection?")
        msg.setStandardButtons(QMessageBox.Yes | QMessageBox.No)
        self.continue_popup_selected = msg.exec_()
        self.continue_popup_status = True

    def disconnect_device1(self):
        print("disconnect_device1 called", self.shared_state.get_device_connection_state())
        print("disconnect_device1 called input text", self.device1_input.text())
        if self.shared_state.get_device_connection_state() and self.device1_input.text():
            device1_ip = self.device1_input.text()
            try:
                
                if not self.shared_state.get_Disconnect_Cubedevice():
                    rc = self.shared_state.cube.Disconnect(device1_ip)
                    self.shared_state.set_failed_status("Disconnected")
                else:                    
                    rc = True  
                    if self.shared_state.get_failed_status() == "":
                        self.shared_state.set_failed_status("Disconnected (Network Error)")
                    
                        
                if rc:
                    self.update_status_signal.emit("Device 1",self.shared_state.get_failed_status())
                    #QMetaObject.invokeMethod(self, "update_ui_after_disconnect", Qt.QueuedConnection)
                    #self.update_ui_after_disconnect_signal.emit()
                    #self.log_button.setEnabled(True)
                else:
                    self.update_status_signal.emit("Device 1", "Disconnected")
                
                self.connection_in_progress = False
                self.Settings_Screen_checkbox_controls_signal.emit(False)
                self.shared_state.set_device_connection_state(False)
                self.enable_bricklets_checkbox.setEnabled(True)
                self.update_ui_after_disconnect_signal.emit()
                self.log_button.setEnabled(True)
            except Exception as e:
                logging.error(f"Error disconnecting Device 1: {e}")
                if self.shared_state.get_failed_status() == "":
                    self.update_status_signal.emit("Device 1", "Disconnection Failed")
                else:
                    self.update_status_signal.emit("Device 1",self.shared_state.get_failed_status())

                self.shared_state.set_device_connection_state(False)
                self.enable_bricklets_checkbox.setEnabled(True) 
                self.log_button.setEnabled(True)
                self.connection_in_progress = False

    def disconnect_devices(self):
        print("USER Pressed Disconnect button",self.shared_state.get_device_connection_state())

        self.stop_connecting_animation()
        # Lock this
        print("DB : Acquiring Lock")
        integrate._disconnect_lock.acquire()
        if self.shared_state.get_device_connection_state():
          
            integrate.set_stopmeasurement1(True)
            print("UI Disconnected")
            tnow = datetime.now()
            timeout = timedelta(seconds=20)
            while not integrate.get_exit_measurement_thread1():
                print(f"Disconnect Exit Thread loop : {integrate.get_exit_measurement_thread1()}")
                if (datetime.now() - tnow) > timeout:
                    break
                time.sleep(0.25)

            self.disconnect_device1()    
            integrate.set_stopmeasurement1(False)
            #integrate.set_exit_measurement_thread1(False)
            # Deactivate temperature monitoring
            integrate.set_temperature_monitor_active(False) 
            self.shared_state.device1_ip = None
        
        # UnLock this
        print("DB : Releasiing Lock")
        integrate._disconnect_lock.release()
          
    @pyqtSlot()
    def update_ui_after_disconnect(self):
        self.enable_ui()
        any_connected = self.shared_state.get_device_connection_state()
        if any_connected is not None:
            print("Disconnect state", any_connected)
            self.connect_button.setEnabled(not any_connected)
            self.disconnect_button.setEnabled(any_connected)

    @pyqtSlot(str, str)
    def update_status(self, device, status):
        if device == "Device 1":
            if status.startswith("Initializing bricklets please wait"):
                return
            self.device1_status.setText(f"Device 1 : {status}")
            if "Connected" in status:
                time.sleep(0.5)
                self.device1_status.setStyleSheet("font-size: 18px; color: green; font-weight: bold; background-color: #242746;")
            
            elif "missing" in status or "down" in status or "Disconnected" in status or "Failure" in status or "Error" in status or "Failed" in status or "not connected" in status or "Exception" in status or "Missing Bricklet" in status or "Cancelling" in status:
                self.device1_status.setStyleSheet("font-size: 18px; color: red; font-weight: bold; background-color: #242746;")
            else:
                self.device1_status.setStyleSheet("font-size: 18px; color: white; font-weight: bold; background-color: #242746;")

    def create_input(self, label_text, default_value):
        label = QLabel(label_text)
        label.setStyleSheet("font-size: 18px; color: white; font-weight: bold; background-color: #242746;")
        input_field = QLineEdit()
        clean_label_text = label_text.rstrip(':')
        input_field.setPlaceholderText(f"Enter {clean_label_text}")
        input_field.setFixedSize(300, 50)
        self.apply_input_style(input_field)
        return label, input_field

    def apply_input_style(self, input_widget):
        input_widget.setStyleSheet("""
            QLineEdit {
                background-color: white;
                color: black;
                font-size: 18px;
                font-weight: bold;
                padding: 10px;  
                border: 2px solid #ccc;
                border-radius: 10px;
            }
        """)

