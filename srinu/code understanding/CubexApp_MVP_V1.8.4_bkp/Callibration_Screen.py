from TitleBar import TitleBar
from PyQt5.QtWidgets import QWidget, QVBoxLayout, QHBoxLayout, QLabel, QLineEdit, QComboBox, QPushButton, QGroupBox,QMessageBox
from PyQt5.QtCore import Qt
import integrate
import os
import shutil

class CallibrationScreen(TitleBar):
    def __init__(self, main_window, username, shared_state):
        super().__init__(main_window, username, "CALIBRATE")
        self.shared_state = shared_state
        
        print("[DEBUG] Initializing CallibrationScreen")
        
        # Initial array values for Board 1
        self.Vg_sn1 = [0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.]
        self.Vsd_sn1 = [0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.]
        self.Vg_sn2 = [0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.]
        self.Vsd_sn2 = [0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.]
        
       
        # Set dark background for the container
        self.setStyleSheet("background-color: #242746;")
        
        # Main layout
        if not self.layout():
            self.main_layout = QVBoxLayout()
            self.setLayout(self.main_layout)
        else:
            self.main_layout = self.layout()
        
        self.setFixedHeight(750)
        print("[DEBUG] CallibrationScreen layout set and widget visible")
        
        # Create a centered container widget for all content
        self.central_widget = QWidget()
        self.central_widget.setStyleSheet("background-color: #242746;")
        self.central_layout = QVBoxLayout()
        self.central_widget.setLayout(self.central_layout)
        self.central_widget.setFixedWidth(600)
        self.main_layout.addWidget(self.central_widget, alignment=Qt.AlignCenter)
        
        self.central_layout.addSpacing(40)
        
        # Calibration Status
        self.status_label = QLabel("Calibration Status: Not Started")
        self.status_label.setStyleSheet("font-size: 16px; color: #00CED1; font-weight: bold;")
        self.status_label.setAlignment(Qt.AlignCenter)
        self.central_layout.addWidget(self.status_label)
        
        self.central_layout.addSpacing(20)
        
        # Time Input
        time_layout = QHBoxLayout()
        time_label = QLabel("Time:")
        time_label.setStyleSheet("color: white; font-size: 14px;")
        time_label.setFixedWidth(60)
        time_layout.addWidget(time_label)
        self.time_entry = QLineEdit("0")
        self.time_entry.setFixedSize(100, 30)
        self.time_entry.setStyleSheet("background-color: #333; color: white; border: 1px solid #FFF; padding: 5px; font-size: 14px;")
        time_layout.addWidget(self.time_entry)
        time_layout.addStretch()
        self.central_layout.addLayout(time_layout)
        
        self.central_layout.addSpacing(20)
        
        # Board 1
        board1_group = QGroupBox("Board 1")
        board1_group.setStyleSheet("color: white; border: 1px solid #FFF; border-radius: 5px; background-color: #242746; font-size: 14px;")
        board1_layout = QVBoxLayout()
        board1_group.setMinimumSize(400, 200)
        board1_group.setLayout(board1_layout)
        
        # Vg Sensor 1 (Board 1)
        vg_sn1_layout = QHBoxLayout()
        self.combo_vg_sn1 = QComboBox()
        self.combo_vg_sn1.setFixedSize(100, 30)
        self.combo_vg_sn1.addItems([str(i) for i in range(1, 17)])
        self.combo_vg_sn1.setStyleSheet("background-color: #333; color: white; border: 1px solid #FFF; padding: 5px; font-size: 14px;")
        vg_sn1_layout.addWidget(self.combo_vg_sn1)
        vg_sn1_layout.addSpacing(10)
        vg_sn1_label = QLabel("Vg Sensor 1")
        vg_sn1_label.setStyleSheet("color: white; font-size: 14px;")
        vg_sn1_label.setFixedWidth(100)
        vg_sn1_layout.addWidget(vg_sn1_label)
        self.entry_vg_sn1 = QLineEdit("0")
        self.entry_vg_sn1.setFixedSize(100, 30)
        self.entry_vg_sn1.setStyleSheet("background-color: #333; color: white; border: 1px solid #555; padding: 5px; font-size: 14px;")
        vg_sn1_layout.addWidget(self.entry_vg_sn1)
        vg_sn1_layout.addSpacing(10)
        vsd_sn1_label = QLabel("Vsd Sensor 1")
        vsd_sn1_label.setStyleSheet("color: white; font-size: 14px;")
        vsd_sn1_label.setFixedWidth(100)
        vg_sn1_layout.addWidget(vsd_sn1_label)
        self.entry_vsd_sn1 = QLineEdit("0")
        self.entry_vsd_sn1.setFixedSize(100, 30)
        self.entry_vsd_sn1.setStyleSheet("background-color: #333; color: white; border: 1px solid #555; padding: 5px; font-size: 14px;")
        vg_sn1_layout.addWidget(self.entry_vsd_sn1)
        vg_sn1_layout.addStretch()
        board1_layout.addLayout(vg_sn1_layout)
        
        # Vg Sensor 2 (Board 1)
        vg_sn2_layout = QHBoxLayout()
        self.combo_vg_sn2 = QComboBox()
        self.combo_vg_sn2.setFixedSize(100, 30)
        self.combo_vg_sn2.addItems([str(i) for i in range(1, 17)])
        self.combo_vg_sn2.setStyleSheet("background-color: #333; color: white; border: 1px solid #555; padding: 5px; font-size: 14px;")
        vg_sn2_layout.addWidget(self.combo_vg_sn2)
        vg_sn2_layout.addSpacing(10)
        vg_sn2_label = QLabel("Vg Sensor 2")
        vg_sn2_label.setStyleSheet("color: white; font-size: 14px;")
        vg_sn2_label.setFixedWidth(100)
        vg_sn2_layout.addWidget(vg_sn2_label)
        self.entry_vg_sn2 = QLineEdit("0")
        self.entry_vg_sn2.setFixedSize(100, 30)
        self.entry_vg_sn2.setStyleSheet("background-color: #333; color: white; border: 1px solid #555; padding: 5px; font-size: 14px;")
        vg_sn2_layout.addWidget(self.entry_vg_sn2)
        vg_sn2_layout.addSpacing(10)
        vsd_sn2_label = QLabel("Vsd Sensor 2")
        vsd_sn2_label.setStyleSheet("color: white; font-size: 14px;")
        vsd_sn2_label.setFixedWidth(100)
        vg_sn2_layout.addWidget(vsd_sn2_label)
        self.entry_vsd_sn2 = QLineEdit("0")
        self.entry_vsd_sn2.setFixedSize(100, 30)
        self.entry_vsd_sn2.setStyleSheet("background-color: #333; color: white; border: 1px solid #555; padding: 5px; font-size: 14px;")
        vg_sn2_layout.addWidget(self.entry_vsd_sn2)
        vg_sn2_layout.addStretch()
        board1_layout.addLayout(vg_sn2_layout)
        
        self.central_layout.addWidget(board1_group)
        
        self.central_layout.addSpacing(50)
        
                
        # Calibrate Button
        self.calibrate_button = QPushButton("Calibrate")
        self.calibrate_button.setFixedSize(120, 40)
        self.calibrate_button.setStyleSheet("""
            QPushButton {
                background-color: #444;
                color: white;
                font-size: 14px;
                font-weight: bold;
                padding: 8px;
                border: 1px solid #666;
                border-radius: 4px;
            }
            QPushButton:hover {
                background-color: #555;
            }
        """)
        self.calibrate_button.clicked.connect(self.calibrate)
        self.central_layout.addWidget(self.calibrate_button, alignment=Qt.AlignCenter)
        
        self.central_layout.addStretch()
        
        print("[DEBUG] CallibrationScreen UI setup complete")
    
    
    # Calibrate Button Click
    def calibrate(self):
        if self.shared_state.get_device_connection_state():
            try:  
                # Checking the device available space before the measurement
                path = "C:\\" if os.name == "nt" else "/"
                _, _, free = shutil.disk_usage(path)
                free_gb = free / (1024 ** 3)
                
                if free_gb < 1:
                    QMessageBox.warning(None, "Alert", f"Low Disk Space. Not able to continue the measurement.\nAvailable space: {free_gb:.2f} GB")
                    return

                # Retrieve and validate the time value
                time_value = self.time_entry.text()
                if time_value:
                    try:
                        time_value = float(time_value)  # Convert to float (or int if needed)
                        self.shared_state.calib_time = time_value
                        print(f"[DEBUG] Time value: {time_value}")
                    except ValueError:
                        self.status_label.setText("Calibration Status: Invalid time input")
                        print("[DEBUG] Invalid time input")
                        return
                else:
                    self.status_label.setText("Calibration Status: Time input is empty")
                    print("[DEBUG] Time input is empty")
                    return
                
                self.reset_all_arrays() # Resetting array values before setting

                # Flag to check if any updates were made
                updated = False
                
                # Update Board 1 arrays
                # Vg_sn1 (Board 1)
                if self.combo_vg_sn1.currentText() and self.entry_vg_sn1.text():
                    index = int(self.combo_vg_sn1.currentText()) - 1
                    value = float(self.entry_vg_sn1.text())
                    self.Vg_sn1[index] = value
                    updated = True
                    print(f"[DEBUG] Updated Vg_sn1 at index {index} to {value}")
                
                # Vsd_sn1 (Board 1)
                if self.entry_vsd_sn1.text():
                    index = int(self.combo_vg_sn1.currentText()) - 1
                    value = float(self.entry_vsd_sn1.text())
                    self.Vsd_sn1[index] = value
                    updated = True
                    print(f"[DEBUG] Updated Vsd_sn1 at index {index} to {value}")
                
                # Vg_sn2 (Board 1)
                if self.combo_vg_sn2.currentText() and self.entry_vg_sn2.text():
                    index = int(self.combo_vg_sn2.currentText()) - 1
                    value = float(self.entry_vg_sn2.text())
                    self.Vg_sn2[index] = value
                    updated = True
                    print(f"[DEBUG] Updated Vg_sn2 at index {index} to {value}")
                
                # Vsd_sn2 (Board 1)
                if self.entry_vsd_sn2.text():
                    index = int(self.combo_vg_sn2.currentText()) - 1
                    value = float(self.entry_vsd_sn2.text())
                    self.Vsd_sn2[index] = value
                    updated = True
                    print(f"[DEBUG] Updated Vsd_sn2 at index {index} to {value}")
               
           
                if updated:
                    # Update status
                    self.status_label.setText("Calibration Status: Array lists are Updated")
                    
                    # Final updated array values for Board 1 and 2
                    print("\n[DEBUG] Updated Array Values:")
                    print(f"Vg_sn1 (Board 1): {self.Vg_sn1}")
                    print(f"Vsd_sn1 (Board 1): {self.Vsd_sn1}")
                    print(f"Vg_sn2 (Board 1): {self.Vg_sn2}")
                    print(f"Vsd_sn2 (Board 1): {self.Vsd_sn2}")


                    self.disable_ui()
                    
                    self.status_label.setText("Calibration Status: Measurement is going on")
                    
                    #if integrate.single_device_connected == True:
                    print("Calibrate Device 1 status", self.shared_state.get_device_connection_state())
                        
                    if self.shared_state.get_device_connection_state():
                        #integrate.Calibrate_Measurement = True
                        #integrate.startmeasurement1 = True
                        integrate.set_Calibrate_Measurement(True)
                        integrate.set_startmeasurement1(True)
                        integrate.startmeasurement1_event.set()

                    print("Started One device measurement Thread From Advance Settings")

                else:
                    self.status_label.setText("Calibration Status: No changes made")
                    self.disable_ui()
                    
            except ValueError as e:
                self.status_label.setText("Calibration Status: Error - Invalid input")
                print(f"[DEBUG] Calibration error: {str(e)}")
        else:
            print("Device is not connected")
            self.status_label.setText("Calibration Status: Device is not connected")
            
    def disable_ui(self):
        self.logout_button.setEnabled(False)
        self.main_window.toggle_sidebar(False)
        self.calibrate_button.setEnabled(False)
        self.time_entry.setEnabled(False)
        for i in range(self.central_layout.count()):
            widget = self.central_layout.itemAt(i).widget()
            if widget:
                widget.setDisabled(True)  # or widget.setEnabled(False)

    def enable_ui(self):
        self.logout_button.setEnabled(True)
        self.main_window.toggle_sidebar(True)
        self.calibrate_button.setEnabled(True)
        self.time_entry.setEnabled(True)
        for i in range(self.central_layout.count()):
            widget = self.central_layout.itemAt(i).widget()
            if widget:
                widget.setEnabled(True)
       
    def reset_all_arrays(self):
        zero_array = [0.0] * 16
        self.Vg_sn1 = zero_array.copy()
        self.Vsd_sn1 = zero_array.copy()
        self.Vg_sn2 = zero_array.copy()
        self.Vsd_sn2 = zero_array.copy()


