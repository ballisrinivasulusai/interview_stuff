import os
import struct
import time
import csv
from datetime import datetime, timedelta
from PyQt5.QtCore import QThread, pyqtSignal, Qt, QObject, QMetaObject, Q_ARG
from PyQt5.QtWidgets import (
    QApplication, QVBoxLayout, QHBoxLayout, QLabel, QLineEdit, 
    QPushButton, QTableWidget, QTableWidgetItem, QHeaderView, QMessageBox,
    QDialog, QTextEdit, QFrame, QStackedWidget, QAbstractItemView
)
from PyQt5.QtGui import QTextCursor, QColor, QDoubleValidator
import sys
import math
import integrate
from sysv_ipc import MessageQueue, IPC_CREAT
from TitleBar import TitleBar
from SharedState import APP_VERSION
import tarfile
import shutil
import faulthandler

faulthandler.enable()

CMD_ID_UPLOAD   = 1
UPLOAD_MSGQ_KEY = 0x1000

class TerminalDialog(QDialog):
    def __init__(self, parent=None, test_code="", pulse_value="", voc_data=None, argon_data=None, dwell_value=0.0, dstep_value=0.0, gstep_value=0.0, vg_value=0.0, vsd_value=0.0, seltemp_value=0.0, selcoiltemp_value=0.0,selS1Vtemp_value=0.0,selS2Vtemp_value=0.0,selN2Htemp_value=0.0, ttotalvalue=0.0,shared_state=None):
        super().__init__(parent)    
        print("TerminalDialog show")
        self.parent = parent  
        self.shared_state = shared_state
        self.setWindowFlags(Qt.Dialog | Qt.FramelessWindowHint)
        self.setFixedWidth(1500)
        self.setStyleSheet("""
            QDialog {
                background-color: #1D2B3A;
                border: 2px solid white;
                border-radius: 5px;
            }
            QLabel {
                font-weight: bold;
            }
        """)
        # Main layout
        layout = QVBoxLayout(self)
        layout.setContentsMargins(15, 15, 15, 15)
        layout.setSpacing(15)
        
        # Test code and pulse code info
        info_frame = QFrame()
        info_frame.setStyleSheet("background-color: #1D2B3A;")
        info_layout = QVBoxLayout(info_frame)
        info_layout.setSpacing(10)
        info_layout.setAlignment(Qt.AlignCenter)

        # Selected Test Code and Pulse Code
        test_code_layout = QHBoxLayout()
        test_code_layout.setAlignment(Qt.AlignCenter)
        test_code_label = QLabel("Selected Test Code:")
        test_code_label.setStyleSheet("color: #72B6DC; font-size: 18px;")
        self.test_code_value = QLabel(test_code)
        self.test_code_value.setStyleSheet("color: white; font-size: 18px;")
        test_code_layout.addWidget(test_code_label)
        test_code_layout.addWidget(self.test_code_value)

        pulse_layout = QHBoxLayout()
        pulse_layout.setAlignment(Qt.AlignCenter)
        pulse_label = QLabel("Pulse Code:")
        pulse_label.setStyleSheet("color: #72B6DC; font-size: 18px;")
        self.pulse_value = QLabel(pulse_value)
        self.pulse_value.setStyleSheet("color: white; font-size: 18px;")
        pulse_layout.addWidget(pulse_label)
        pulse_layout.addWidget(self.pulse_value)

        info_layout.addLayout(test_code_layout)
        info_layout.addLayout(pulse_layout)

        # Second Row: D-Well, D-Step, G-Step
        row2_layout = QHBoxLayout()
        row2_layout.setAlignment(Qt.AlignCenter)

        dwell_label = QLabel("D-Well:")
        dwell_label.setStyleSheet("color: #72B6DC; font-size: 18px;")
        self.dwell_lable_value = QLabel(f"{dwell_value:.2f}")
        self.dwell_lable_value.setStyleSheet("color: white; font-size: 18px;")

        dstep_label = QLabel("D-Step:")
        dstep_label.setStyleSheet("color: #72B6DC; font-size: 18px;")
        self.dstep_label_value = QLabel(f"{dstep_value:.2f}")
        self.dstep_label_value.setStyleSheet("color: white; font-size: 18px;")

        gstep_label = QLabel("G-Step:")
        gstep_label.setStyleSheet("color: #72B6DC; font-size: 18px;")
        self.gstep_lable_value = QLabel(f"{gstep_value:.2f}")
        self.gstep_lable_value.setStyleSheet("color: white; font-size: 18px;")

        row2_layout.addWidget(dwell_label)
        row2_layout.addWidget(self.dwell_lable_value)
        row2_layout.addSpacing(20)
        row2_layout.addWidget(dstep_label)
        row2_layout.addWidget(self.dstep_label_value)
        row2_layout.addSpacing(10)
        row2_layout.addWidget(gstep_label)
        row2_layout.addWidget(self.gstep_lable_value)
        info_layout.addLayout(row2_layout)

        # Third Row: Vg, Vsd, Time
        row3_layout = QHBoxLayout()
        row3_layout.setAlignment(Qt.AlignCenter)

        vg_label = QLabel("Vg:")
        vg_label.setStyleSheet("color: #72B6DC; font-size: 18px;")
        self.vg_label_value = QLabel(f"{float(vg_value):.2f}")
        self.vg_label_value.setStyleSheet("color: white; font-size: 18px;")

        vsd_label = QLabel("Vsd:")
        vsd_label.setStyleSheet("color: #72B6DC; font-size: 18px;")
        self.vsd_label_value = QLabel(f"{float(vsd_value)}")
        self.vsd_label_value.setStyleSheet("color: white; font-size: 18px;")

        seltemp_label = QLabel("SH(°C):")
        seltemp_label.setStyleSheet("color: #72B6DC; font-size: 18px;")
        self.seltemp_label_value = QLabel(f"{float(seltemp_value)}")
        self.seltemp_label_value.setStyleSheet("color: white; font-size: 18px;")
        
        selcoiltemp_label = QLabel("CH(°C):")
        selcoiltemp_label.setStyleSheet("color: #72B6DC; font-size: 18px;")
        self.selcoiltemp_label_value = QLabel(f"{float(selcoiltemp_value)}")
        self.selcoiltemp_label_value.setStyleSheet("color: white; font-size: 18px;")
        
        selS1Vtemp_label = QLabel("S1V(°C):")
        selS1Vtemp_label.setStyleSheet("color: #72B6DC; font-size: 18px;")
        self.selS1Vtemp_label_value = QLabel(f"{float(selS1Vtemp_value)}")
        self.selS1Vtemp_label_value.setStyleSheet("color: white; font-size: 18px;")
        
        selS2Vtemp_label = QLabel("S2V(°C):")
        selS2Vtemp_label.setStyleSheet("color: #72B6DC; font-size: 18px;")
        self.selS2Vtemp_label_value = QLabel(f"{float(selS2Vtemp_value)}")
        self.selS2Vtemp_label_value.setStyleSheet("color: white; font-size: 18px;")
        
        selN2Htemp_label = QLabel("N2H(°C):")
        selN2Htemp_label.setStyleSheet("color: #72B6DC; font-size: 18px;")
        self.selN2Htemp_label_value = QLabel(f"{float(selN2Htemp_value)}")
        self.selN2Htemp_label_value.setStyleSheet("color: white; font-size: 18px;")

        ttotal_label = QLabel("T-Total :")
        ttotal_label.setStyleSheet("color: #72B6DC; font-size: 18px;")
        self.ttotal_label_value = QLabel(f"{float(ttotalvalue):.2f}")
        self.ttotal_label_value.setStyleSheet("color: white; font-size: 18px;")

        Timer_label = QLabel("Measurement Timer :")
        Timer_label.setStyleSheet("color: #72B6DC; font-size: 18px;")
        self.Timer_label_value = QLabel("")
        self.Timer_label_value.setStyleSheet("color: white; font-size: 18px;")

        row3_layout.addWidget(vg_label)
        row3_layout.addWidget(self.vg_label_value)
        row3_layout.addSpacing(20)
        row3_layout.addWidget(vsd_label)
        row3_layout.addWidget(self.vsd_label_value)
        row3_layout.addSpacing(20)
        row3_layout.addWidget(seltemp_label)
        row3_layout.addWidget(self.seltemp_label_value)
        row3_layout.addSpacing(20)
        row3_layout.addWidget(selcoiltemp_label)
        row3_layout.addWidget(self.selcoiltemp_label_value)
        row3_layout.addSpacing(20)
        row3_layout.addWidget(selS1Vtemp_label)
        row3_layout.addWidget(self.selS1Vtemp_label_value)
        row3_layout.addSpacing(20)
        row3_layout.addWidget(selS2Vtemp_label)
        row3_layout.addWidget(self.selS2Vtemp_label_value)
        row3_layout.addSpacing(20)
        row3_layout.addWidget(selN2Htemp_label)
        row3_layout.addWidget(self.selN2Htemp_label_value)
        row3_layout.addSpacing(20)
        row3_layout.addWidget(ttotal_label)
        row3_layout.addWidget(self.ttotal_label_value)
        row3_layout.addWidget(Timer_label)
        row3_layout.addWidget(self.Timer_label_value)

        info_layout.addLayout(row3_layout)

        # Fourth Row: Nitrogen flow set
        row4_layout = QHBoxLayout()
        row4_layout.setAlignment(Qt.AlignCenter)
        
        self.argon_flow_label = QLabel("Change NitrogenFlow")
        self.argon_flow_label.setStyleSheet("color: #72B6DC; font-size: 18px;")

        self.argon_flow_edit = QLineEdit()
        validator = QDoubleValidator()
        validator.setNotation(QDoubleValidator.StandardNotation)
        self.argon_flow_edit.setValidator(validator)
        self.argon_flow_edit.setStyleSheet("""
            QLineEdit {
                background-color: darkgray;
                color: black;
                font-size: 16px;
                border: 1px solid #3A4F63;
                border-radius: 3px;
                padding: 5px;
            }
        """)
        self.argon_flow_edit.setFixedWidth(150)

        self.set_button = QPushButton("SET")
        self.set_button.setFixedSize(80, 30)
        self.set_button.setStyleSheet("""
            QPushButton {
                background-color: #72B6DC;
                color: #1D2B3A;
                font-size: 14px;
                font-weight: bold;
                border: 2px solid #3A4F63;
                border-radius: 5px;
                padding: 5px;
            }
            QPushButton:hover {
                background-color: #5A9EC0;
            }
        """)
        self.set_button.clicked.connect(self.on_set_button_clicked)

        center_wrapper = QHBoxLayout()
        center_wrapper.addStretch()
        center_wrapper.addLayout(row4_layout)
        center_wrapper.addStretch()

        row4_layout.addWidget(self.argon_flow_label)
        row4_layout.addWidget(self.argon_flow_edit)
        row4_layout.addWidget(self.set_button)

        info_layout.addLayout(center_wrapper)
        
        self.timestamp = 0
        # VOC Table
        if '+' in pulse_value:
            self.argon_pulse_code, self.voc_pulse_code = pulse_value.split('+')
            self.argon_pulse_code = self.argon_pulse_code.strip()
            self.voc_pulse_code = self.voc_pulse_code.strip()
                       
            self.voc_row = next((row for row in voc_data if row[0] == self.voc_pulse_code), None)
            if self.voc_row:
                voc_label = QLabel("VOC Parameters:")
                voc_label.setStyleSheet("color: #72B6DC; font-size: 18px;")
                info_layout.addWidget(voc_label)
                
                self.voc_table = QTableWidget(1, 4)
                self.voc_table.setHorizontalHeaderLabels(["VOC \n Pulse Code", "Mixed Constant(Nitrogen + VOC) \n %",  "T1 \n (sec)", "T2 \n (sec)"])  
                self.voc_table.verticalHeader().setVisible(False)
                self.voc_table.setStyleSheet("""
                    QTableWidget {
                        background-color: #1D2B3A;
                        color: white;
                        border: 1px solid #3A4F63;
                        font-size: 12px;
                    }
                    QHeaderView::section {
                        background-color: white;
                        color: black;
                        font-weight: bold;
                        padding: 5px;
                        border: 1px solid #3A4F63;
                    }
                """)
                self.voc_table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
                self.voc_table.setFixedHeight(100)
                self.voc_table.setSelectionMode(QTableWidget.NoSelection)
                
                self.voc_table.setItem(0, 0, QTableWidgetItem(self.voc_pulse_code))
                self.voc_table.setItem(0, 1, QTableWidgetItem(str(self.voc_row[1])))
                self.voc_table.setItem(0, 2, QTableWidgetItem(str(self.voc_row[2])))
                self.voc_table.setItem(0, 3, QTableWidgetItem(str(self.voc_row[3])))
                
                for col in range(self.voc_table.columnCount()):
                    item = self.voc_table.item(0, col)
                    if item:
                        item.setTextAlignment(Qt.AlignCenter)
                        item.setFlags(item.flags() & ~Qt.ItemIsEditable & ~Qt.ItemIsSelectable)
                        item.setForeground(QColor(Qt.white))
                
                info_layout.addWidget(self.voc_table)
        
        # Nitrogen Table
        if '+' in pulse_value:
            self.argon_row = next((row for row in argon_data if row[0] == self.argon_pulse_code), None)
            if self.argon_row:
                argon_label = QLabel("Nitrogen Parameters:")
                argon_label.setStyleSheet("color: #72B6DC; font-size: 18px;")
                info_layout.addWidget(argon_label)
                
                self.argon_table = QTableWidget(1, 4)
                self.argon_table.setHorizontalHeaderLabels(["Nitrogen \n Pulse Code", "Nitrogen \n PPM", "T0 \n (sec)", "T3 \n (sec)"])  
                self.argon_table.verticalHeader().setVisible(False)
                self.argon_table.setStyleSheet("""
                    QTableWidget {
                        background-color: #1D2B3A;
                        color: white;
                        border: 1px solid #3A4F63;
                        font-size: 12px;
                    }
                    QHeaderView::section {
                        background-color: white;
                        color: black;
                        font-weight: bold;
                        padding: 5px;
                        border: 1px solid #3A4F63;
                    }
                """)
                self.argon_table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
                self.argon_table.setFixedHeight(100)
                self.argon_table.setSelectionMode(QTableWidget.NoSelection)
                
                self.argon_table.setItem(0, 0, QTableWidgetItem(self.argon_pulse_code))
                self.argon_table.setItem(0, 1, QTableWidgetItem(str(self.argon_row[1])))
                self.argon_table.setItem(0, 2, QTableWidgetItem(str(self.argon_row[2])))
                self.argon_table.setItem(0, 3, QTableWidgetItem(str(self.argon_row[3])))
                
                for col in range(self.argon_table.columnCount()):
                    item = self.argon_table.item(0, col)
                    if item:
                        item.setTextAlignment(Qt.AlignCenter)
                        item.setFlags(item.flags() & ~Qt.ItemIsEditable & ~Qt.ItemIsSelectable)
                        item.setForeground(QColor(Qt.white))
                
                info_layout.addWidget(self.argon_table)
        
        self.status_label = QLabel()
        self.status_label.setStyleSheet("""
            QLabel {
                color: #FFFFFF;
                font-size: 18px;
                font-family: 'Times New Roman';
                background-color: #1D2B3A; 
                padding: 5px; 
                border-radius: 5px;
            }
        """)
        self.status_label.setAlignment(Qt.AlignCenter)
        
        self.terminal = QTextEdit()
        self.terminal.setFixedHeight(250)
        self.terminal.setReadOnly(True)
        self.terminal.setStyleSheet("""
            QTextEdit {
                background-color: black;
                color: white;
                font-family: Consolas, Courier New, monospace;
                font-size: 16px;
                border-radius: 5px;
                padding: 5px;
            }
        """)
        
        self.ok_button = QPushButton("OK")
        self.ok_button.setFixedSize(120, 35)
        self.ok_button.setEnabled(False)
        self.ok_button.setStyleSheet("""
            QPushButton {
                background-color: #1D2B3A;
                color: #72B6DC;
                font-size: 14px;
                font-weight: bold;
                border: 2px solid #3A4F63;
                border-radius: 5px;
                padding: 5px;
            }
            QPushButton:enabled {
                background-color: #72B6DC;
                color: #1D2B3A;
            }
            QPushButton:hover {
                border: 2px solid #72B6DC;
            }
        """)
        self.ok_button.clicked.connect(self.close_dialog)     
        
        self.cancel_button = QPushButton("Cancel")
        self.cancel_button.setFixedSize(120, 35)
        self.cancel_button.setStyleSheet("""
            QPushButton {
                background-color: #1D2B3A;
                color: #72B6DC;
                font-size: 14px;
                font-weight: bold;
                border: 2px solid #3A4F63;
                border-radius: 5px;
                padding: 5px;
            }
            QPushButton:enabled {
                background-color: #72B6DC;
                color: #1D2B3A;
            }
            QPushButton:hover {
                border: 2px solid #72B6DC;
            }
        """)
        self.cancel_button.clicked.connect(self.on_cancel_clicked)        
           
        layout.addWidget(info_frame)
        layout.addWidget(self.cancel_button, alignment=Qt.AlignCenter)
        layout.addWidget(self.status_label)
        layout.addWidget(self.terminal)
        layout.addWidget(self.ok_button, alignment=Qt.AlignCenter)
    
    def on_set_button_clicked(self):
        text = self.argon_flow_edit.text()
        if text.strip():
            try:
                argon_flow_value = float(text)
            except ValueError:
                argon_flow_value = 0.0
        else:
            argon_flow_value = 0.0
        integrate.set_current_argon_flow(argon_flow_value)
        print(f"Nitrogen Flow value set to: {argon_flow_value}")
    
    def append_text(self, text):
        self.terminal.moveCursor(QTextCursor.End)
        self.terminal.insertPlainText(text + "\n")
        self.terminal.moveCursor(QTextCursor.End)
    
    def measurement_completed(self, success):
        if success:
            self.status_label.setText("MEASUREMENT COMPLETED SUCCESSFULLY! PLEASE WAIT FOR DISPLAYING  GRAPH.")
            self.status_label.setStyleSheet("""
                QLabel {
                    color: #00FF00;
                    font-size: 18px;
                    font-family: 'Times New Roman';
                }
            """)
        else:
            self.status_label.setText("MEASUREMENT FAILED!")
            self.status_label.setStyleSheet("""
                QLabel {
                    color: #FF0000;
                    font-size: 18px;
                    font-family: 'Times New Roman';
                }
            """)
        self.ok_button.setEnabled(True)
    
    def close_dialog(self):
        if self.parent:
            self.parent.show_measurement_widgets()
        self.close()

    def on_cancel_clicked(self):
        # Prevent multiple clicks
        if not self.cancel_button.isEnabled():
            return
        self.cancel_button.setEnabled(False)

        print("<WT> <User Pressed Cancel button>")
        
        # --- UI Setup ---
        self.status_label.setText("Please wait, the current thread is closing the resources...")
        self.status_label.setStyleSheet("""
            QLabel {
                color: white;
                font-size: 18px;
                font-family: 'Times New Roman';
                background-color: #1D2B3A;
                padding: 5px;
                border-radius: 5px;
            }
        """)
        QApplication.processEvents()  

        # Set Cancel event Flag
        self.parent.shared_state.set_Cancel_event(True)

        return 
    

value = 2

# Worker classes for Bricklet and Timer threads
class BrickletWorker(QObject):
    finished = pyqtSignal()  # Signal to indicate completion

    def __init__(self, measurement_screen):
        super().__init__()
        self.measurement_screen = measurement_screen
        self.integrate = integrate

    def run(self):
        try:
            self.integrate.BrickletThread(self.measurement_screen)
        finally:
            self.finished.emit()
    
    

class TimerWorker(QObject):
    finished = pyqtSignal()  # Signal to indicate completion

    def __init__(self, measurement_screen):
        super().__init__()
        self.measurement_screen = measurement_screen
        self.integrate = integrate

    def run(self):
        try:
            self.integrate.Meas_timer_counter_thread(self.measurement_screen)
        finally:
            self.finished.emit()

class TemperatureMonitorWorker(QObject):
    finished = pyqtSignal()  # Signal to indicate completion

    def __init__(self, measurement_screen):
        super().__init__()
        self.measurement_screen = measurement_screen
        self.integrate = integrate

    def run(self):
        try:
            self.integrate.Temperature_Monitor_Worker_thread(self.measurement_screen)
        finally:
            self.finished.emit()

# Custom QThread class for device measurement
class DeviceMeasurementThread(QThread):
    update_status = pyqtSignal(str, str, str)  # status_message, device, ip_address
    measurement_completed = pyqtSignal(str, bool)  # device, success

    def __init__(self, measurement_screen, device_list):
        super().__init__()
        self.measurement_screen = measurement_screen
        self.device, self.ip_address, self.filename = device_list
        self.shared_state = measurement_screen.shared_state
        self.integrate = integrate

    def run(self):
        try:
            print(f"<MT> Started Measurement Channel Thread for {self.device}")
            self.shared_state.set_continue_measurement(False)
            self.integrate.set_MeasurmentThread_Closed(False)
            self.integrate.set_exit_measurement_thread1(False)
            while True:
                while True:    
                    try:
                        print("<MT> Waiting for Startmeasurement event")

                        # Wait for Start Measurement Event
                        event_set = self.integrate.startmeasurement1_event.wait(timeout=1)

                        # Application X button Close and Ethernet/Bricklet connection is disconnected
                        #if self.integrate.get_Appclose() or self.shared_state.get_Disconnect_Cubedevice():
                        if self.integrate.get_Appclose():
                            print(f"<MT> Application {integrate.get_Appclose()} closing, exiting DeviceMeasurementThread {self.shared_state.get_Disconnect_Cubedevice()}")
                            self.integrate.set_MeasurmentThread_Closed(True)
                            #self.integrate.set_exit_measurement_thread1(True)
                            return

                        if self.integrate.get_stopmeasurement1():
                            print(f"<MT> Stopmeasurment{self.integrate.get_stopmeasurement1()} exited DeviceMeasurementThread")
                            integrate.set_exit_measurement_thread1(True)
                            self.integrate.set_MeasurmentThread_Closed(True)
                            return
                            

                        if self.integrate.get_Cancel_Measurement():
                            print("<MT> User Pressed the Cancel button During the Measurment")
                            integrate.set_MeasurmentThread_Closed(True)
                        
                        if self.integrate.get_exit_measurement_thread1():
                            self.integrate.set_MeasurmentThread_Closed(True)
                            print("<MT> get_exit_measurement_thread1")
                            return
                            
                        if event_set:
                            print("Event Received")
                            break                        
                        
                    except Exception as e:
                        print(f"<MT> Error in Measurement inner loop: {e}")

                try:
                    print("<MT> Received the Startmeasurement event")
                    # Clear the event for next use  
                    self.integrate.startmeasurement1_event.clear()

                    self.shared_state.set_failed_status(" ")

                    self.integrate.set_view_graph1(False)
                    self.integrate.set_view_graph(False)
                    self.shared_state.set_measurement_failed(False)

                    print("<MT> integrate.Calibrate_Measurement", self.integrate.get_Calibrate_Measurement())
                    print("<MT> integrate.get_Sweep_Measurement",self.integrate.get_Sweep_Measurement())
                    print("<MT> self.integrate.get_Normal_Measurement()", self.integrate.get_Normal_Measurement())
                    print("<MT> self.integrate.get_Calibrate_Measurement()", self.integrate.get_Calibrate_Measurement())

                        
                        
                    # Set the Flag to get the Permission from the connection thread 
                    # to Start the measurement.
                    self.shared_state.set_continue_measurement(True)
                    tnow = datetime.now()
                    while True:
                        rc = self.shared_state.get_measurement_granted()
                        if rc:
                            break

                        if datetime.now() > tnow + timedelta(seconds=10):
                            return False

                        time.sleep(0.5)        

                    # Select measurement function
                    self.shared_state.set_timestamp(str(time.strftime("%Y%m%d_%H%M%S")))
                    
                    now = datetime.now()
                    print("<MT> Measurement Started Time", now)

                    # ZNKB Clear all the events
                    integrate.Start_data_acquisition_event.clear()

                    if (self.integrate.get_Sweep_Measurement() or self.integrate.get_Normal_Measurement()):
                        # Advanced settings measurement started
                        self.measurement_screen.AdvancedSettingsGraphFunction(self.device, self.ip_address, self.filename)

                    elif self.integrate.get_mainmeasurement():
                        # Main Measurement Started
                        self.is_active = True
                        self.measurement_screen.BrickletMeasurementGraphFunction(self.device, self.ip_address, self.filename)
                    elif self.integrate.get_Calibrate_Measurement():
                        #Calibration Measurement 
                        self.measurement_screen.CalibrationScreen_GraphFunction(self.device, self.ip_address, self.filename)
                except Exception as e:
                    print(f"<MT> Error in measurement main loop: {e}")
                    break
                
                now = datetime.now()
                print("<MT> Measurement Completed Time", now)
                    
                integrate.set_view_graph(False)
                integrate.set_view_graph1(False)
                integrate.set_mainmeasurement(False)
                integrate.set_stop_measurement_data_acq(False)
                #self.shared_state.set_continue_measurement(False)
                integrate.set_Sweep_Measurement(False)
                integrate.set_Normal_Measurement(False)
                self.is_active = False

        except Exception as e:
            print(f"<MT> Critical error in StartDeviceMeasurement for {self.device}: {e}")
            self.measurement_completed.emit(self.device, False)

class MeasurementScreen(TitleBar):
    update_status_signal = pyqtSignal(str, str, str)
    bricklet_status_signal = pyqtSignal(str)
    show_message_signal = pyqtSignal(str, str)
    measurement_complete_signal = pyqtSignal(bool)
    close_dialog_signal = pyqtSignal()
    Timer_status_signal = pyqtSignal(str)
    control_update_signal = pyqtSignal(str)
    switch_to_sample_screen = pyqtSignal(bool, str, int)
    advanced_settings_complete_signal = pyqtSignal()
    sample_temperature_update_signal = pyqtSignal(float)
    coil_temperature_update_signal = pyqtSignal(float)
    aircube_temperature_update_signal = pyqtSignal(float)
    aircube_humidity_update_signal = pyqtSignal(float)
    aircube_pressure_update_signal = pyqtSignal(float)
    advanced_settings_update_signal = pyqtSignal(float)
    status_label_text = pyqtSignal(str)
    status_label_style = pyqtSignal(str, str)   # text, color
    argon_controls_state = pyqtSignal(bool)     # True = enable, False = disable    
    
    updatestatus_Advanced_SEttings_Signal = pyqtSignal(str,str)
    enable_ui_Signal = pyqtSignal()
    

    def __init__(self, main_window, username, shared_state):
        super().__init__(main_window, username, "MEASURE")
        self.shared_state = shared_state
        self.meas_max_count = 0
        self.completed_devices = set()
        self.connected_devices = None
        self.Start_timer = False
        self.failed_devices = set()
        self.base_path = os.path.join(self.get_app_path1(), 'data')
        self.db_path = os.path.join(self.get_app_path1(), 'res', 'measurementtable.db')
        self.terminal_dialog = None
        self.devices = []
        self.is_active = False
        self.status_messages = []
        self.date_str = datetime.now().strftime('%d_%m_%Y')
        self.today_date_folder_path = os.path.join(self.base_path, self.date_str)
        os.makedirs(self.today_date_folder_path , exist_ok=True)
        self.stacked_widget = QStackedWidget()
        self.layout().addWidget(self.stacked_widget)
        self.device1NotReady = False
        self.update_status_signal.connect(self.update_terminal)
        self.bricklet_status_signal.connect(self.update_bricklet_status)
        self.show_message_signal.connect(self.show_message_box)
        self.measurement_complete_signal.connect(self.handle_measurement_completion)
        self.updatestatus_Advanced_SEttings_Signal.connect(self.update_AdvancedSettings_measurmentstatus)
        self.enable_ui_Signal.connect(self.enable_ui)
        self.close_dialog_signal.connect(self.close_terminal_dialog)
        self.control_update_signal.connect(self.handle_control_update)
        self.switch_to_sample_screen.connect(self.handle_switch_to_sample)
        self.advanced_settings_complete_signal.connect(self.handle_advanced_settings_operations)
        self.threads = []  # Store QThread instances
        # Upload Initialization flag
        self.mqId_upload = MessageQueue(UPLOAD_MSGQ_KEY, IPC_CREAT)
        print("Firstview Zaid mqId_MEAS: ", self.mqId_upload)
        self.show_terminal()

    def create_temperature_log_folder(self, base_path, date_str):
        """Create temperature_log folder if it doesn't exist"""
        temp_log_path = os.path.join(base_path, date_str, 'temperature_log')
        try:
            os.makedirs(temp_log_path, exist_ok=True)
            print(f"[DEBUG] Created/verified temperature log folder: {temp_log_path}")
            if not os.path.exists(temp_log_path):
                print(f"[ERROR] Failed to create temperature log folder: {temp_log_path}")
                raise OSError(f"Directory {temp_log_path} does not exist after creation attempt")
            return temp_log_path
        except Exception as e:
            print(f"[ERROR] Failed to create temperature log folder {temp_log_path}: {e}")
            raise

    def rename_temperature_log_file(self, base_path, date_str, original_filename):
        """Rename rawtext file to temperature log format"""
        print(f"[DEBUG] Original filename: {original_filename}")
        if 'rawtext1' in original_filename:
            new_filename = original_filename.replace('rawtext1', 'temperature_log') + '.txt'
            full_path = os.path.join(base_path, date_str, 'temperature_log', new_filename)
            print(f"[DEBUG] Generated temperature log file path: {full_path}")
            return full_path
        else:
            print(f"[ERROR] 'rawtext1' not found in filename: {original_filename}")
            return None


    def handle_advanced_settings_operations(self):
        """
        Slot to handle operations on AdvancedSettingScreen when the signal is emitted.
        """
        
        print("rawtext_file_path",self.shared_state.last_rawtext_file_path)
        AdvancedSettingScreen = self.main_window.screens["AdvanceSettings"]
        AdvancedSettingScreen.lasttransferred_rawtext_file_path = self.shared_state.last_rawtext_file_path
        AdvancedSettingScreen._complete_measurement()
        AdvancedSettingScreen.update_available_samples()
        AdvancedSettingScreen.plot_graphs()
        AdvancedSettingScreen.refresh_samples()
        AdvancedSettingScreen.enable_ui()

    def handle_switch_to_sample(self, plot_calib_graph, sample_title, current_sample):
        """
        Handler for the switch_to_sample_screen signal.
        Updates the Sample screen and switches to it.
        """

        Settings_screen = self.main_window.screens["Settings"]
        Settings_screen.UI_Visiblity(True)
        if self.shared_state.get_BackFlush_ON():
            Settings_screen.backflush_status_label.setText("Backflush: ON")
            Settings_screen.on_btn.setEnabled(False)
        else:
            Settings_screen.backflush_status_label.setText("Backflush: OFF")
            Settings_screen.off_btn.setEnabled(False)

        sample_screen = self.main_window.screens["Sample"]
        sample_screen.lasttransferred_rawtext_file_path = self.shared_state.last_rawtext_file_path
        sample_screen.PlotCalibgraph = plot_calib_graph
        sample_screen.generate_graph()
        sample_screen.sampletitle = sample_title
        sample_screen.current_sample = current_sample
        self.main_window.change_screen("Sample")

    # getters and setters
    def get_meas_max_count(self):
        with integrate._shared_state_lock:
            return self.meas_max_count

    def set_meas_max_count(self, value: int):
        with integrate._shared_state_lock:
            self.meas_max_count = value   

    def handle_control_update(self, command):
        if command == "enable_controls":
            self.argon_controls_state.emit(True)
 
        elif command == "disable_controls":
            self.argon_controls_state.emit(False)
            self.terminal_dialog.argon_flow_edit.setStyleSheet("""
                QLineEdit {
                    background-color: darkgray;
                    color: black;
                    font-size: 16px;
                    border: 1px solid #3A4F63;
                    border-radius: 3px;
                    padding: 5px;
                }
            """)

    def update_bricklet_status(self, status_message):
        if self.terminal_dialog and self.is_active:
            timestamp = time.strftime("%H:%M:%S", time.localtime())
            self.terminal_dialog.append_text(f"[{timestamp}] Bricklets: {status_message}")
        else:
            self.status_messages.append((status_message, "Bricklets", "N/A"))

    def show_terminal(self):
        print("[DEBUG] show_terminal called")
        self.is_active = True
        self.shared_state.set_timestamp(str(time.strftime("%Y%m%d_%H%M%S")))
        self.devices = [
            ("Device 1", self.shared_state.device1_ip, "rawtext1"),
        ]
        self.connected_devices = [(device, ip, path) for device, ip, path in self.devices if self.shared_state.get_device_connection_state()]
       
        for device, ip, path in self.connected_devices:
            print(f"Device: {device}, IP: {ip}, Path: {path}")

        self.setFixedHeight(900)
        self.calculate_ttotal = int(self.shared_state.get_T1() + self.shared_state.get_T2() + self.shared_state.get_T3())
        
        if self.terminal_dialog is None:
            print("Creating TerminalDialog")
            self.terminal_dialog = TerminalDialog(
                self,
                test_code=self.shared_state.get_current_test_code(),
                pulse_value=self.shared_state.get_current_pulse_value(),
                voc_data=self.shared_state.get_voc_data(),
                argon_data=self.shared_state.get_argon_data(),
                dwell_value=self.shared_state.get_var_t_dwell(),
                dstep_value=self.shared_state.get_var_d_step(),
                gstep_value=self.shared_state.get_var_g_step(),
                vg_value=self.shared_state.var_vg_measure,
                vsd_value=self.shared_state.var_vsd_measure,
                seltemp_value=float(integrate.get_heater_temperature()),
                selcoiltemp_value=float(integrate.get_tubing_coil_temp()),
                selS1Vtemp_value=float(integrate.get_S1V_temp()),
                selS2Vtemp_value=float(integrate.get_S2V_temp()),
                selN2Htemp_value=float(integrate.get_N2H_temp()),
                ttotalvalue=self.calculate_ttotal,
                shared_state=self.shared_state
            )
            
            # === Connect signals safely ===
            
            self.status_label_text.connect(self.terminal_dialog.status_label.setText)
            self.status_label_style.connect(self._update_status_label_with_color)
            self.argon_controls_state.connect(self._set_argon_controls_enabled)
            self.Timer_status_signal.connect(self.terminal_dialog.Timer_label_value.setText)
        else:
            # Reconnect signals when reusing dialog
            self.status_label_text.connect(self.terminal_dialog.status_label.setText)
            self.status_label_style.connect(self._update_status_label_with_color)
            self.argon_controls_state.connect(self._set_argon_controls_enabled)
            self.Timer_status_signal.connect(self.terminal_dialog.Timer_label_value.setText)
            print("Using already Created TerminalDialog")
            self.terminal_dialog.terminal.clear()
            self.terminal_dialog.test_code_value.setText(self.shared_state.get_current_test_code())
            self.terminal_dialog.pulse_value.setText(self.shared_state.get_current_pulse_value())

            lcurrent_pulse_value = self.shared_state.get_current_pulse_value()
            argon_pulse_code, voc_pulse_code = lcurrent_pulse_value.split('+')
            argon_pulse_code = argon_pulse_code.strip()
            voc_pulse_code = voc_pulse_code.strip()

            voc_row = self.shared_state.get_voc_data()
            voc_row = voc_row[0]
            argon_row = self.shared_state.get_argon_data()
            argon_row = argon_row[0]

            self.terminal_dialog.voc_row = voc_row
            self.terminal_dialog.argon_row = argon_row

            # Update VOC Table
            self.terminal_dialog.voc_table.setEditTriggers(QAbstractItemView.NoEditTriggers)
            self.terminal_dialog.voc_table.setItem(0, 0, QTableWidgetItem(voc_pulse_code))
            self.terminal_dialog.voc_table.setItem(0, 1, QTableWidgetItem(str(voc_row[1])))
            self.terminal_dialog.voc_table.setItem(0, 2, QTableWidgetItem(str(voc_row[2])))
            self.terminal_dialog.voc_table.setItem(0, 3, QTableWidgetItem(str(voc_row[3])))
            self.terminal_dialog.seltemp_label_value.setText(f"{float(integrate.get_heater_temperature())}")
            self.terminal_dialog.selcoiltemp_label_value.setText(f"{float(integrate.get_tubing_coil_temp())}")
            self.terminal_dialog.selS1Vtemp_label_value.setText(f"{float(integrate.get_S1V_temp())}")
            self.terminal_dialog.selS2Vtemp_label_value.setText(f"{float(integrate.get_S2V_temp())}")
            self.terminal_dialog.selN2Htemp_label_value.setText(f"{float(integrate.get_N2H_temp())}")
            
            for col in range(self.terminal_dialog.voc_table.columnCount()):
                item = self.terminal_dialog.voc_table.item(0, col)
                if item:
                    item.setTextAlignment(Qt.AlignCenter)

            # Update Nitrogen Table
            self.terminal_dialog.argon_table.setEditTriggers(QAbstractItemView.NoEditTriggers)
            self.terminal_dialog.argon_table.setItem(0, 0, QTableWidgetItem(argon_pulse_code))
            self.terminal_dialog.argon_table.setItem(0, 1, QTableWidgetItem(str(argon_row[1])))
            self.terminal_dialog.argon_table.setItem(0, 2, QTableWidgetItem(str(argon_row[2])))
            self.terminal_dialog.argon_table.setItem(0, 3, QTableWidgetItem(str(argon_row[3])))
            
            for col in range(self.terminal_dialog.argon_table.columnCount()):
                item = self.terminal_dialog.argon_table.item(0, col)
                if item:
                    item.setTextAlignment(Qt.AlignCenter)

            self.terminal_dialog.dwell_lable_value.setText(f"{float(self.shared_state.get_var_t_dwell())}")
            self.terminal_dialog.dstep_label_value.setText(f"{float(self.shared_state.get_var_d_step())}")
            self.terminal_dialog.gstep_lable_value.setText(f"{float(self.shared_state.get_var_g_step())}")
            self.terminal_dialog.vg_label_value.setText(f"{float(self.shared_state.var_vg_measure)}")
            self.terminal_dialog.vsd_label_value.setText(f"{float(self.shared_state.var_vsd_measure)}")
            self.terminal_dialog.seltemp_label_value.setText(f"{float(integrate.get_heater_temperature())}")

            if integrate.get_enable_bricklet():
                self.terminal_dialog.ttotal_label_value.setText(f"{float(self.calculate_ttotal)} + 8S")
            else:
                self.terminal_dialog.ttotal_label_value.setText(f"{float(self.calculate_ttotal)}")

        self.stacked_widget.addWidget(self.terminal_dialog)
        self.stacked_widget.setCurrentWidget(self.terminal_dialog)
        self.terminal_dialog.show()
        self.terminal_dialog.ok_button.setVisible(False)
        
        if integrate.get_t2() > 0:
            print("You cannot adjust the Nitrogen")
            self.terminal_dialog.argon_flow_edit.hide()
            self.terminal_dialog.argon_flow_label.hide()
            self.terminal_dialog.set_button.hide()
            self.Timer_status_signal.emit(f"{integrate.get_Count()} Sec")
        else:
            print("You can adjust the Nitrogen")
            self.terminal_dialog.argon_flow_edit.setText("")
            self.terminal_dialog.argon_flow_edit.show()
            self.terminal_dialog.argon_flow_label.show()
            self.terminal_dialog.set_button.show()
            self.terminal_dialog.argon_flow_edit.setEnabled(False)
            self.terminal_dialog.set_button.setEnabled(False)
            self.Timer_status_signal.emit(f"{integrate.get_Count()} Sec")

        if not self.shared_state.get_device_connection_state():
            QMetaObject.invokeMethod(self.terminal_dialog.cancel_button, "setEnabled", Qt.QueuedConnection, Q_ARG(bool, False))           
            self.status_label_style.emit("NO DEVICE IS CONNECTED", "#F71616")
            return
        else:
            QMetaObject.invokeMethod(self.terminal_dialog.cancel_button, "setEnabled", Qt.QueuedConnection, Q_ARG(bool, True))
            self.status_label_style.emit("<MT> MEASUREMENT IS GOING ON", "#FFFFFF")
            if not self.shared_state.get_current_test_code():
                self.status_label_style.emit("Test code value is not set", "#FFFFFF")
                msg = QMessageBox()
                msg.setIcon(QMessageBox.Information)
                msg.setText("Test code value is not set. Please go to the settings page and set the value.")
                msg.setWindowTitle("Info")
                msg.setStandardButtons(QMessageBox.Ok)
                msg.exec_()
                return
            
            #self.shared_state.set_continue_measurement(True)
            self.disable_ui()

    def _update_status_label_with_color(self, text: str, color: str):
        """Thread-safe way to update status label with color"""
        if self.terminal_dialog and hasattr(self.terminal_dialog, 'status_label'):
            self.terminal_dialog.status_label.setText(text)
            self.terminal_dialog.status_label.setStyleSheet(f"""
                QLabel {{
                    color: {color};
                    font-size: 18px;
                    font-family: 'Times New Roman';
                    background-color: #1D2B3A;
                    padding: 5px;
                    border-radius: 5px;
                }}
            """)

    def _set_argon_controls_enabled(self, enabled: bool):
        """Thread-safe enable/disable of Nitrogen flow controls"""
        if self.terminal_dialog:
            self.terminal_dialog.argon_flow_edit.setEnabled(enabled)
            self.terminal_dialog.set_button.setEnabled(enabled)
            style = """
                QLineEdit {
                    background-color: white;
                    color: black;
                    font-size: 16px;
                    border: 1px solid #3A4F63;
                    border-radius: 3px;
                    padding: 5px;
                }
            """ if enabled else """
                QLineEdit {
                    background-color: darkgray;
                    color: black;
                    font-size: 16px;
                    border: 1px solid #3A4F63;
                    border-radius: 3px;
                    padding: 5px;
                }
            """
            self.terminal_dialog.argon_flow_edit.setStyleSheet(style)
            
    def startAllThreads(self):
        self.devices = [
            ("Device 1", self.shared_state.device1_ip, "rawtext1"),
        ]
        print("Start all thread device1 status", self.shared_state.get_device_connection_state())
        enable_bricklet = self.shared_state.Enable_Bricklets
        integrate.set_enable_bricklet(enable_bricklet) 

        # Start Bricklet and Timer threads
        if not self.shared_state.BrickletThreadStarted:
            self.shared_state.BrickletThreadStarted = True
            print("BrickletThreadStarted measurement_screen.py")

            # Bricklet thread
            self.bricklet_thread = QThread()
            self.threads.append(self.bricklet_thread)
            self.bricklet_worker = BrickletWorker(self.main_window.screens["Measure"])
            self.bricklet_worker.moveToThread(self.bricklet_thread)
            self.bricklet_thread.started.connect(self.bricklet_worker.run)
            self.bricklet_worker.finished.connect(self.bricklet_thread.quit)
            self.bricklet_worker.finished.connect(self.bricklet_worker.deleteLater)
            self.bricklet_thread.finished.connect(self.bricklet_thread.deleteLater)
            self.bricklet_thread.start()

            time.sleep(0.5)
            # Timer thread
            self.timer_thread = QThread()
            self.threads.append(self.timer_thread)
            self.timer_worker = TimerWorker(self.main_window.screens["Measure"])
            self.timer_worker.moveToThread(self.timer_thread)
            self.timer_thread.started.connect(self.timer_worker.run)
            self.timer_worker.finished.connect(self.timer_thread.quit)
            self.timer_worker.finished.connect(self.timer_worker.deleteLater)
            self.timer_thread.finished.connect(self.timer_thread.deleteLater)
            self.timer_thread.start()
            time.sleep(0.5)

            # TemperatureMonitorWorker thread
            TempMonitor_thread = QThread()
            self.threads.append(TempMonitor_thread)
            TempMonitor_worker = TemperatureMonitorWorker(self.main_window.screens["Measure"])
            TempMonitor_worker.moveToThread(TempMonitor_thread)
            TempMonitor_thread.started.connect(TempMonitor_worker.run)
            TempMonitor_worker.finished.connect(TempMonitor_thread.quit)
            TempMonitor_worker.finished.connect(TempMonitor_worker.deleteLater)
            TempMonitor_thread.finished.connect(TempMonitor_thread.deleteLater)
            TempMonitor_thread.start()
            time.sleep(0.5)

        # Start Device 1 thread
        if self.shared_state.get_device_connection_state():
            print("Start all thread Starting DeviceMeasurementThread")
            
            device_thread = DeviceMeasurementThread(self.main_window.screens["Measure"], self.devices[0])
            device_thread.update_status.connect(self.update_status_signal.emit)
            device_thread.measurement_completed.connect(self.handle_device_completion)
            self.threads.append(device_thread)
            device_thread.start()
            time.sleep(0.5)

    def handle_device_completion(self, device, success):
        if success:
            self.completed_devices.add(device)
        else:
            self.failed_devices.add(device)
            self.measurement_complete_signal.emit(False)

    def close_terminal_dialog(self):
        if self.terminal_dialog:
            
            self.show_measurement_widgets()
            # Clean up threads
            for thread in self.threads:
                if thread.isRunning():
                    thread.quit()
                    thread.wait()
            self.threads.clear()

    def update_terminal(self, status_message, device, ip_address):
        if self.terminal_dialog and self.is_active:
            timestamp = time.strftime("%H:%M:%S", time.localtime())
            self.terminal_dialog.append_text(f"[{timestamp}] {device} ({ip_address}): {status_message}")
        else:
            self.status_messages.append((status_message, device, ip_address))

    def handle_measurement_completion(self, success):
        if self.terminal_dialog:
            self.terminal_dialog.measurement_completed(success)
            print("integrate.get_check_temperature()",integrate.get_check_temperature())            
            if not success:
                if not integrate.get_check_temperature() and integrate.get_enable_bricklet() and not integrate.get_Cancel_Measurement() and not self.shared_state.get_measurement_failed():
    
                    if integrate.get_Sweep_Measurement() or integrate.get_Normal_Measurement():
                        print("Temperature not reached in Advanced Settings Measurement")
                        self.shared_state.set_Cancel_event(True)
                        AdvancedSettingScreen = self.main_window.screens["AdvanceSettings"]
                        AdvancedSettingScreen.update_status_signal.emit("Measurement Failed: Temperature not reached", "red") 
                        AdvancedSettingScreen.enable_ui()                        
                    else:
                        print("Temperature not reached during Measurement")
                        self.status_label_style.emit("Measurement Failed: Temperature not reached", "#FF0000")    
                    
                elif not integrate.get_Cancel_Measurement() and self.shared_state.get_measurement_failed():       
                    self.status_label_style.emit("Measurement Failed", "#FF0000")       
                    QMetaObject.invokeMethod(self.terminal_dialog.cancel_button, "setEnabled", Qt.QueuedConnection, Q_ARG(bool, False))
   
                        
                # Uncheck the checkboxes on the Settings screen after measurement Failed
                #settings_screen = self.main_window.screens["Settings"]
                #settings_screen.reset_checkboxes(False)    

    def update_AdvancedSettings_measurmentstatus(self, Message,color):
        AdvancedSettingScreen = self.main_window.screens["AdvanceSettings"]
        AdvancedSettingScreen.update_status_signal.emit(Message, color)
        

        
    def get_app_path1(self):
        if getattr(sys, 'frozen', False):
            app_dir = os.path.dirname(sys.executable)
            if "dist" in app_dir:
                app_dir = app_dir[:app_dir.rfind("dist")].rstrip(os.sep)
            data_path = os.path.join(app_dir)
            return data_path
        else:
            app_dir = os.path.dirname(os.path.abspath(__file__))
            return app_dir

    def disable_ui(self):
        self.logout_button.setEnabled(False)
        self.main_window.toggle_sidebar(False)

    def enable_ui(self):
        self.logout_button.setEnabled(True)
        self.main_window.toggle_sidebar(True)

    def CalibrationScreen_GraphFunction(self, device, ip_address, filename):
        print(f"CalibrationScreen_GraphFunction {device} started")
        return
            
    def BrickletMeasurementGraphFunction(self, device, ip_address, filename):
        print(f"<MT> BrickletMeasurementGraphFunction {device} started")

        # Check the Current date Folder is present or not.
        self.check_date_and_update()           
        #self.shared_state.set_continue_measurement(True)
        # Step 1: Start pre-sweep measurement for Main Measurement
        if self.shared_state.EnablePresweep_Checkbox:
            self.status_label_style.emit("<MT> PRE-SWEEP MEASUREMENT IS GOING ON", "#FFFFFF")

            presweep_completed = self.Pre_Sweep_Measurment(device, ip_address)
            
            if not presweep_completed:
                print(f"<MT> Pre-Sweep Measurement failed for {device}")
                return False
            
            print(f"<MT> waiting pre sweep delay : {self.shared_state.Pre_sweep_delay} seconds")

            PreSweep_delay_time = time.time() + int(self.shared_state.Pre_sweep_delay)
            while time.time()  < PreSweep_delay_time:
                if integrate.get_Cancel_Measurement():
                    print("<MT> [Pre Sweep Delay] Stopped the Measurement due to user cancel request.")
                    if self.is_active:
                        self.update_status_signal.emit("<MT> [Pre Sweep Delay] Stopped the Measurement due to user cancel request.", device, ip_address)

                    return False
                time.sleep(0.5)

        self.status_label_style.emit("<MT> MEASUREMENT IS GOING ON", "#FFFFFF") 
        
        # Step 2: Perform DAC operations
        success = self.perform_dac_operations(device, ip_address, filename)
        if success:
            print("<MT> Perform_DAC Operation Completed")
        else:
            print("<MT> Perform DAC Failed operations")
            self.measurement_complete_signal.emit(False)
            return False

        # Step 3: Switch to sample graph
        while not integrate.get_view_graph():
            print(f"<MT> Waiting the Bricklet flag to view the graph {device}")
            time.sleep(1)
            if integrate.get_Cancel_Measurement():
                print("<MT>Stopped the Measurement due to cancel request.")
                if self.is_active:
                    self.update_status_signal.emit("<MT> Stopped the Measurement due to cancel request", device, ip_address)
                return False             
        
        
        self.measurement_complete_signal.emit(True)
        self.shared_state.set_continue_measurement(False)
        time.sleep(2)
        self.enable_ui_Signal.emit()
        self.switch_to_sample_screen.emit(False, "SAMPLE1", 1)  # Plot main measurement graph

    @staticmethod
    def print_current_time():
        timestamp = time.time()
        readable_time = datetime.fromtimestamp(timestamp)
        only_time = readable_time.strftime("%H:%M:%S")
        return only_time

    def AdvancedSettingsGraphFunction(self, device, ip_address, filename):
        print(f"AdvancedSettingsGraphFunction {device} started")

        # Check the Current date Folder is present or not.
        self.check_date_and_update()           
        #self.shared_state.set_continue_measurement(True)
        # Step 1: Start pre-sweep measurement for Normal Measurement
        if integrate.get_Normal_Measurement():
            AdvancedSettingScreen = self.main_window.screens["AdvanceSettings"]
            if self.shared_state.EnablePresweep_Checkbox:
                AdvancedSettingScreen.update_status_signal.emit("<MT> Pre-Sweep Measurement is going on", "yellow")
                presweep_completed = self.Pre_Sweep_Measurment(device, ip_address)
                AdvancedSettingScreen.update_status_signal.emit("<MT> Measurement is going on", "yellow")
            
                if not presweep_completed:
                    print(f"<MT> Advance Settings Screen  Pre-Sweep Measurement failed for {device}")
                    AdvancedSettingScreen.update_status_signal.emit("Pre-Sweep Measurement has Failed", "red")                    
                    return False

                print(f"waiting pre sweep delay : {self.shared_state.Pre_sweep_delay} seconds")
                time.sleep(int(self.shared_state.Pre_sweep_delay))

       
        # Step 2: Perform DAC operations
        success = self.perform_dac_operations(device, ip_address, filename)
        if success:
            print("<MT> Perform_DAC Operation Completed")
            # if bricklet checkbox is disabled on the Advanced setting Screen
            if not self.shared_state.get_EnableBricklets_Checkbox():
                integrate.set_view_graph1(True)
        else:
            print("<MT> Perform DAC Failed operations")
            self.measurement_complete_signal.emit(False)
            return False

        # Step 3: Switch to  graph Advanced Settings Screen
        while not integrate.get_view_graph1():
            print(f"<MT> Waiting the Bricklet flag to view the graph {device}")
            time.sleep(1)
            
            if integrate.get_Cancel_Measurement():
                print("<MT> Advancesettings: Stopped the Measurement due to cancel request.")
                return False
                
        
        self.shared_state.set_continue_measurement(False)
        self.advanced_settings_complete_signal.emit()




    def check_date_and_update(self):
        # Get today's folder name (only the date string)
        today_folder_name = datetime.now().strftime('%d_%m_%Y')

        # Build expected full folder path
        expected_path = os.path.join(self.base_path, today_folder_name)

        # If day changed? Create new folder & update path
        if expected_path != self.today_date_folder_path:
            print("New day detected. Updating folder...")

            # Update the variable
            self.today_date_folder_path = expected_path
            os.makedirs(self.today_date_folder_path, exist_ok=True)

            # Also create inside application data folder
            app_data_folder = os.path.join(self.get_app_path1(), 'data', today_folder_name)

            if not os.path.exists(app_data_folder):
                os.makedirs(app_data_folder)
                print(f"New date folder created: {app_data_folder}")

    def Pre_Sweep_Measurment(self, device, ip_address):
        """
        Handles the DAC updates and adjustments specific to Sweep Measurement mode.
        """
        try:
            # This one is OK if called from main thread, but to be safe:
            QMetaObject.invokeMethod(self.terminal_dialog.cancel_button, "setEnabled", Qt.QueuedConnection, Q_ARG(bool, True))      
            Vg_min = self.shared_state.var_vg_min
            Vg_max = self.shared_state.var_vg_max
            Vsd = self.shared_state.var_vsd
            g_step = self.shared_state.get_var_g_step()
            d_time = self.shared_state.cube.d_time
            d_step = self.shared_state.get_var_d_step()
            g_step = self.shared_state.get_var_g_step()
            do_double_sweep = True  # Assuming double sweep is required; adjust if needed
            date_str = datetime.now().strftime('%d_%m_%Y')
            timestamp = self.shared_state.get_timestamp()

            # Calculate sampling time
            x_axis_time = (0.001 * d_time) * ((Vg_max - Vg_min) / g_step)
            sampling_time = x_axis_time 
            print(f'<MT> Sampling time: {sampling_time} s')
            
            if self.is_active:
                self.update_status_signal.emit(f"<MT> Sampling time: {sampling_time} s", device, ip_address)

            if self.is_active:
                self.update_status_signal.emit("<MT> Starting Pre Sweep Measurement...", device, ip_address)

            # Set dwell time
            if self.is_active:
                self.update_status_signal.emit(f"<MT> Setting dwell time to {d_time}", device, ip_address)

            print(f"<MT> Setting dwell time to {d_time}")
            rc = self.shared_state.cube.SetDwell_Time_Value(ip_address, d_time)
            if not rc:
                if self.is_active:
                    self.update_status_signal.emit("Failed to set dwell time", device, ip_address)
                print(f"<MT> Failed to set dwell time for {device}")
                self.shared_state.set_failed_status("<MT> Measurement PreSweep : Failed to set dwell time <Network Failure>")
                self.shared_state.set_measurement_failed(True)
                return False
            
            if self.is_active:
                self.update_status_signal.emit("<MT> Dwell time set successfully", device, ip_address)
            
            print("<MT> Dwell time set successfully")
            
            # Set d_step0
            if self.is_active:
                self.update_status_signal.emit(f"<MT> Setting d_step0 to {d_step}", device, ip_address)
            print(f"<MT> Setting d_step0 to {d_step}")
            rc = self.shared_state.cube.Setd_step0Value(ip_address, d_step)
            if not rc:
                if self.is_active:
                    self.update_status_signal.emit("<MT> Failed to set d_step0", device, ip_address)
                print(f"<MT> Failed to set d_step0 for {device}")
                self.shared_state.set_failed_status("<MT> Measurement PreSweep : Failed to set d_step0 <Network Failure>")
                self.shared_state.set_measurement_failed(True)
                return False
            
            if self.is_active:
                self.update_status_signal.emit("<MT> d_step0 set successfully", device, ip_address)
            print("<MT> d_step0 set successfully")

            # Set g_step0
            if self.is_active:
                self.update_status_signal.emit(f"<MT> Setting g_step0 to {g_step}", device, ip_address)
            print(f"<MT> Setting g_step0 to {g_step}")
            rc = self.shared_state.cube.Setg_step0Value(ip_address, g_step)
            if not rc:
                if self.is_active:
                    self.update_status_signal.emit("<MT> Failed to set g_step0", device, ip_address)
                print(f"<MT> Failed to set g_step0 for {device}")
                self.shared_state.set_failed_status("<MT> Measurement PreSweep : Failed to set g_step0 <Network Failure>")
                self.shared_state.set_measurement_failed(True)
                return False
            if self.is_active:
                self.update_status_signal.emit("<MT> g_step0 set successfully", device, ip_address)
            print("<MT> g_step0 set successfully")
            # Apply DAC channels
            if self.is_active:
                self.update_status_signal.emit("<MT> Applying DAC channel values...", device, ip_address)
            print("<MT> Applying DAC channel values...")
            rc = self.shared_state.cube.ApplyDACChannels(ip_address)
            if not rc:
                if self.is_active:
                    self.update_status_signal.emit("<MT> Failed to update DACs", device, ip_address)
                print(f"<MT> Failed to update DACs for {device}")
                self.shared_state.set_failed_status("<MT> Measurement PreSweep : Failed to update DACs <Network Failure>")
                self.shared_state.set_measurement_failed(True)
                return False
            if self.is_active:
                self.update_status_signal.emit("<MT> DAC values applied successfully", device, ip_address)
            print("<MT> DAC values applied successfully")
            
            if integrate.get_Cancel_Measurement():
                self.update_status_signal.emit("<MT> Canelling the Measurement as per cancel request", device, ip_address)
                return False             
            
            # Set Vsd for channels 1-16
            if self.is_active:
                self.update_status_signal.emit(f"<MT> Setting all Vsd to 1-16 channels to {Vsd} V", device, ip_address)
            print(f"<MT> Setting all Vsd to 1-16 channels to {Vsd} V")
            #failed_channels = []
            for dac_channel in range(0, 16):
                if integrate.get_Cancel_Measurement():
                    print("<MT> [Pre Sweep measurment] Stopped the Measurement due to cancel request.")
                    self.update_status_signal.emit("<MT> [Pre Sweep measurment] Stopped the Measurement due to cancel request.", device, ip_address)
                    return False

                rc = self.shared_state.cube.SetDAC1ChannelValue(ip_address, dac_channel + 1, float(Vsd))
                if not rc:
                    if self.is_active:
                        self.update_status_signal.emit(f"<MT> Failed for DAC1 channel {dac_channel + 1}, value {Vsd}", device, ip_address)
                    self.shared_state.set_failed_status("<MT> Measurement PreSweep : Set DAC1 Channel Value Failed  <Network Failure>")
                    self.shared_state.set_measurement_failed(True)
                    print(f"<MT> Failed for DAC1 channel {dac_channel + 1}, value {Vsd}")
                    return False

                rc = self.shared_state.cube.SetDAC2ChannelValue(ip_address, dac_channel + 1, float(Vsd))
                if not rc:
                    if self.is_active:
                        self.update_status_signal.emit(f"<MT> Failed for DAC2 channel {dac_channel + 1}, value {Vsd}", device, ip_address)
                    print(f"<MT> Failed for DAC2 channel {dac_channel + 1}, value {Vsd}")
                    self.shared_state.set_failed_status("<MT> Measurement PreSweep : Set DAC2 Channel Value Failed <Network Failure>")
                    self.shared_state.set_measurement_failed(True)
                    return False

            # Apply DAC channels after setting Vsd
            rc = self.shared_state.cube.ApplyDACChannels(ip_address)
            if not rc:
                if self.is_active:
                    self.update_status_signal.emit("<MT> Failed to update DACs after setting Vsd", device, ip_address)
                print(f"Failed to update DACs after setting Vsd for {device}")
                self.shared_state.set_failed_status("<MT> Measurement PreSweep : Apply DAC Channels Failed <Network Failure>")
                self.shared_state.set_measurement_failed(True)
                return False
            
            dac1_value = self.shared_state.cube.GetDAC1ChannelValue(ip_address, 1)
            if self.is_active:
                self.update_status_signal.emit(f"DAC1 = {dac1_value}", device, ip_address)
            print(f"DAC1 = {dac1_value}")
            dac2_value = self.shared_state.cube.GetDAC2ChannelValue(ip_address, 1)
            if self.is_active:
                self.update_status_signal.emit(f"DAC2 = {dac2_value}", device, ip_address)
            print(f"DAC2 = {dac2_value}")

            # Set Vg_min for channels 16-32
            if self.is_active:
                self.update_status_signal.emit(f"Setting all Vg to 16-32 channels to {Vg_min} V", device, ip_address)
            print(f"Setting all Vg to 16-32 channels to {Vg_min} V")
            for dac_channel in range(16, 32):
                if integrate.get_Cancel_Measurement():
                    print("[Pre Sweep measurment] Stopped the Measurement due to user cancel request.")
                    self.update_status_signal.emit("[Pre Sweep measurment] Stopped the Measurement due to user cancel request.", device, ip_address)
                    return False

                rc = self.shared_state.cube.SetDAC1ChannelValue(ip_address, dac_channel + 1, float(Vg_min))
                if not rc:
                    if self.is_active:
                        self.update_status_signal.emit(f"Failed for DAC1 channel {dac_channel + 1}, value {Vg_min}", device, ip_address)
                    print(f"Failed for DAC1 channel {dac_channel + 1}, value {Vg_min}")
                    self.shared_state.set_failed_status("<MT> Measurement PreSweep : Set DAC1 Channel Value Failed <Network Failure>")
                    self.shared_state.set_measurement_failed(True)
                    return False
                    
                rc = self.shared_state.cube.SetDAC2ChannelValue(ip_address, dac_channel + 1, float(Vg_min))
                if not rc:
                    if self.is_active:
                        self.update_status_signal.emit(f"Failed for DAC2 channel {dac_channel + 1}, value {Vg_min}", device, ip_address)
                    print(f"Failed for DAC2 channel {dac_channel + 1}, value {Vg_min}")
                    self.shared_state.set_failed_status("<MT> Measurement PreSweep : Set DAC2 Channel Value Failed <Network Failure>")
                    self.shared_state.set_measurement_failed(True)
                    return False

            # Apply DAC channels after setting Vg_min
            rc = self.shared_state.cube.ApplyDACChannels(ip_address)
            if not rc:
                if self.is_active:
                    self.update_status_signal.emit("Failed to update DACs after setting Vg_min", device, ip_address)
                print(f"Failed to update DACs after setting Vg_min for {device}")
                self.shared_state.set_failed_status("<MT> Measurement PreSweep : Apply DAC Channels Failed <Network Failure>")
                self.shared_state.set_measurement_failed(True)
                return False
            dac1_value = self.shared_state.cube.GetDAC1ChannelValue(ip_address, 17)
            if self.is_active:
                self.update_status_signal.emit(f"<MT> DAC1 = {dac1_value}", device, ip_address)
            print(f"<MT> DAC1 = {dac1_value}")
            dac2_value = self.shared_state.cube.GetDAC2ChannelValue(ip_address, 17)
            if self.is_active:
                self.update_status_signal.emit(f"<MT> DAC2 = {dac2_value}", device, ip_address)
            print(f"<MT> DAC2 = {dac2_value}")

            # Enable Bricklets if selected
            if self.shared_state.get_EnableBricklets_Checkbox():
                integrate.set_Start_Bricketlet_T0_flow(True)
                print("<MT> integrate.Start_Bricketlet_T0_flow sweep Measurement", integrate.get_Start_Bricketlet_T0_flow())


            if integrate.get_Cancel_Measurement():
                self.update_status_signal.emit("<MT> Stopped the Measurement due to user cancel request.", device, ip_address)
                return False 

            # Start Data Acquisition
            if self.is_active:
                self.update_status_signal.emit("<MT> Pre-Sweep Starting Data Acquisition...", device, ip_address)
            now = datetime.now()
            print(f"<MT> {now.strftime('%Y-%m-%d %H:%M:%S')} Pre-Sweep Starting Data Acquisition")
            print("<MT> Pre-Sweep Sampling Time", (2 * sampling_time if do_double_sweep else sampling_time) + 3)
            rc = self.shared_state.cube.StartDataAcq(ip_address, (2 * sampling_time if do_double_sweep else sampling_time) + 10)
            if not rc:
                if self.is_active:
                    self.update_status_signal.emit("<MT> Pre-Sweep Failed to start Data Acquisition", device, ip_address)
                print(f"<MT> Pre-Sweep Failed to start Data Acquisition for {device}")
                self.shared_state.set_failed_status("<MT> Measurement PreSweep : Start Data Acquisition Failed <Network Failure> ")
                self.shared_state.set_measurement_failed(True)    
                return False
            t0 = time.time()
            #if self.is_active:
            #    self.update_status_signal.emit("<MT> Pre-Sweep Data acquisition started successfully.", device, ip_address)
            #print("<MT> Pre-Sweep Data acquisition started successfully.")
            
            if integrate.get_Cancel_Measurement():
                self.update_status_signal.emit(" <MT> Pre-Sweep Aborting the Measurement due to user cancel request.", device, ip_address)
                self.shared_state.cube.AbortMeasurement(ip_address)
                return False 

            # Update to Vg_max for channels 16-32
            #if self.is_active:
            #    self.update_status_signal.emit(f"<MT> Updating dac_value to {Vg_max} V", device, ip_address)
            #print(f"Updating dac_value to {Vg_max} V")
            for dac_channel in range(16, 32):
                rc = self.shared_state.cube.SetDAC1ChannelValue(ip_address, dac_channel + 1, float(Vg_max))
                if not rc:
                    if self.is_active:
                        self.update_status_signal.emit(f"<MT> Failed for DAC1 channel {dac_channel + 1}, value {Vg_max}", device, ip_address)
                    print(f"<MT> Failed for DAC1 channel {dac_channel + 1}, value {Vg_max}")
                    self.shared_state.set_failed_status("<MT> Measurement PreSweep : Set DAC1 Channel Value Failed <Network Failure>")
                    self.shared_state.set_measurement_failed(True)    
                    return False 
                    
                if integrate.get_Cancel_Measurement():
                    self.update_status_signal.emit("<MT> Stopped the Measurement due to cancel request.", device, ip_address)
                    self.shared_state.cube.AbortMeasurement(ip_address)
                    return False 
                    
                rc = self.shared_state.cube.SetDAC2ChannelValue(ip_address, dac_channel + 1, float(Vg_max))
                if not rc:
                    if self.is_active:
                        self.update_status_signal.emit(f"<MT> Failed for DAC2 channel {dac_channel + 1}, value {Vg_max}", device, ip_address)
                    print(f"<MT> Failed for DAC2 channel {dac_channel + 1}, value {Vg_max}")
                    self.shared_state.set_failed_status("<MT> Measurement PreSweep : Set DAC2 Channel Value Failed <Network Failure>")
                    self.shared_state.set_measurement_failed(True)    
                    return False 

            # Apply DAC channels after setting Vg_max
            rc = self.shared_state.cube.ApplyDACChannels(ip_address)
            if not rc:
                if self.is_active:
                    self.update_status_signal.emit("<MT> Failed to update DACs", device, ip_address)
                print(f"<MT> Failed to update DACs for {device}")
                self.shared_state.set_failed_status("<MT> Measurement PreSweep : Apply DAC Channels Failed <Network Failure>")    
                self.shared_state.set_measurement_failed(True)
                return False

            now = datetime.now()
            print(f"<MT> {now.strftime('%Y-%m-%d %H:%M:%S')} Pre-Sweep Starting Data Acquisition Before Wait 1")
            # Wait for sampling time
            while (time.time() < t0 + sampling_time):
                if integrate.get_Cancel_Measurement():
                    self.update_status_signal.emit("<MT> Stopped the Measurement due to user cancel request.", device, ip_address)
                    self.shared_state.cube.AbortMeasurement(ip_address)
                    return False 
                time.sleep(0.1)
            now = datetime.now()
            print(f"<MT> {now.strftime('%Y-%m-%d %H:%M:%S')} Pre-Sweep Starting Data Acquisition After Wait 1")

            # Check DAC values after ramp up
            #dac1_value = self.shared_state.cube.GetDAC1ChannelValue(ip_address, 1)
            #if self.is_active:
            #    self.update_status_signal.emit(f"<MT> DAC1 after ramp up = {dac1_value}", device, ip_address)
            #print(f"<MT> DAC1 after ramp up = {dac1_value}")
            #dac2_value = self.shared_state.cube.GetDAC2ChannelValue(ip_address, 1)
            #if self.is_active:
            #    self.update_status_signal.emit(f"<MT> DAC2 after ramp up = {dac2_value}", device, ip_address)
            #print(f"<MT> DAC2 after ramp up = {dac2_value}")
            
            
            # Perform double sweep if enabled
            if do_double_sweep:
                #if self.is_active:
                #    self.update_status_signal.emit(f"<MT> Setting all Vg to 16-32 channels to {Vg_min} V for double sweep", device, ip_address)
                #print(f"<MT> Setting all Vg to 16-32 channels to {Vg_min} V for double sweep")
                for dac_channel in range(16, 32):
                    
                    if integrate.get_Cancel_Measurement():
                        self.update_status_signal.emit("<MT> Stopped the Measurement due to user cancel request. [16-32] DAC1", device, ip_address)
                        self.shared_state.cube.AbortMeasurement(ip_address)
                        return False 

                    rc = self.shared_state.cube.SetDAC1ChannelValue(ip_address, dac_channel + 1, float(Vg_min))
                    if not rc:
                        if self.is_active:
                            self.update_status_signal.emit(f"<MT> Failed for DAC1 channel {dac_channel + 1}, value {Vg_min}", device, ip_address)
                        print(f"<MT> Failed for DAC1 channel {dac_channel + 1}, value {Vg_min}")
                        self.shared_state.set_failed_status("<MT> Measurement PreSweep : Set DAC1 Channel Value Failed <Network Failure>")
                        self.shared_state.set_measurement_failed(True)    
                        return False 

                    if integrate.get_Cancel_Measurement():
                        self.update_status_signal.emit("<MT> Stopped the Measurement due to user cancel request. [16-32] DAC2", device, ip_address)
                        self.shared_state.cube.AbortMeasurement(ip_address)
                        return False 
                    rc = self.shared_state.cube.SetDAC2ChannelValue(ip_address, dac_channel + 1, float(Vg_min))
                    if not rc:
                        if self.is_active:
                            self.update_status_signal.emit(f"<MT> Failed for DAC2 channel {dac_channel + 1}, value {Vg_min}", device, ip_address)
                        print(f"<MT> Failed for DAC2 channel {dac_channel + 1}, value {Vg_min}")
                        self.shared_state.set_failed_status("<MT> Measurement PreSweep : Set DAC2 Channel Value Failed <Network Failure>")
                        self.shared_state.set_measurement_failed(True)    
                        return False 


                # Apply DAC channels after double sweep
                rc = self.shared_state.cube.ApplyDACChannels(ip_address)
                if not rc:
                    if self.is_active:
                        self.update_status_signal.emit("<MT> Failed to update DACs for double sweep", device, ip_address)
                    print(f"<MT> Failed to update DACs for double sweep for {device}")
                    self.shared_state.set_failed_status("<MT> Measurement PreSweep : Apply DAC Channels Failed <Network Failure>")
                    self.shared_state.set_measurement_failed(True)
                    return False

                if integrate.get_Cancel_Measurement():
                    self.update_status_signal.emit("<MT> Stopped the Measurement due to user cancel request. Pre-Sweep Apply DAC1/2", device, ip_address)
                    self.shared_state.cube.AbortMeasurement(ip_address)
                    return False 
                now = datetime.now()
                print(f"<MT> {now.strftime('%Y-%m-%d %H:%M:%S')} Pre-Sweep Starting Data Acquisition Before Wait 2")
                # Wait for second sampling time
                while (time.time() < t0 + 2 * sampling_time) and not self.shared_state.should_stop:
                    if integrate.get_Cancel_Measurement():
                        self.update_status_signal.emit("<MT> Stopped the Measurement due to user cancel request.", device, ip_address)
                        self.shared_state.cube.AbortMeasurement(ip_address)
                        return False 
                    time.sleep(0.1)
                now = datetime.now()
                print(f"<MT> {now.strftime('%Y-%m-%d %H:%M:%S')} Pre-Sweep Starting Data Acquisition After Wait 2")

            # Abort Measurement
            if self.is_active:
                self.update_status_signal.emit("<MT> Aborting Measurement...", device, ip_address)
            print("<MT> Aborting Measurement...")
            rc = self.shared_state.cube.AbortMeasurement(ip_address)
            if not rc:
                if self.is_active:
                    self.update_status_signal.emit("<MT> 4. Pre-Sweep Failed to Abort Measurement", device, ip_address)
                print(f"<MT> 4. Pre-Sweep Failed to Abort Measurement for {device}")
                self.shared_state.set_failed_status("<MT> Measurement PreSweep : 4. Abort Measurement Failed <Network Failure>")
                self.shared_state.set_measurement_failed(True)    
                return False 
            now = datetime.now()
            print(f"<MT> {now.strftime('%Y-%m-%d %H:%M:%S')} Pre-Sweep Starting Data Acquisition = Abort End")

            #self.terminal_dialog.cancel_button.setEnabled(False)
            QMetaObject.invokeMethod(self.terminal_dialog.cancel_button, "setEnabled", Qt.QueuedConnection, Q_ARG(bool, False))

            # Transfer raw file
            if self.is_active:
                self.update_status_signal.emit("<MT> Transferring Raw file...", device, ip_address)
            print("<MT> Transferring Raw file...")
            output_file = f"SM_{Vg_min}_{Vg_max}_{timestamp}"
            output_file += "_rawtext1"

            if self.is_active:
                self.update_status_signal.emit(f"Output file: {output_file}", device, ip_address)
            print(f"Output file: {output_file}")
            remote_path = os.path.join('data', date_str, output_file)
            local_file_path = os.path.join(self.base_path, date_str, output_file)

            if self.is_active:
                self.update_status_signal.emit(f"<MT> Current working directory: {os.getcwd()}", device, ip_address)
                self.update_status_signal.emit(f"<MT> Expected local file path: {local_file_path}", device, ip_address)
            print(f"<MT> Current working directory: {os.getcwd()}")
            print(f"<MT> Expected local file path: {local_file_path}")

            rc = self.shared_state.cube.StartTransferMeasurementFile(ip_address, remote_path)
            if not rc:
                if self.is_active:
                    self.update_status_signal.emit(f"Failed to transfer measurement file {remote_path} from Cube device", device, ip_address)
                print(f"Failed to transfer measurement file {remote_path} from Cube device")
                self.shared_state.set_failed_status("<MT> Measurement PreSweep : Transfer Measurement File Failed <Network Failure>")
                self.shared_state.set_measurement_failed(True) 
                return False


            if self.is_active:
                self.update_status_signal.emit("<MT> Pre Sweep Measurement setup completed.", device, ip_address)
            print("<MT> Pre Sweep Measurement setup completed.")
            return True

        except Exception as e:
            print(f"[ERROR] <MT> Pre-Sweep Measurement failed for {device}: {e}")
            if self.is_active:
                self.update_status_signal.emit(f"<MT> Pre-Sweep Measurement error: {e}", device, ip_address)
            
            self.shared_state.set_failed_status("<MT> Pre-Sweep Measurement Error : <Netwwork Error>")
            self.shared_state.set_measurement_failed(True) 
            return False

    def generate_timestamped_filename(self, filename, device):
        """Generate timestamped filename based on measurement type."""
        timestamp = self.shared_state.get_timestamp()
        if filename == "rawtext1":
            print(f"[DEBUG] Generating rawtext1 filename for {device}")
            if integrate.get_Sweep_Measurement():
                Vg_min = self.shared_state.var_vg_min
                Vg_max = self.shared_state.var_vg_max
                return f"SM_{Vg_min}_{Vg_max}_rawtext1_{timestamp}"
            elif integrate.get_Calibrate_Measurement():
                return f"CM_0_{self.shared_state.calib_time}_rawtext1_{timestamp}"
            elif integrate.get_Normal_Measurement():
                return f"AM_0_{self.shared_state.var_time}_rawtext1_{timestamp}"
            else:
                if integrate.get_enable_bricklet():
                    self.Ttotal = int(self.shared_state.get_T1() + self.shared_state.get_T2() + self.shared_state.get_T3()+8)
                else:
                    self.Ttotal = int(self.shared_state.get_T1() + self.shared_state.get_T2() + self.shared_state.get_T3())  
                return f"MM_0_{self.Ttotal}_rawtext1_{timestamp}"
        elif filename == "rawtext2":
            print(f"[DEBUG] Generating rawtext2 filename for {device}")
            if integrate.get_Sweep_Measurement():
                Vg_min = self.shared_state.var_vg_min
                Vg_max = self.shared_state.var_vg_max
                return f"SM_{Vg_min}_{Vg_max}_rawtext2_{timestamp}"
            elif integrate.get_Calibrate_Measurement():
                return f"CM_0_{self.shared_state.calib_time}_rawtext2_{timestamp}"
            elif integrate.get_Normal_Measurement():
                return f"AM_0_{self.shared_state.var_time}_rawtext2_{timestamp}"
            else:
                self.Ttotal = int(self.shared_state.get_T1() + self.shared_state.get_T2() + self.shared_state.get_T3())
                return f"MM_0_{self.Ttotal}_rawtext2_{timestamp}"
        else:
            print(f"Generating default filename for {device}")
            return f"{filename}_{timestamp}"

    def perform_dac_operations(self, device, ip_address, filename):
        try:
            #self.terminal_dialog.cancel_button.setEnabled(True)
            QMetaObject.invokeMethod(self.terminal_dialog.cancel_button, "setEnabled", Qt.QueuedConnection, Q_ARG(bool, True))

            now = datetime.now()
            print(f"<MT> {now.strftime('%Y-%m-%d %H:%M:%S')} Starting perform_dac_operations for {device} ({ip_address})")

            # Generate timestamped filename
            timestamped_filename = self.generate_timestamped_filename(filename, device)
            rawtext_file_path = os.path.join(self.today_date_folder_path, timestamped_filename)
            print(f"Generated timestamped filename: {timestamped_filename}")

            # STORE IT SO WE CAN EMIT LATER
            self.shared_state.last_rawtext_file_path = rawtext_file_path

            # Initialize temperature logging for main measurement
            temp_log_file = None
            if not integrate.get_Calibrate_Measurement() and self.shared_state.Enable_Bricklets:
                if integrate.get_mainmeasurement() or integrate.get_Sweep_Measurement() or integrate.get_Normal_Measurement():
                    temp_log_file = self.rename_temperature_log_file(self.base_path, self.date_str, timestamped_filename)
                    print(f"<MT> Temperature log file path: {temp_log_file}")
                    if temp_log_file:
                        print(f"<MT> Temperature log file created at: {temp_log_file}")
                        integrate.set_temperature_log_filepath(temp_log_file)
                        integrate.set_temperature_log_active(True)
                        print(f"<MT> Temperature logging started at: {temp_log_file}")
                        if self.is_active:
                            self.update_status_signal.emit(f"<MT> Temperature logging started at: {temp_log_file}", device, ip_address)


            """
            Set DWELL, DSTEP, GSTEP
            """
            self.shared_state.set_t_dwell(self.shared_state.get_var_t_dwell())
            print(f"<MT> var_t_dwell Value : {self.shared_state.get_var_t_dwell()}")
            rc = self.shared_state.cube.SetDwell_Time_Value(ip_address, self.shared_state.get_t_dwell())
            if not rc:
                print(f"Failed to set dwell time for {device}")
                if self.is_active:
                    print(f"<MT> Emitting update_status_signal: Failed to set dwell time for {device}")
                    self.update_status_signal.emit("Failed to set dwell time.", device, ip_address)
                self.shared_state.set_failed_status("<MT> Failed to set dwell time <Network Failure>  ")
                self.shared_state.set_measurement_failed(True)
                return False
            else:
                print(f"<MT> Dwell time set successfully for {device}")
                if self.is_active:
                    print(f"<MT> Emitting update_status_signal: Dwell time set successfully for {device}")
                    self.update_status_signal.emit("Dwell time set successfully.", device, ip_address)

            if integrate.get_Cancel_Measurement():
                self.update_status_signal.emit("<MT> 1. PDO Stopped the Measurement due to cancel request.", device, ip_address)
                return False

            self.shared_state.set_d_step(self.shared_state.get_var_d_step())
            print(f"<MT> var_d_step Value : {self.shared_state.get_var_d_step()}")
            rc = self.shared_state.cube.Setd_step0Value(ip_address, self.shared_state.get_d_step())
            if not rc:
                print(f"<MT> Failed to set d_step0 for {device}")
                if self.is_active:
                    print(f"<MT> Emitting update_status_signal: Failed to set d_step0 for {device}")
                    self.update_status_signal.emit("Failed to set d_step0.", device, ip_address)
                self.shared_state.set_failed_status("<MT> Failed to set d_step0 <Network Failure>  ")
                self.shared_state.set_measurement_failed(True)
                return False
            else:
                print(f"<MT> d_step0 set successfully for {device}")
                if self.is_active:
                    print(f"<MT> Emitting update_status_signal: d_step0 set successfully for {device}")
                    self.update_status_signal.emit("d_step0 set successfully.", device, ip_address)

            if integrate.get_Cancel_Measurement():
                self.update_status_signal.emit("<MT> 2. PDO Stopped the Measurement due to cancel request.", device, ip_address)
                return False

            self.shared_state.set_g_step(self.shared_state.get_var_g_step())
            print(f"<MT> G Step Value : {self.shared_state.get_var_g_step()}")
            rc = self.shared_state.cube.Setg_step0Value(ip_address, self.shared_state.get_var_g_step())
            if not rc:
                print(f"<MT> Failed to set g_step0 for {device}")
                if self.is_active:
                    print(f"<MT> Emitting update_status_signal: Failed to set g_step0 for {device}")
                    self.update_status_signal.emit("<MT> Failed to set g_step0.", device, ip_address)
                self.shared_state.set_failed_status("<MT> Failed to set g_step0 <Network Failure>  ")
                self.shared_state.set_measurement_failed(True)
                return False
            else:
                print(f"<MT> g_step0 set successfully for {device}")
                if self.is_active:
                    print(f"<MT> Emitting update_status_signal: g_step0 set successfully for {device}")
                    self.update_status_signal.emit("<MT> g_step0 set successfully.", device, ip_address)

            if integrate.get_Cancel_Measurement():
                self.update_status_signal.emit("<MT> 3. PDO Stopped the Measurement due to cancel request.", device, ip_address)
                return False

            rc = self.shared_state.cube.ApplyDACChannels(ip_address)
            if not rc:
                print(f"<MT> Failed to update DACs for {device}")
                if self.is_active:
                    print(f"<MT> Emitting update_status_signal: Failed to update DACs for {device}")
                    self.update_status_signal.emit("<MT> Failed to update DACs. <Network Failure>  ", device, ip_address)
                self.shared_state.set_failed_status("<MT> Failed to update DACs")
                self.shared_state.set_measurement_failed(True)
                return False
            else:
                print(f"<MT> DAC values applied successfully for {device}")
                if self.is_active:
                    print(f"<MT> Emitting update_status_signal: DAC values applied successfully for {device}")
                    self.update_status_signal.emit("<MT> DAC values applied successfully.", device, ip_address)

            """
            DAC Configuration
            """
            main_measurement_dac_VSD_value = None
            main_measurement_dac_Vg_value = None
            now = datetime.now()
            print(f"<MT> {now.strftime('%Y-%m-%d %H:%M:%S')} After g_step {device}")

            # MAIN MEASUREMENT DAC CALIBRATION
            main_measurement_dac_VSD_value = None  
            main_measurement_dac_Vg_value = None          
            if integrate.get_mainmeasurement() or integrate.get_Sweep_Measurement() or integrate.get_Normal_Measurement():                
                main_measurement_dac_VSD_value = [0.0] * 32
                main_measurement_dac_Vg_value = [0.0] * 32
                # -------------------------------
                # Locate calibration folder  (14/01/2026)
                # -------------------------------    
                #calib_folder = os.path.join('data', self.date_str, 'calibrationData')
                # Get absolute base directory of the running script
                BASE_DIR = os.path.dirname(os.path.abspath(__file__))
                calib_folder = os.path.join('data', self.date_str, 'calibrationData')    
                os.makedirs(calib_folder, exist_ok=True)
                # ======================================================
                # CHECK Vg_Vsd_2set.txt FILE
                # ======================================================
                vg_vsd_file = os.path.join(calib_folder, "Vg_Vsd_2set.txt")                
                
                if not os.path.isfile(vg_vsd_file):
                    print("<MT> Vg_Vsd_2set.txt not found → using default Vsd measure value")
                    default_vsd = float(self.shared_state.var_vsd_measure)
                    default_vg = float(self.shared_state.var_vg_measure)
                    main_measurement_dac_VSD_value = [default_vsd] * 32
                    main_measurement_dac_Vg_value = [default_vg] * 32
                    self.update_status_signal.emit(f"<MT> No calibration file found, using default Vsd = {default_vsd} & Vg = {default_vg} values", device, ip_address)
                else:
                    if self.shared_state.get_Vg_Vsd_calibration_mode() or not integrate.get_Sweep_Measurement() or not integrate.get_Normal_Measurement():
                        print(f"<MT> Calibration file found: Applying {vg_vsd_file} Calibration")
                        self.update_status_signal.emit(f"<MT> Calibration file found: Applying {vg_vsd_file} Calibration", device, ip_address)

                        # ======================================================
                        # READ DAC VALUE FROM Vg_Vsd_2set.txt
                        # ======================================================
                        with open(vg_vsd_file, "r") as f:
                            for line in f:
                                # Remove leading/trailing spaces and newline
                                line = line.strip()

                                # Skip empty lines, separator lines (----), and header lines ("Device")
                                if not line or line.startswith("-") or line.startswith("Device"):
                                    continue
                                
                                # We need at least 2 useful columns (channel number and VSD value)
                                # If line is malformed, ignore it
                                cols = [c.strip() for c in line.split("|")]
                                if len(cols) < 3:
                                    continue

                                try:
                                    # First column -> Device / Channel number (1 to 32)
                                    channelno = int(cols[0])     # 1…32
                                    # Second column -> VSD DAC value
                                    vsd_val = float(cols[1])    # Vsd                                
                                    print("vsd_val:", vsd_val)                                
                                    Vg_val = float(cols[2])     # Vg
                                    print("Vg_val:", Vg_val)
                                except:
                                    continue
                                
                                # Store the VSD value only if channel is valid (1 to 32)        
                                if 1 <= channelno <= 32:
                                    main_measurement_dac_VSD_value[channelno - 1] = vsd_val
                                    
                                # Store the Vg value only if channel is valid (1 to 32)        
                                if 1 <= channelno <= 32:
                                    main_measurement_dac_Vg_value[channelno - 1] = Vg_val

                        # order = (0, 2, 4, 6, 8, 10, 12, 14,1, 3, 5, 7, 9, 11, 13, 15,16, 18, 20, 22, 24, 26, 28, 30,17, 19, 21, 23, 25, 27, 29, 31)
                        # rearranged_main_measurement_dac_Vg_value = [main_measurement_dac_Vg_value[index] for index in order]
                        # main_measurement_dac_Vg_value = rearranged_main_measurement_dac_Vg_value

                        # rearranged_main_measurement_dac_VSD_value = [main_measurement_dac_VSD_value[index] for index in order]
                        # main_measurement_dac_VSD_value = rearranged_main_measurement_dac_VSD_value

                        print("<MT> Loaded VSD values:")
                        for i in range(32):
                            print(f"   Device {i+1:02d} = {main_measurement_dac_VSD_value[i]}")
                            
                        print("<MT> Loaded Vg values:")
                        for i in range(32):
                            print(f"   Device {i+1:02d} = {main_measurement_dac_Vg_value[i]}")   

                    else:
                        default_vsd = float(self.shared_state.var_vsd_measure)
                        default_vg = float(self.shared_state.var_vg_measure)
                        main_measurement_dac_VSD_value = [default_vsd] * 32
                        main_measurement_dac_Vg_value = [default_vg] * 32
                        self.update_status_signal.emit(f"<MT> Applying Advanced Settings default Vsd = {default_vsd} & Vg = {default_vg} values", device, ip_address)

                if integrate.get_Sweep_Measurement():
                    dac_value = self.shared_state.var_vsd
                    print(f"<MT> SetDAC1ChannelValue Measurement dac_value [0:16] Vsd <Sweep> = {dac_value}")
                else:
                    dac_value = self.shared_state.var_vsd_measure
                    print(f"<MT> SetDAC1ChannelValue Measurement dac_value[0:16] VSD = {dac_value}")
                                             
            elif integrate.get_Sweep_Measurement():
                dac_value = self.shared_state.var_vsd
                print(f"<MT> SetDAC1ChannelValue Measurement dac_value [0:16] Vsd <Sweep> = {dac_value}")
            else:
                dac_value = self.shared_state.var_vsd_measure
                print(f"<MT> SetDAC1ChannelValue Measurement dac_value[0:16] VSD = {dac_value}")

            Calibrate_Screen = self.main_window.screens["Calibrate"]
            for dac_channel in range(0, 16):
                if integrate.get_Cancel_Measurement():
                    self.update_status_signal.emit("<MT> 4. PDO Stopped the Measurement due to cancel request.", device, ip_address)
                    return False

                if integrate.get_Calibrate_Measurement():
                    if device == "Device 1":
                        print("<MT> Device 1 Array values")
                        print("<MT> Vsd_sn1:", Calibrate_Screen.Vsd_sn1)
                        print("<MT> Vsd_sn2:", Calibrate_Screen.Vsd_sn2)
                        print("<MT> Vg_sn1:", Calibrate_Screen.Vg_sn1)
                        print("<MT> Vg_sn2:", Calibrate_Screen.Vg_sn2)
                        dac_value1 = Calibrate_Screen.Vsd_sn1[dac_channel]
                        dac_value2 = Calibrate_Screen.Vsd_sn2[dac_channel]
                        print(f"<MT> Device 1 dac_channel {dac_channel} Vsd dac_value1 {dac_value1} Vsd dac_value2 {dac_value2}")
                        rc = self.shared_state.cube.SetDAC1ChannelValue(ip_address, dac_channel + 1, float(dac_value1))
                        if not rc:
                            print(f'<MT> Fail for DAC1 channel {dac_channel + 1}, value {dac_value1}')
                            self.shared_state.set_failed_status("<MT> Failed to update DAC1 <Network Failure>  ")
                            self.shared_state.set_measurement_failed(True)
                            return False

                        rc = self.shared_state.cube.SetDAC2ChannelValue(ip_address, dac_channel + 1, float(dac_value2))
                        if not rc:
                            print(f'<MT> Fail for DAC2 channel {dac_channel}, value {dac_value2}')
                            self.shared_state.set_failed_status("<MT> Failed to update DAC2 <Network Failure>  ")
                            self.shared_state.set_measurement_failed(True)
                            return False

                elif integrate.get_mainmeasurement():
                    rc = self.shared_state.cube.SetDAC1ChannelValue(ip_address, dac_channel + 1, float(main_measurement_dac_VSD_value[dac_channel]))
                    if not rc:
                        print(f'<MT> Fail for DAC1 channel {dac_channel + 1}, value {float(main_measurement_dac_VSD_value[dac_channel])}')
                        self.shared_state.set_failed_status("<MT> Failed to update DAC1 <Network Failure>  ")
                        self.shared_state.set_measurement_failed(True)
                        return False

                    rc = self.shared_state.cube.SetDAC2ChannelValue(ip_address, dac_channel + 1, float(main_measurement_dac_VSD_value[dac_channel + 16]))
                    if not rc:
                        print(f'<MT> Fail for DAC2 channel {dac_channel}, value {float(main_measurement_dac_VSD_value[dac_channel])}')
                        self.shared_state.set_failed_status("<MT> Failed to update DAC2 <Network Failure>")
                        self.shared_state.set_measurement_failed(True)
                        return False

                else:
                    rc = self.shared_state.cube.SetDAC1ChannelValue(ip_address, dac_channel + 1, float(dac_value))
                    if not rc:
                        print(f'<MT> Fail for DAC1 channel {dac_channel + 1}, value {dac_value}')
                        self.shared_state.set_failed_status("<MT> Failed to update DAC1 <Network Failure>  ")
                        self.shared_state.set_measurement_failed(True)
                        return False

                    rc = self.shared_state.cube.SetDAC2ChannelValue(ip_address, dac_channel + 1, float(dac_value))
                    if not rc:
                        print(f'<MT> Fail for DAC2 channel {dac_channel}, value {dac_value}')
                        self.shared_state.set_failed_status("<MT> Failed to update DAC2 <Network Failure>")
                        self.shared_state.set_measurement_failed(True)
                        return False

            if integrate.get_Cancel_Measurement():
                self.update_status_signal.emit("<MT> 5. PDO Stopped the Measurement due to cancel request.", device, ip_address)
                integrate.set_measurement1Completed(True)
                return False

            print("<MT> Applying Dac Channels")
            rc = self.shared_state.cube.ApplyDACChannels(ip_address)
            if not rc:
                print(f'<MT> Actual Measurement Apply DAC Channels failed {dac_channel}, value {dac_value}')
                self.shared_state.set_failed_status("<MT> Actual Measurement Apply DAC Channels failed <Network Failure>")
                self.shared_state.set_measurement_failed(True)
                return False

            now = datetime.now()
            print(f"<MT> {now.strftime('%Y-%m-%d %H:%M:%S')} Applied Dac Channels {device}")
            dac1_value = self.shared_state.cube.GetDAC1ChannelValue(ip_address, 1)
            print(f'<MT> DAC1 = {dac1_value}')
            dac2_value = self.shared_state.cube.GetDAC2ChannelValue(ip_address, 1)
            print(f'<MT> DAC2 = {dac2_value}')

            if integrate.get_Cancel_Measurement():
                self.update_status_signal.emit("<MT> 6. PDO Stopped the Measurement due to cancel request.", device, ip_address)
                return False

            Vg_max = self.shared_state.var_vg_max
            Vg_min = self.shared_state.var_vg_min
            if integrate.get_Sweep_Measurement():
                dac_value = Vg_min
                print(f"<MT> SetDAC1ChannelValue Measurement dac_value [16:32] Vg_min <Sweep>= {dac_value}")
            # elif integrate.get_mainmeasurement():
            #     print(f"<MT> SetDAC1ChannelValue Main Measurement dac_value [16:32]  {float(main_measurement_dac_value[dac_channel])}")
            #     dac_value = float(main_measurement_dac_value[dac_channel])
            else:
                dac_value = self.shared_state.var_vg_measure
                print(f"<MT> SetDAC1ChannelValue Measurement dac_value [16:32] Vg= {dac_value}")

            for dac_channel in range(16, 32):
                if integrate.get_Cancel_Measurement():
                    self.update_status_signal.emit("<MT> 7. PDO Stopped the Measurement due to cancel request.", device, ip_address)
                    return False

                if integrate.get_Cancel_Measurement():
                    self.update_status_signal.emit("<MT> 8. PDO Stopped the Measurement due to cancel request.", device, ip_address)
                    return False

                if integrate.get_Calibrate_Measurement():
                    index = dac_channel - 16
                    if device == "Device 1":
                        dac_value1 = Calibrate_Screen.Vg_sn1[index]
                        dac_value2 = Calibrate_Screen.Vg_sn2[index]
                        print(f'<MT> Device 1 dac_channel {dac_channel} Vg dac_value1 {dac_value1} Vg dac_value2 {dac_value2}')
                        rc = self.shared_state.cube.SetDAC1ChannelValue(ip_address, dac_channel + 1, float(dac_value1))
                        if not rc:
                            print(f'<MT> SetDAC1ChannelValue Failed {dac_channel + 1}, value {dac_value1}')
                            self.shared_state.set_failed_status(f"<MT> Fail for DAC1 channel {dac_channel + 1} <Network Failure>")
                            self.shared_state.set_measurement_failed(True)
                            return False
                        rc = self.shared_state.cube.SetDAC2ChannelValue(ip_address, dac_channel + 1, float(dac_value2))
                        if not rc:
                            print(f'<MT> SetDAC2ChannelValue Failed {dac_channel + 1}, value {dac_value2}')
                            self.shared_state.set_failed_status(f"<MT> Fail for DAC2 channel {dac_channel + 1} <Network Failure>")
                            self.shared_state.set_measurement_failed(True)
                            return False
                elif integrate.get_mainmeasurement():
                    rc = self.shared_state.cube.SetDAC1ChannelValue(ip_address, dac_channel + 1, float(main_measurement_dac_Vg_value[dac_channel - 16]))
                    if not rc:
                        print(f'<MT> SetDAC1ChannelValue Failed {dac_channel + 1}, value {float(main_measurement_dac_Vg_value[dac_channel])}')
                        self.shared_state.set_failed_status(f"<MT> Fail for DAC1 channel {dac_channel + 1} <Network Failure>")
                        self.shared_state.set_measurement_failed(True)
                        return False
                    rc = self.shared_state.cube.SetDAC2ChannelValue(ip_address, dac_channel + 1, float(main_measurement_dac_Vg_value[dac_channel]))
                    if not rc:
                        print(f'<MT> SetDAC2ChannelValue Failed {dac_channel + 1}, value {float(main_measurement_dac_Vg_value[dac_channel + 1])}')
                        self.shared_state.set_failed_status(f"<MT> Fail for DAC2 channel {dac_channel + 1} <Network Failure>")
                        self.shared_state.set_measurement_failed(True)
                        return False
                else:
                    rc = self.shared_state.cube.SetDAC1ChannelValue(ip_address, dac_channel + 1, float(dac_value))
                    if not rc:
                        print(f'<MT> SetDAC1ChannelValue Failed {dac_channel + 1}, value {dac_value}')
                        self.shared_state.set_failed_status(f"<MT> Fail for DAC1 channel {dac_channel + 1} <Network Failure>")
                        self.shared_state.set_measurement_failed(True)
                        return False
                    rc = self.shared_state.cube.SetDAC2ChannelValue(ip_address, dac_channel + 1, float(dac_value))
                    if not rc:
                        print(f'<MT> SetDAC2ChannelValue Failed {dac_channel + 1}, value {dac_value}')
                        self.shared_state.set_failed_status(f"<MT> Fail for DAC2 channel {dac_channel + 1} <Network Failure>")
                        self.shared_state.set_measurement_failed(True)
                        return False
            rc = self.shared_state.cube.ApplyDACChannels(ip_address)
            if not rc:
                print('<MT> Fail to ApplyDACChannels')
                self.shared_state.set_failed_status("<MT> Fail to ApplyDACChannels <Network Failure>")
                self.shared_state.set_measurement_failed(True)
                return False

            now = datetime.now()
            print(f"{now.strftime('%Y-%m-%d %H:%M:%S')} Applied Config1 {device}")
            if integrate.get_Sweep_Measurement():
                dac_value = Vg_max
                for dac_channel in range(16, 32):
                    if integrate.get_Cancel_Measurement():
                        self.update_status_signal.emit("<MT> 9. PDO Stopped the Measurement due to cancel request.", device, ip_address)
                        return False

                    rc = self.shared_state.cube.SetDAC1ChannelValue(ip_address, dac_channel + 1, float(dac_value))
                    if not rc:
                        print(f'<MT> SetDAC1ChannelValue Failed {dac_channel + 1}, value {dac_value}')
                        self.shared_state.set_failed_status("<MT> SetDAC1ChannelValue Failed  <Network Failure>")
                        self.shared_state.set_measurement_failed(True)
                    rc = self.shared_state.cube.SetDAC2ChannelValue(ip_address, dac_channel + 1, float(dac_value))
                    if not rc:
                        print(f'<MT> SetDAC2ChannelValue Failed {dac_channel + 1}, value {dac_value}')
                        self.shared_state.set_failed_status("<MT> SetDAC1ChannelValue Failed <Network Failure>")
                        self.shared_state.set_measurement_failed(True)
                        return False
                now = datetime.now()
                print(f"{now.strftime('%Y-%m-%d %H:%M:%S')} Prepared Config2 {device}")

            if integrate.get_Sweep_Measurement():
                sampling_time = (0.001 * self.shared_state.cube.d_time) * ((self.shared_state.var_vg_max - self.shared_state.var_vg_min) / self.shared_state.get_var_g_step())
                integrate.set_sampling_time(sampling_time)
                print(f'<MT> Sweep Measurement Sampling time: {sampling_time} s')
                if self.shared_state.get_EnableBricklets_Checkbox():
                    integrate.set_Start_Bricketlet_T0_flow(True)
                    print("<MT> integrate.Start_Bricketlet_T0_flow sweep Measurement", integrate.get_Start_Bricketlet_T0_flow())
            elif integrate.get_Normal_Measurement():
                sampling_time = float(self.shared_state.var_time)
                integrate.set_sampling_time(sampling_time)
                print(f'<MT> Advance Measurement Sampling time: {sampling_time} s')
            elif integrate.get_Calibrate_Measurement():
                sampling_time = self.shared_state.calib_time
                integrate.set_sampling_time(sampling_time)
                print(f'<MT> Calibrate Measurement Sampling time: {sampling_time} s')
            else:
                # Adding 8 seconds for the Bricklet delay
                if integrate.get_enable_bricklet():
                    sampling_time = (self.shared_state.get_T1() + self.shared_state.get_T2() + self.shared_state.get_T3() + 8)
                    integrate.set_sampling_time(sampling_time)
                else:
                    sampling_time = (self.shared_state.get_T1() + self.shared_state.get_T2() + self.shared_state.get_T3())
                    integrate.set_sampling_time(sampling_time)
                print(f'<MT> Main Measurement Sampling time: {sampling_time} s')

            if integrate.get_Cancel_Measurement():
                self.update_status_signal.emit("<MT> 10. PDO Stopped the Measurement due to cancel request.", device, ip_address)
                return False

            sampling_time = math.ceil(sampling_time)
            print(f'<MT> Main Measurement Sampling time Roundoff value: {sampling_time} s')
            if not integrate.get_Calibrate_Measurement():
                if integrate.get_mainmeasurement() or ((integrate.get_Sweep_Measurement() or integrate.get_Normal_Measurement()) and self.shared_state.get_EnableBricklets_Checkbox()):
                    print("<MT> Starting Bricklet Configuration")
                    self.update_status_signal.emit("<MT> Starting Bricklet Configuration", device, ip_address)
                    integrate.start_bricklet_event.set()
                    self.update_status_signal.emit("<MT> Waiting to start the Data Acquisition", device, ip_address)
                    print("<MT> Waiting to start the Data Acquisition")

                    # Waiting to get the start data acquisition event.
                    while True:
                        event_set = integrate.Start_data_acquisition_event.wait(timeout=1)
                        if event_set:
                            break
                        if integrate.get_Cancel_Measurement():
                            self.update_status_signal.emit("<MT> 11. PDO Stopped the Measurement due to user cancel request.", device, ip_address)
                            return False

                    integrate.Start_data_acquisition_event.clear()

            final_measurement_timing = None
            # Start Data Acquisition
            now = datetime.now()
            print(f"<MT> {now.strftime('%Y-%m-%d %H:%M:%S')} Starting Data Acq {device}")
            self.update_status_signal.emit("<MT> Starting Data Acq Measurement Started", device, ip_address)

            if self.shared_state.Enable_Bricklets:
                data_acq_start_time = time.time()

            if integrate.get_Cancel_Measurement():
                self.update_status_signal.emit("<MT> 12. PDO Aborting the Measurement due to user cancel request.", device, ip_address)
                return False
            if integrate.get_Sweep_Measurement():
                rc = self.shared_state.cube.StartDataAcq(ip_address, 2 * sampling_time + 3)
            else:
                rc = self.shared_state.cube.StartDataAcq(ip_address, sampling_time + 3)
            if not rc:
                if self.is_active:
                    print(f"<MT> Emitting update_status_signal: Failed Start Data Acquisition for {device}")
                    self.update_status_signal.emit("<MT> Failed Start Data Acquisition.", device, ip_address)
                    self.shared_state.set_failed_status("<MT> Failed Start Data Acquisition <Network Failure>")
                    self.shared_state.set_measurement_failed(True)
                return False
            t0 = time.time()

            # Ramp up for Sweep Measurement
            if integrate.get_Sweep_Measurement():
                print(f"<MT> Applying ADC Channels 0:15 <Sweep> {device}")
                rc = self.shared_state.cube.ApplyDACChannels(ip_address)
                print(f"<MT> After Applying ADC Channels <Sweep> {device}")
                dac_value = Vg_min
                for dac_channel in range(16, 32):
                    if integrate.get_Cancel_Measurement():
                        self.update_status_signal.emit("<MT> 13. PDO Aborting the Measurement due to user cancel request.", device, ip_address)
                        return False

                    rc = self.shared_state.cube.SetDAC1ChannelValue(ip_address, dac_channel + 1, float(dac_value))
                    if not rc:
                        print(f'<MT> Failed to SetDAC1ChannelValue channel {dac_channel + 1}, value {dac_value}')
                        self.shared_state.set_failed_status("<MT> Measurement Sweep : Set DAC1 Channel Value Failed <Network Failure>")
                        self.shared_state.set_measurement_failed(True)
                        return False

                    rc = self.shared_state.cube.SetDAC2ChannelValue(ip_address, dac_channel + 1, float(dac_value))
                    if not rc:
                        print(f'<MT> Failed to SetDAC2ChannelValue channel {dac_channel + 1}, value {dac_value}')
                        self.shared_state.set_failed_status("<MT> Measurement Sweep : Set DAC2 Channel Value Failed <Network Failure>")
                        self.shared_state.set_measurement_failed(True)
                        return False
                print(f"<MT> Applying ADC Channels16:32 <Sweep> {device}")
                rc = self.shared_state.cube.ApplyDACChannels(ip_address)
                if not rc:
                    print('<MT> Failed to ApplyDACChannels during Sweep Measurement ramp up')
                    self.shared_state.set_failed_status("<MT> Measurement Sweep : Apply DAC Channels Failed <Network Failure>")
                    self.shared_state.set_measurement_failed(True)
                    return False
                print(f"<MT> After Applying ADC Channels <Sweep> {device}")

            # Wait for remaining measurement time
            measurement_sleep_time = sampling_time if not integrate.get_Sweep_Measurement() else 2 * sampling_time
            print(f"<MT> Sleeping for {measurement_sleep_time} seconds for {device}")

            now = datetime.now()
            print(f"<MT> {now.strftime('%Y-%m-%d %H:%M:%S')} Starting Data Acquisition Before Loop")
            while time.time() < t0 + measurement_sleep_time:
                print("<MT> Measurement is going Waiting for time to elapse")
                if integrate.get_Cancel_Measurement():
                    self.update_status_signal.emit("<MT> 14. PDO Aborting the Measurement due to user cancel request.", device, ip_address)
                    self.shared_state.cube.AbortMeasurement(ip_address)
                    return False
                if not self.shared_state.is_eth_up():
                    if self.is_active:
                        self.update_status_signal.emit("<MT> Failed to Network is Down", device, ip_address)
                    print(f"<MT>  Ethernet is down during the Measurement {device}")
                    self.shared_state.set_failed_status("<MT>  Ethernet is down during the Measurement <Network Failure>")
                    self.shared_state.set_measurement_failed(True)
                    return False

                time.sleep(0.1)  # Small sleep to prevent busy waiting
            now = datetime.now()
            print(f"<MT> {now.strftime('%Y-%m-%d %H:%M:%S')} Starting Data Acquisition After Loop")

            now = datetime.now()
            print(f"<MT> {now.strftime('%Y-%m-%d %H:%M:%S')} Going to Start Abort {device}")
            if not integrate.get_Calibrate_Measurement():
                if integrate.get_mainmeasurement() or ((integrate.get_Sweep_Measurement() or integrate.get_Normal_Measurement()) and self.shared_state.get_EnableBricklets_Checkbox()):
                    while not integrate.get_stop_measurement_data_acq():
                        print("<MT> Waiting to Abort the Measurement")
                        if integrate.get_Cancel_Measurement():
                            self.update_status_signal.emit("<MT> 15. PDO <Aborting Loop : Stopped the Measurement due to user cancel request.", device, ip_address)
                            self.shared_state.cube.AbortMeasurement(ip_address)
                            return False
                        time.sleep(1)
                        if not self.shared_state.is_eth_up():
                            if self.is_active:
                                self.update_status_signal.emit("<MT> Failed to Network is Down <2>", device, ip_address)
                            print(f"<MT>  Ethernet is down during the Measurement  <2> {device}")
                            self.shared_state.set_failed_status("<MT>  Ethernet is down during the Measurement <2>")
                            self.shared_state.set_measurement_failed(True)
            print(f"<MT> Starting Abort {device} {ip_address}")
            now = datetime.now()
            print(f"<MT> {now.strftime('%Y-%m-%d %H:%M:%S')} Starting Abort Acquisition")
            self.update_status_signal.emit("<MT> Aborting Measurement", device, ip_address)
            rc = self.shared_state.cube.AbortMeasurement(ip_address)
            #if not rc:
            #    if integrate.get_Cancel_Measurement():
            #        self.update_status_signal.emit("<MT> 16. PDO Cancel the Measurement ", device, ip_address)
            #        return False
            #    if self.is_active:
            #        self.update_status_signal.emit("<MT> Failed to Abort Measurement.", device, ip_address)
            #    print("<MT>  Failed to Abort Measurement")
            #    self.shared_state.set_failed_status("<MT> Failed to Abort Measurement <Network Failure>")
            #    self.shared_state.set_measurement_failed(True)
            #    return False

            print(f"<MT> End Aborted {device}")

            now = datetime.now()
            # Wait for Bricklet to finish final stage
            tnow = datetime.now()
            while True:
                rc = self.shared_state.get_Bricklet_T3Stage_Complete()
                if rc:
                    break

                if datetime.now() > tnow + timedelta(seconds=10):
                    break

                if integrate.get_Cancel_Measurement():
                    self.shared_state.set_Bricklet_T3Stage_Complete(False)
                    self.update_status_signal.emit("<MT> 17. PDO Stopped the Measurement due to Bricklet T3Stage issue.", device, ip_address)
                    return False

                time.sleep(0.5)

            self.shared_state.set_Bricklet_T3Stage_Complete(False)

            if self.shared_state.Enable_Bricklets:
                data_acq_end_time = time.time()
                final_measurement_timing = data_acq_end_time - data_acq_start_time
                print(f"<MT> Final measurement timing for {device}: {final_measurement_timing:.2f} seconds")

            now = datetime.now()
            print(f"<MT>{now.strftime('%Y-%m-%d %H:%M:%S')} Measurement completed successfully for {device}")
            if self.is_active:
                print(f"<MT> Emitting update_status_signal: Measurement completed successfully for {device}")
                self.update_status_signal.emit("<MT> Measurement has been completed successfully.", device, ip_address)

            integrate.set_measurement1Completed(True)
            #self.terminal_dialog.cancel_button.setEnabled(False)
            QMetaObject.invokeMethod(self.terminal_dialog.cancel_button, "setEnabled", Qt.QueuedConnection, Q_ARG(bool, False))

            if not os.path.exists(self.base_path):
                try:
                    os.makedirs(self.base_path)
                except Exception as e:
                    print(f"<MT> Failed to create data directory for {device}: {str(e)}")
                    if self.is_active:
                        print(f"<MT> Emitting update_status_signal: Failed to create data directory for {device}")
                        self.update_status_signal.emit(f"<MT> Failed to create data directory: {str(e)}", device, ip_address)
                    return False


            print(f"<MT> Starting to transfer rawtext file for {device}")

            if self.is_active:
                print(f"<MT> Emitting update_status_signal: Starting to transfer rawtext file for {device}")
                self.update_status_signal.emit("<MT> Starting to transfer rawtext file", device, ip_address)
            flag, result, cmd = self.shared_state.cube.StartTransferMeasurementFile(ip_address, rawtext_file_path)
            if not flag:
                print(f"<MT> Scp Command Failed for {device}")
                if self.is_active:
                    print(f"<MT> Emitting update_status_signal: Scp Command Failed for {device}")
                    self.update_status_signal.emit("<MT> Scp Command Failed", device, ip_address)
                self.shared_state.set_failed_status("<MT> Scp Command Failed <Network Failure>")
                self.shared_state.set_measurement_failed(True)
                return False
            if result:
                print(f"<MT> File not transferred successfully for {device}")
                if self.is_active:
                    print(f"<MT> Emitting update_status_signal: File not transferred successfully for {device}")
                    self.update_status_signal.emit("<MT> File Not transferred successfully!", device, ip_address)
                self.shared_state.set_failed_status("<MT> File Not transferred successfully! <Network Failure>")
                self.shared_state.set_measurement_failed(True)
                return False
            print(f"<MT> File transferred successfully as {timestamped_filename} for {device}")
            
            if self.is_active:
                print(f"<MT> Emitting update_status_signal: File transferred successfully for {device}")
                self.update_status_signal.emit(f"<MT> File transferred successfully as {timestamped_filename}!", device, ip_address)

            integrate.set_temperature_log_active(False)
            self.update_rawtext_header(rawtext_file_path)

            # Compress the raw text file and saving to upload folder
            # Define upload_data folder path
            upload_folder = os.path.join(self.today_date_folder_path, "upload_data")

            # Check if folder exists, if not create
            if not os.path.exists(upload_folder):
                os.makedirs(upload_folder)

            # Create tar file name
            tar_filename = timestamped_filename + ".tar"
            tar_file_path = os.path.join(self.today_date_folder_path, tar_filename)

            # Compress file into tar
            with tarfile.open(tar_file_path, "w") as tar:
                tar.add(rawtext_file_path, arcname=timestamped_filename)

            # Move tar file to upload_data folder
            destination_path = os.path.join(upload_folder, tar_filename)
            shutil.move(tar_file_path, destination_path)

            #Sending command to Upload Process
            cmd_input = struct.pack("I", CMD_ID_UPLOAD)
            self.mqId_upload.send(cmd_input, True, type=1)
            print("Firstview Zaid mqId_upload: sending ", self.mqId_upload)

            # ======================================================
            # ExcelSheet Logging
            # ======================================================
            
            if integrate.get_mainmeasurement() or integrate.get_Sweep_Measurement() or integrate.get_Normal_Measurement():
                # Get absolute base directory
                BASE_DIR = os.path.dirname(os.path.abspath(__file__))
                calib_folder = os.path.join(BASE_DIR, 'data', self.date_str, 'calibrationData')
                os.makedirs(calib_folder, exist_ok=True)
                # -------------------------------
                # Find LATEST SM file
                # Example: SM_-3.0_0.5_rawtext1_20260103_183157.txt
                # -------------------------------
                latest_sm_file = None
                latest_sm_ts = None

                for fname in os.listdir(calib_folder):
                    if not fname.startswith("SM_"):
                        continue

                    name = fname
                    if name.endswith(".txt"):
                        name = name.replace(".txt", "")

                    parts = name.split('_')
                    if len(parts) < 3:
                        continue

                    sm_date = parts[-2]
                    sm_time = parts[-1]

                    if not (sm_date.isdigit() and sm_time.isdigit()):
                        continue

                    sm_ts = sm_date + sm_time

                    if latest_sm_ts is None or sm_ts > latest_sm_ts:
                        latest_sm_ts = sm_ts
                        latest_sm_file = fname

                # --------------------------------------------------
                # If NO SM file exists → use Vsd default
                # --------------------------------------------------
                sm_is_virtual = False

                if latest_sm_file is None or integrate.get_Sweep_Measurement() or integrate.get_Normal_Measurement() or not self.shared_state.get_Vg_Vsd_calibration_mode():
                    if integrate.get_Sweep_Measurement():
                        Vsd_value = self.shared_state.var_vsd
                    else:   
                        Vsd_value = self.shared_state.var_vsd_measure
                        
                    latest_sm_file = f"Advanced Settings Vsd Value {Vsd_value}"
                    sm_is_virtual = True
                    print("<MT> No calibration file found, using default Vsd =", Vsd_value)

                print(f"<MT> Latest SM file selected: {latest_sm_file}")

                # -------------------------------
                # Experiment LIST CSV
                # -------------------------------
                csv_filename = f"Experiment_list_calibration_{self.date_str}.csv"
                csv_file_path = os.path.join(calib_folder, csv_filename)
                os.makedirs(os.path.dirname(csv_file_path), exist_ok=True)
                csv_headers = ["File Name","File Type","Date","Time","Associated Sweep Calibration","SN1 Number","SN2 Number","Sample 1 ID","Sample 2 ID","Raspberry Pi Serial Number","Board ID","Software version"]
                if not os.path.isfile(csv_file_path):
                    with open(csv_file_path, 'w', newline='') as f:
                        csv.writer(f).writerow(csv_headers)

                # ======================================================
                # STORE SM ENTRY (Calibration Sweep)
                # ======================================================
                sm_entry_exists = False

                with open(csv_file_path, 'r', newline='') as f:
                    for row in csv.reader(f):
                        if row and row[0] == latest_sm_file:
                            sm_entry_exists = True
                            break

                if not sm_entry_exists:
                    if sm_is_virtual:
                        # VSD based calibration – no timestamp
                        sm_date = "-"
                        sm_time = "-"
                    else:
                        sm_parts = latest_sm_file.replace(".txt", "").split('_')
                        sm_date = sm_parts[-2]
                        sm_time = sm_parts[-1]

                    # with open(csv_file_path, 'a', newline='') as f:
                    #     csv.writer(f).writerow([
                    #         latest_sm_file,
                    #         "Calibration sweep",
                    #         sm_date,
                    #         sm_time,
                    #         "-"
                    #     ])

                    # print(f"<MT> SM entry added: {latest_sm_file}")

                # ======================================================
                # STORE MM ENTRY (Main Measurement)
                # ======================================================
                # Example: MM_0_178_rawtext1_20260103_184201.txt

                sn1_number = self.shared_state.get_var_Sensor_SN1()
                sn2_number = self.shared_state.get_var_Sensor_SN2()
                sample1_id = self.shared_state.get_var_Sample_ID1()
                sample2_id = self.shared_state.get_var_Sample_ID2()
                
                """
                Zaid NewReq
                """
                Rp_id = self.shared_state.get_rpi_serial()
                dev_id = self.shared_state.read_soc_uid()
                parts = timestamped_filename.replace(".txt", "").split('_')
                file_date = parts[-2]
                file_time = parts[-1]

                mm_entry_exists = False

                with open(csv_file_path, 'r', newline='') as f:
                    for row in csv.reader(f):
                        if row and row[0] == timestamped_filename:
                            mm_entry_exists = True
                            break
                                                        
                if integrate.get_Sweep_Measurement():
                    File_Type = "Sweep Measurement"
                elif integrate.get_Normal_Measurement():
                    File_Type = "Normal Measurement"
                elif not mm_entry_exists:
                    File_Type = "Main Measurement"
                with open(csv_file_path, 'a', newline='') as f:
                    """
                    Zaid NewReq
                    """
                    csv.writer(f).writerow([timestamped_filename,File_Type,file_date,file_time,latest_sm_file,sn1_number,sn2_number,sample1_id,sample2_id,Rp_id,dev_id,APP_VERSION])
                print(f"<MT> MM entry added: {timestamped_filename}")

            return True
        except Exception as e:
            print(f"<MT> Error in perform_dac_operations for {device}: {str(e)}")
            if self.is_active:
                print(f"<MT> Emitting update_status_signal: Error for {device}")
                self.update_status_signal.emit(f"<MT> Error: {str(e)}", device, ip_address)
            if not integrate.get_Cancel_Measurement():
                self.shared_state.set_failed_status(f"<MT> Error in perform dac: {str(e)}  <Network Failure>")
                self.shared_state.set_measurement_failed(True)
            return False

    def show_message_box(self, title, message):
        QMessageBox.information(self, title, message)
        QMessageBox.information(self, title, message)
        
    def update_rawtext_header(self, filename: str):
        
        header_format = (
            "<" +
            "I" +            # m_FileFormat
            "I" +            # m_StartTimestamp
            "Q" +            # m_Sensor1SerialNo
            "I" +            # m_MeasurementCnt1
            "Q" +            # m_Sensor2SerialNo
            "I" +            # m_MeasurementCnt2
            "I" +            # m_MeasurementRate
            "I" +            # m_AlgorithmMode
            "f" * 8 +        # m_SensorXTempY and HumiY
            "d" * 8 +        # m_ProxiData[0-7]
            "32s" +          # App Version
            "32s" +          # Pi Serial
            "d" * 2 +        # Padding
            "I" * 16 +       # ADC Channel Sel
            "16s" +          # SensorID 1
            "16s" +          # SensorID 2
            "16s" +          # SampleID 1
            "16s" +          # SampleID 2
            "I" +            # m_MeasAbortCnt1
            "I" +            # m_MeasAbortCnt2
            "I"              # m_TestResult
        )

        header_size = struct.calcsize(header_format)
        self.file_path = filename

        # -----------------------------
        # Read existing header
        # -----------------------------
        with open(self.file_path, "rb") as f:
            header_bytes = f.read(header_size)

        if len(header_bytes) < header_size:
            print("File too small for full header.")
            return

        header_list = list(struct.unpack(header_format, header_bytes))

        # -----------------------------
        # Modify sensor serial numbers
        # -----------------------------
        header_list[2] = 0
        header_list[4] = 0

        # -----------------------------
        # Modify ProxiData values
        # -----------------------------
        header_list[16] = self.shared_state.var_vg_measure
        header_list[17] = self.shared_state.var_vsd_measure
        header_list[18] = self.shared_state.var_vg_min
        header_list[19] = self.shared_state.var_vg_max
        header_list[20] = self.shared_state.var_vsd
        header_list[21] = self.shared_state.var_t_dwell
        header_list[22] = self.shared_state.var_d_step
        header_list[23] = self.shared_state.var_g_step

        # -----------------------------
        # Store App Version & Pi Serial
        # -----------------------------
        header_list[24] = self.shared_state.get_MVP_Device_Version().encode("utf-8")
        header_list[25] = self.shared_state.get_Raspberrypi_Serial_Number().encode("utf-8")

        header_list[26] = 0.0
        header_list[27] = 0.0

        # -----------------------------
        # Store SensorID / SampleID
        # -----------------------------
        sn1_number = str(self.shared_state.get_var_Sensor_SN1()).encode("utf-8")[:16]
        sn2_number = str(self.shared_state.get_var_Sensor_SN2()).encode("utf-8")[:16]
        sample1_id = str(self.shared_state.get_var_Sample_ID1()).encode("utf-8")[:16]
        sample2_id = str(self.shared_state.get_var_Sample_ID2()).encode("utf-8")[:16]

        # Correct indexes after ADC array
        header_list[44] = sn1_number.ljust(16, b'\x00')
        header_list[45] = sn2_number.ljust(16, b'\x00')
        header_list[46] = sample1_id.ljust(16, b'\x00')
        header_list[47] = sample2_id.ljust(16, b'\x00')

        # -----------------------------
        # Write back updated header
        # -----------------------------
        updated_header = struct.pack(header_format, *header_list)

        with open(self.file_path, "r+b") as f:
            f.seek(0)
            f.write(updated_header)

    def write_nfc_tags(self, device, filename: str, id1: bytes, id2):
        print(f"Writing NFC tags to {filename} for {device}")
        with open(filename, 'a+b') as f:
            f.seek(0, 2)
            size = f.tell()
            if size < 204:
                f.write(b'\x00' * (204 - size))
        with open(filename, 'r+b') as f:
            f.seek(72)
            f.write(id1)
            f.seek(79)
            f.write(id2)
