import os
import shutil
import sys
import faulthandler
import time
import platform
import subprocess
import integrate
import re
from datetime import datetime, timedelta
from PyQt5.QtWidgets import (
    QApplication, QWidget, QPushButton, QHBoxLayout, QVBoxLayout,
    QLabel, QStackedWidget, QGroupBox, QFrame, QCheckBox,
    QLineEdit, QGridLayout, QMessageBox
)
from PyQt5.QtCore import Qt, pyqtSignal
from PyQt5.QtGui import QIcon, QPixmap, QFont

from Connection_Screen import ConnectionScreen
from Sample_Screen import SampleScreen
from Measurement_Screen import MeasurementScreen
from Callibration_Screen import CallibrationScreen
from Sensor_Calibration import SensorCalibrationScreen
from Settings_Screen import SettingsScreen
from AdvanceSettings_Screen import AdvanceSettingsScreen
from Login_Screen import LoginScreen
from SharedState import SharedState

faulthandler.enable()

def get_app_path():
    base_path = os.path.dirname(sys.executable if getattr(sys, 'frozen', False) else os.path.abspath(__file__))
    if getattr(sys, 'frozen', False):
        while not base_path.endswith("CubexApp"):
            base_path = os.path.dirname(base_path)
    return os.path.normpath(base_path)

CURRENT_APP_PATH = get_app_path()

def get_linux_serial_hex():
    """
    Returns device serial as normalized HEX.
    - Works on Linux (Raspberry Pi)
    - Default value is also converted to HEX
    - Output: 16-char uppercase HEX
    """

    DEFAULT_SERIAL = "4980f87acf6dcfcf"

    # Step 1: Read serial (or use default)
    serial = None

    if platform.system() == "Linux":
        # Method 1: /proc/cpuinfo
        try:
            with open("/proc/cpuinfo", "r") as f:
                for line in f:
                    if line.startswith("Serial"):
                        serial = line.split(":")[1].strip()
                        break
        except Exception:
            pass

        # Method 2: device tree
        if not serial:
            try:
                with open("/sys/firmware/devicetree/base/serial-number", "r") as f:
                    serial = f.read().strip()
            except Exception:
                pass

    # Step 2: If still no serial, use default
    if not serial:
        serial = DEFAULT_SERIAL

    # Step 3: Clean non-HEX chars
    serial = re.sub(r'[^0-9a-fA-F]', '', serial)

    # Step 4: Convert HEX → INT → HEX (normalized)
    try:
        hex_serial = format(int(serial, 16), '016X')
    except ValueError:
        hex_serial = format(int(DEFAULT_SERIAL, 16), '016X')

    return hex_serial

class ClickableLabel(QLabel):
    clicked = pyqtSignal()
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
    def mousePressEvent(self, event):
        self.clicked.emit()
        super().mousePressEvent(event)

class MainWindow(QWidget):
    CloseEvent_Signal = pyqtSignal()
    def __init__(self):
        super().__init__()
        self.username = None        
        self.shared_state = SharedState()
        mvp_version = self.shared_state.get_MVP_Device_Version()
        self.shared_state.set_Raspberrypi_Serial_Number(get_linux_serial_hex())
        print("Raspberry Pi Serial Number:", self.shared_state.get_Raspberrypi_Serial_Number())
        self.screens = {}
        self.CloseEvent_Signal.connect(self.CloseAllResources)
        self.init_ui()

    def CloseAllResources(self):
        print("Application is closing...") 
        
        # Set the flag to signal BrickletThread to exit
        integrate.set_Appclose(True)
       
        # Save DB File
        self.screens["Settings"].save_data_to_db()      
         
        tnow = datetime.now()
        timeout = timedelta(seconds=10)

        while True:
            # If all threads are closed → stop waiting
            if (
                integrate.get_ConnectionThread_Closed() and
                integrate.get_BrickletThread_Closed() and
                integrate.get_TimerThread_Closed() and
                integrate.get_TemperaturThread_Closed() and
                integrate.get_MeasurmentThread_Closed()
            ):
                break

            # Timeout reached → force close temperature thread
            if datetime.now() - tnow > timeout:
                print("Timeout reached while waiting for threads to close.")

                if not integrate.get_TemperaturThread_Closed() and integrate.get_enable_bricklet():
                    integrate.close_Temperature()

                break

            time.sleep(0.05)         

        # Wait for BrickletThread to terminate
        try:
            Connection_screen = self.screens.get("Connect")
            if Connection_screen and hasattr(Connection_screen, 'worker_thread') and Connection_screen.worker_thread.isRunning():
                print("Waiting for worker_thread to terminate...")
                Connection_screen.worker_thread.quit()  # Request thread to quit
                Connection_screen.worker_thread.wait(1000)  # Wait up to 1 second
                print("worker_thread terminated")    

            if Connection_screen and hasattr(Connection_screen, 'connection_thread') and Connection_screen.connection_thread.isRunning():
                print("Waiting for connection_thread to terminate...")
                Connection_screen.connection_thread.quit()  # Request thread to quit
                Connection_screen.connection_thread.wait(1000)  # Wait up to 1 second
                print("connection_thread terminated")  
            
            measurement_screen = self.screens.get("Measure")
            if measurement_screen and hasattr(measurement_screen, 'bricklet_thread') and measurement_screen.bricklet_thread.isRunning():
                print("Waiting for BrickletThread to terminate...")
                measurement_screen.bricklet_thread.quit()  # Request thread to quit
                measurement_screen.bricklet_thread.wait(1000)  # Wait up to 1 second
                print("BrickletThread terminated")

            if measurement_screen and hasattr(measurement_screen, 'timer_thread') and measurement_screen.timer_thread.isRunning():
                print("Waiting for TimerThread to terminate...")
                measurement_screen.timer_thread.quit()  # Request thread to quit
                measurement_screen.timer_thread.wait(1000)  # Wait up to 1 second
                print("TimerThread terminated")

            if measurement_screen and hasattr(measurement_screen, 'TempMonitor_thread') and measurement_screen.TempMonitor_thread.isRunning():
                print("Waiting for TempMonitorThread to terminate...")
                measurement_screen.TempMonitor_thread.quit()  # Request thread to quit
                measurement_screen.TempMonitor_thread.wait(1000)  # Wait up to 1 second
                print("TempMonitorThread terminated")

            if measurement_screen and hasattr(measurement_screen, 'device_thread') and measurement_screen.device_thread.isRunning():
                print("Waiting for DeviceThread to terminate...")
                measurement_screen.device_thread.quit()  
                measurement_screen.device_thread.wait(1000)  
                print("DeviceThread terminated")                

        except Exception as e:
            print(f"Error during thread cleanup: {e}")
               

    def closeEvent(self, event):
        print("Application is closing...")
        self.CloseEvent_Signal.emit()
        event.accept()

        
    def init_ui(self):
        self.left_arrow = QPushButton("◀")
        self.right_arrow = QPushButton("▶")
        self.left_arrow.setEnabled(False)
        self.right_arrow.setEnabled(False)    
        self.sample_label = ClickableLabel("SAMPLE")
        app_version = self.shared_state.get_MVP_Device_Version().split("_MVP")[0].split("V")[-1]
        self.voc_health_label = QLabel(f"Cubex VOC Analyzer System Version {app_version}")
        self.settings_voc_health_label = ClickableLabel(f"Cubex VOC Analyzer System Version {app_version}")
        self.setStyleSheet("background-color: #242746;")
        self.resize(1920, 1080)
        self.center_window()
        
        self.main_layout = QHBoxLayout(self)
        self.main_layout.setContentsMargins(0, 0, 0, 0)
        self.main_layout.setSpacing(0)
        
        self.sidebar_stack = QStackedWidget()
        self.normal_sidebar = self.create_normal_sidebar()
        self.settings_sidebar = self.create_settings_sidebar()
        self.sidebar_stack.addWidget(self.normal_sidebar)
        self.sidebar_stack.addWidget(self.settings_sidebar)
        
        self.stacked_widget = QStackedWidget()
        self.screens = {                         
            "Login": LoginScreen(self),
            "Connect": ConnectionScreen(self, self.username, self.shared_state),
            "Measure": MeasurementScreen(self, self.username, self.shared_state),
            "Sensor_Calibration": SensorCalibrationScreen(self, self.username, self.shared_state),
            "Calibrate": CallibrationScreen(self, self.username, self.shared_state),            
            "Settings": SettingsScreen(self, self.username, self.shared_state),
            "Sample": SampleScreen(self, self.username, self.shared_state),
            "AdvanceSettings": AdvanceSettingsScreen(self, self.username, self.shared_state),
        }
        for screen in self.screens.values():
            self.stacked_widget.addWidget(screen)
            
        #self.screens["Settings"].connect_signals()
        self.main_layout.addWidget(self.sidebar_stack)
        self.main_layout.addWidget(self.stacked_widget)
        
        self.sample_label.clicked.connect(self.on_sample_label_clicked)
        self.settings_voc_health_label.clicked.connect(self.on_voc_health_label_clicked)
        
        # Connect SampleScreen's update_sample_title_signal to update sample_label
        self.screens["Sample"].update_sample_title_signal.connect(self.update_sample_label)
        
        self.update_button_visibility()

    def update_sample_label(self, title: str):
        """Update the sample_label to show current sample / total samples."""
        print("update_sample_label")
        sample_screen = self.screens["Sample"]
        current_sample = sample_screen.current_sample
        max_samples = sample_screen.max_samples
        self.sample_label.setText(f"Sample {current_sample} /{max_samples}")

    def create_normal_sidebar(self):
        sidebar = QWidget()
        sidebar.setStyleSheet("background-color: #141414;")
        sidebar.setFixedWidth(int(self.width() * 0.20))
        
        layout = QVBoxLayout(sidebar)
        layout.setContentsMargins(0, 0, 0, 0)
        layout.setSpacing(20)
        layout.setAlignment(Qt.AlignVCenter | Qt.AlignHCenter)
        layout.addSpacing(50)
        
        btn_style = """
            QPushButton {
                background-color: #2C2F38; 
                color: white; 
                font-size: 18px; 
                font-weight: bold; 
                padding: 10px;
                border: 1px solid #D3D3D3; 
                border-radius: 15px; 
                text-align: center;
            } 
            QPushButton:hover {
                background-color: #333333;
            }
        """
        
        button_labels = [
            ("Login", "login.png"), ("Connect", "ethernet.png"),
            ("Measure", "measurment.png"), ("Sensor_Calibration", "identifyvocs.png"),
            ("Calibrate", "measurment.png"), ("Settings", "settings.png")
        ]
        
        self.buttons = []
        for label, icon in button_labels:
            btn = QPushButton(label, icon=QIcon(os.path.join(CURRENT_APP_PATH, "res", "images", icon)))
            btn.setFixedSize(350, 75)
            btn.setStyleSheet(btn_style)
            
            if label == "AdvanceSettings":
                btn.clicked.connect(self.on_settings_button_clicked)
            else:
                btn.clicked.connect(lambda _, screen_name=label: self.change_screen(screen_name))
            
            container = QVBoxLayout()
            container.setAlignment(Qt.AlignCenter)
            container.addWidget(btn)
            layout.addLayout(container)
            self.buttons.append(btn)
        
        arrow_container = QHBoxLayout()
        arrow_container.setAlignment(Qt.AlignCenter)
        arrow_container.setSpacing(10)
        
        self.left_arrow.setFixedSize(50, 50)
        self.left_arrow.setStyleSheet("""
            QPushButton { 
                background-color: #2C2F38; 
                color: white; 
                font-size: 24px; 
                border: 1px solid #D3D3D3; 
                border-radius: 25px; 
            } 
            QPushButton:hover { 
                background-color: #333333; 
            }
        """)
        
        self.sample_label.setStyleSheet("color: white; font-size: 18px; font-weight: bold;")
        self.sample_label.setAlignment(Qt.AlignCenter)
        self.sample_label.setFixedWidth(150)

        self.right_arrow.setFixedSize(50, 50)
        self.right_arrow.setStyleSheet(self.left_arrow.styleSheet())
        
        arrow_container.addWidget(self.left_arrow)
        arrow_container.addWidget(self.sample_label)
        arrow_container.addWidget(self.right_arrow)
        self.left_arrow.clicked.connect(lambda: self.screens["Sample"].change_sample("prev"))
        self.right_arrow.clicked.connect(lambda: self.screens["Sample"].change_sample("next"))
        
        layout.addSpacing(10)
        layout.addLayout(arrow_container)

        # Add Name, Y-axis, Remove initial time, and DISPLAY dI/I button below arrow_container
        self.display_grid = QGridLayout()
        self.display_grid.setHorizontalSpacing(15)
        self.display_grid.setVerticalSpacing(10)

        line_edit_style = """
            QLineEdit {
                background-color: #333;
                border: 1px solid #555;
                padding: 8px;
                border-radius: 4px;
                color: white;
                font-size: 14px;
            }
        """
        
        button_style = """
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
        """

        self.sample_name_label = self.create_label("Name:")
        self.sample_name_input = QLineEdit("")
        self.y_axis_label = self.create_label("Y-axis:")
        self.y_axis_input = QLineEdit("95,102")
        self.remove_initial_time_label = self.create_label("Remove initial time:")
        self.remove_initial_time_input = QLineEdit("0")
        for widget in [self.sample_name_input, self.y_axis_input, self.remove_initial_time_input]:
            widget.setStyleSheet(line_edit_style)
            widget.setFixedHeight(35)
            widget.setEnabled(False)  # Disabled by default, will be enabled in Sample screen

        self.display_grid.addWidget(self.sample_name_label, 0, 0)
        self.display_grid.addWidget(self.sample_name_input, 0, 1, 1, 3)
        self.display_grid.addWidget(self.y_axis_label, 1, 0)
        self.display_grid.addWidget(self.y_axis_input, 1, 1, 1, 3)
        self.display_grid.addWidget(self.remove_initial_time_label, 2, 0)
        self.display_grid.addWidget(self.remove_initial_time_input, 2, 1, 1, 3)

        self.display_di_i_button = QPushButton("DISPLAY dI/I")
        self.display_di_i_button.setStyleSheet(button_style)
        self.display_di_i_button.setFixedHeight(40)
        self.display_di_i_button.setEnabled(False)  # Disabled by default, will be enabled in Sample screen

        # Initially hide the widgets
        self.sample_name_label.setVisible(False)
        self.sample_name_input.setVisible(False)
        self.y_axis_label.setVisible(False)
        self.y_axis_input.setVisible(False)
        self.remove_initial_time_label.setVisible(False)
        self.remove_initial_time_input.setVisible(False)
        self.display_di_i_button.setVisible(False)

        layout.addLayout(self.display_grid)
        layout.addWidget(self.display_di_i_button)

        layout.addSpacing(10)
        
        box = QGroupBox()
        box.setStyleSheet("""
            background: #2C2F38; 
            border: 1px solid #D3D3D3; 
            border-radius: 5px; 
            padding: 15px;
        """)
        box.setFixedSize(350, 175)
        
        box_layout = QVBoxLayout()
        box_layout.setContentsMargins(10, 10, 10, 10)
        
        icon_label = QLabel(alignment=Qt.AlignCenter)
        icon_label.setPixmap(QPixmap(os.path.join(CURRENT_APP_PATH, "res", "images", 'asset1.png')))
        icon_label.setPixmap(icon_label.pixmap().scaled(100, 100, Qt.KeepAspectRatio, Qt.SmoothTransformation))
        icon_label.setStyleSheet("border: none;")
        self.voc_health_label.setAlignment(Qt.AlignCenter)
        self.voc_health_label.setStyleSheet("color: white; font-size: 12px; font-weight: bold; border: none;")
        box_layout.addWidget(icon_label)
        box_layout.addWidget(self.voc_health_label)
        box.setLayout(box_layout)
        
        layout.addWidget(box, alignment=Qt.AlignBottom)
        layout.addStretch()
        layout.addSpacing(50)
        
        return sidebar

    def create_settings_sidebar(self):
        settings_panel = QWidget()
        settings_panel.setStyleSheet("background-color: #141414; color: white;")
        settings_panel.setFixedWidth(int(self.width() * 0.20))
        
        layout = QVBoxLayout(settings_panel)
        layout.setContentsMargins(20, 20, 20, 20)
        layout.setSpacing(15)
        
        self.add_settings_parameters(layout)
        
        layout.addStretch()
        
        return settings_panel

    def add_settings_parameters(self, layout):
        label_font = QFont()
        label_font.setPointSize(10)
        label_font.setBold(True)
        
        line_edit_style = """
            QLineEdit {
                background-color: #333;
                border: 1px solid #555;
                padding: 8px;
                border-radius: 4px;
                color: white;
                font-size: 14px;
            }
        """
        
        button_style = """
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
        """
        
        self.sensor_sn1 = QLineEdit("23371029")
        self.sensor_sn2 = QLineEdit("23373007")
        
        for widget in [self.sensor_sn1, self.sensor_sn2]:
            widget.setStyleSheet(line_edit_style)
            widget.setFixedHeight(35)
        
        self.add_form_row(layout, "Sensor SN1:", self.sensor_sn1)
        self.add_form_row(layout, "Sensor SN2:", self.sensor_sn2)

        self.set_sensors_button = QPushButton("SET SENSORS")
        self.set_sensors_button.setStyleSheet(button_style)
        self.set_sensors_button.setFixedHeight(40)
        layout.addWidget(self.set_sensors_button)

        layout.addWidget(self.create_separator())
        
        # Check Buttons
        checkbox_layout = QHBoxLayout()
        self.enable_bricklet_check = QCheckBox("Enable Bricklet")
        self.enable_bricklet_check.setStyleSheet("color: white; font-size: 16px;")
        self.enable_bricklet_check.setChecked(False) 
        
        self.presweep_check = QCheckBox("Presweep")
        self.presweep_check.setStyleSheet("color: white; font-size: 16px;")
        self.presweep_check.setChecked(False)  
        
        self.presweep_check.stateChanged.connect(self.on_presweep_changed)

        
        checkbox_layout.addWidget(self.enable_bricklet_check)
        checkbox_layout.addWidget(self.presweep_check)
        layout.addLayout(checkbox_layout)
        
        self.SelectedTemperature_label = QLabel(f"Selected Temperature : {integrate.get_heater_temperature()}")
        self.SelectedTemperature_label.setStyleSheet("color: white; font-size: 16px;")
        layout.addWidget(self.SelectedTemperature_label)
        
        self.vg_min = QLineEdit("-2.0")
        self.t_dwell = QLineEdit("50.00")
        self.vg_max = QLineEdit("0")
        self.d_step = QLineEdit("0.02")
        self.vsd = QLineEdit("0.02")
        self.g_step = QLineEdit("0.02")
        
        for widget in [self.vg_min, self.t_dwell, self.vg_max, self.d_step, self.vsd, self.g_step]:
            widget.setStyleSheet(line_edit_style)
            widget.setFixedHeight(35)
        
        sweep_grid = QGridLayout()
        sweep_grid.setHorizontalSpacing(15)
        sweep_grid.setVerticalSpacing(10)
        
        sweep_grid.addWidget(self.create_label("Vg min:"), 0, 0)
        sweep_grid.addWidget(self.vg_min, 0, 1)
        sweep_grid.addWidget(self.create_label("T dwell:"), 0, 2)
        sweep_grid.addWidget(self.t_dwell, 0, 3)
        
        sweep_grid.addWidget(self.create_label("Vg max:"), 1, 0)
        sweep_grid.addWidget(self.vg_max, 1, 1)
        sweep_grid.addWidget(self.create_label("D step:"), 1, 2)
        sweep_grid.addWidget(self.d_step, 1, 3)
        
        sweep_grid.addWidget(self.create_label("Vsd:"), 2, 0)
        sweep_grid.addWidget(self.vsd, 2, 1)
        sweep_grid.addWidget(self.create_label("G step:"), 2, 2)
        sweep_grid.addWidget(self.g_step, 2, 3)
        
        layout.addLayout(sweep_grid)

        button_layout = QHBoxLayout()
        button_layout.setSpacing(10)
        
        self.set_sweep_button = QPushButton("SET SWEEP")
        self.sweep_button = QPushButton("SWEEP")
        self.set_sweep_button.clicked.connect(self.on_set_sweep_button_clicked)
        
        for btn in [self.set_sweep_button, self.sweep_button]:
            btn.setStyleSheet(button_style)
            btn.setFixedHeight(40)
        
        button_layout.addWidget(self.set_sweep_button)
        button_layout.addWidget(self.sweep_button)
        layout.addLayout(button_layout)

        layout.addWidget(self.create_separator())
        
        self.vg_measure = QLineEdit("-1.5")
        self.vsd_measure = QLineEdit("0.01")
        self.time = QLineEdit("1")
        
        for widget in [self.vg_measure, self.vsd_measure, self.time]:
            widget.setStyleSheet(line_edit_style)
            widget.setFixedHeight(35)
        
        measure_grid = QGridLayout()
        measure_grid.setHorizontalSpacing(15)
        measure_grid.setVerticalSpacing(10)
        
        measure_grid.addWidget(self.create_label("Vg:"), 0, 0)
        measure_grid.addWidget(self.vg_measure, 0, 1)
        measure_grid.addWidget(self.create_label("Vsd:"), 0, 2)
        measure_grid.addWidget(self.vsd_measure, 0, 3)
        measure_grid.addWidget(self.create_label("Time:"), 0, 4)
        measure_grid.addWidget(self.time, 0, 5)
        
        layout.addLayout(measure_grid)

        mbutton_layout = QHBoxLayout()
        mbutton_layout.setSpacing(10)
        
        self.measure_button = QPushButton("MEASURE")
        self.set_measure_button = QPushButton("Set")
        self.measure_button.setStyleSheet(button_style)
        self.measure_button.setFixedHeight(40)
        self.set_measure_button.setStyleSheet(button_style)
        self.set_measure_button.setFixedHeight(40)
        mbutton_layout.addWidget(self.set_measure_button)            
        mbutton_layout.addWidget(self.measure_button)
        layout.addLayout(mbutton_layout)
        layout.addWidget(self.create_separator())
        
        box = QGroupBox()
        box.setStyleSheet("""
            background: #2C2F38; 
            border: 1px solid #D3D3D3; 
            border-radius: 5px; 
            padding: 15px;
        """)
        box.setFixedSize(350, 175)
        
        box_layout = QVBoxLayout()
        box_layout.setContentsMargins(10, 10, 10, 10)
        icon_label = QLabel(alignment=Qt.AlignCenter)
        icon_label.setPixmap(QPixmap(os.path.join(CURRENT_APP_PATH, "res", "images", 'asset1.png')))
        icon_label.setPixmap(icon_label.pixmap().scaled(100, 100, Qt.KeepAspectRatio, Qt.SmoothTransformation))
        icon_label.setStyleSheet("border: none;")
        self.settings_voc_health_label.setAlignment(Qt.AlignCenter)
        self.settings_voc_health_label.setStyleSheet("color: white; font-size: 12px; font-weight: bold; border: none;")
        box_layout.addWidget(icon_label)
        box_layout.addWidget(self.settings_voc_health_label)
        box.setLayout(box_layout)
        
        layout.addWidget(box, alignment=Qt.AlignCenter)

    def on_presweep_changed(self, state):
        if state == Qt.Checked:
            print("Presweep Checked")
            self.shared_state.EnablePresweep_Checkbox = True
        else:
            print("Presweep Unchecked")
            self.shared_state.EnablePresweep_Checkbox = False

    def on_settings_button_clicked(self):
        if self.is_logged_in():
            self.sidebar_stack.setCurrentIndex(1)
            self.change_screen("Settings")

    def on_voc_health_label_clicked(self):
        self.sidebar_stack.setCurrentIndex(0)
        self.show_main_screens()

    def create_label(self, text):
        label = QLabel(text)
        label.setStyleSheet("color: white;")
        return label
    
    def on_set_sweep_button_clicked(self):
        try:
            t_dwell_value = float(self.t_dwell.text())
            d_step_value = float(self.d_step.text())
            g_step_value = float(self.g_step.text()) 
                     
            if hasattr(self.shared_state, 'cube') and self.shared_state.cube is not None:
                self.shared_state.cube.d_time = t_dwell_value
                self.shared_state.cube.d_step = d_step_value
                self.shared_state.cube.g_step = g_step_value
                print(f"Set d_time to: {t_dwell_value}")
            else:
                print("Cube not initialized in shared state")
        except ValueError:
            print("Invalid value in t_dwell field") 

    def add_form_row(self, layout, label_text, widget):
        row = QHBoxLayout()
        label = self.create_label(label_text)
        row.addWidget(label)
        row.addWidget(widget)
        layout.addLayout(row)

    def create_separator(self):
        line = QFrame()
        line.setFrameShape(QFrame.HLine)
        line.setFrameShadow(QFrame.Sunken)
        line.setStyleSheet("color: #444;")
        return line

    def show_normal_sidebar(self):
        self.sidebar_stack.setCurrentIndex(0)


    def on_sample_label_clicked(self):
        print( "Sample label clicked")
        self.left_arrow.setEnabled(self.is_logged_in())
        self.right_arrow.setEnabled(self.is_logged_in())
        self.show_normal_sidebar()
        self.change_screen("Sample")


            
    def center_window(self):
        screen = QApplication.primaryScreen().geometry()
        window_size = self.geometry()
        x = (screen.width() - window_size.width()) // 2
        y = (screen.height() - window_size.height()) // 2
        self.move(x, y)

    def toggle_sidebar(self, enabled):
        self.sidebar_stack.setEnabled(enabled)

    def update_button_visibility(self):
        for btn in self.buttons:
            if btn.text() == "Connect":
                btn.setVisible(self.is_logged_in())
            elif btn.text() == "Login":
                btn.setVisible(not self.is_logged_in())
            elif btn.text() == "Calibrate":  # Check for the Calibrate button
                btn.setVisible(self.is_logged_in() and self.username.lower() != "testtech")  # Hide for testtech
        # self.left_arrow.setEnabled(self.is_logged_in())
        # self.right_arrow.setEnabled(self.is_logged_in())

    def set_username(self, username):
        self.username = username
        for screen in self.screens.values():
            if hasattr(screen, 'username_label'):
                screen.username_label.setText(f"USER: {self.username}")
        self.update_button_visibility()


    def show_main_screens(self):
        self.stacked_widget.setCurrentWidget(self.screens["Connect"])
        # Hide Name, Y-axis, Remove initial time, and DISPLAY dI/I button when not on Sample screen
        self.sample_name_label.setVisible(False)
        self.sample_name_input.setVisible(False)
        self.sample_name_input.setEnabled(False)
        self.y_axis_label.setVisible(False)
        self.y_axis_input.setVisible(False)
        self.y_axis_input.setEnabled(False)
        self.remove_initial_time_label.setVisible(False)
        self.remove_initial_time_input.setVisible(False)
        self.remove_initial_time_input.setEnabled(False)
        self.display_di_i_button.setVisible(False)
        self.display_di_i_button.setEnabled(False)

    def logout(self):
        try:
            self.sidebar_stack.setCurrentIndex(0)
            # Disconnect devices using ConnectionScreen's disconnect_devices method
            self.screens["Connect"].disconnect_devices()
            # Reset username and UI state
            self.username = None
            self.stacked_widget.setCurrentWidget(self.screens["Login"])
            self.update_button_visibility()
            # Hide Name, Y-axis, Remove initial time, and DISPLAY dI/I button on logout
            self.sample_name_label.setVisible(False)
            self.sample_name_input.setVisible(False)
            self.sample_name_input.setEnabled(False)
            self.y_axis_label.setVisible(False)
            self.y_axis_input.setVisible(False)
            self.y_axis_input.setEnabled(False)
            self.remove_initial_time_label.setVisible(False)
            self.remove_initial_time_input.setVisible(False)
            self.remove_initial_time_input.setEnabled(False)
            self.display_di_i_button.setVisible(False)
            self.display_di_i_button.setEnabled(False)
        except Exception as e:
            print(f"Error during logout: {e}")

    def is_logged_in(self):
        return self.username is not None

    def get_current_screen_name(self):
        widget = self.stacked_widget.currentWidget()
        for name, screen in self.screens.items():
            if screen is widget:
                return name
        return None

    def change_screen(self, screen_name):
        print("screen_name",screen_name)
        self.left_arrow.setEnabled(False)
        self.right_arrow.setEnabled(False) 
        sample_screen = self.screens["Sample"]
        if self.is_logged_in() and sample_screen.max_samples > 0:
            # Show and enable Name, Y-axis, Remove initial time, and DISPLAY dI/I button only for Sample screen
            self.sample_name_label.setVisible(screen_name == "Sample")
            self.sample_name_input.setVisible(screen_name == "Sample")
            self.sample_name_input.setEnabled(screen_name == "Sample")
            self.y_axis_label.setVisible(screen_name == "Sample")
            self.y_axis_input.setVisible(screen_name == "Sample")
            self.y_axis_input.setEnabled(screen_name == "Sample")
            self.remove_initial_time_label.setVisible(screen_name == "Sample")
            self.remove_initial_time_input.setVisible(screen_name == "Sample")
            self.remove_initial_time_input.setEnabled(screen_name == "Sample")
            self.display_di_i_button.setVisible(screen_name == "Sample")
            self.display_di_i_button.setEnabled(screen_name == "Sample")
            self.left_arrow.setEnabled(screen_name == "Sample")
            self.right_arrow.setEnabled(screen_name == "Sample")

        if screen_name == "Login":
            self.logout()
            return

        if not self.is_logged_in():
            login_screen = self.screens["Login"]
            login_screen.label_message.setText("✖ Access Denied Please Try to Login")
            login_screen.label_message.setStyleSheet("color: red; font-size: 16px; font-weight: bold;")
            self.stacked_widget.setCurrentWidget(login_screen)
            return

        # Settings Screen Button Click
        if screen_name == "Settings":            
            if "Settings" in self.screens:
                print("Settings Screen Button click")
                
                self.screens["Settings"].connect_signals()
                self.screens["Settings"].status_label.setText("")
                # For Advanced Settings button is visible for the admin login not for testtech
                username = self.username.lower()
                if username == "admin":
                    self.screens["Settings"].advanced_button.setVisible(True)
                elif username == "testtech":
                    self.screens["Settings"].advanced_button.setVisible(False)
                # Update Vg,Vsd Values
                self.screens["Settings"].vg_label.setText(f"Vg: {self.shared_state.var_vg_measure}")
                self.screens["Settings"].vsd_label.setText(f"Vsd: {self.shared_state.var_vsd_measure}")
                self.screens["Settings"].update_heat_controls_visibility()

                self.date_str = datetime.now().strftime('%d_%m_%Y')
                calib_folder = os.path.join('data', self.date_str, 'calibrationData')
                vg_vsd_file = os.path.join(calib_folder, "Vg_Vsd_2set.txt")               
                if not os.path.isfile(vg_vsd_file):
                    self.shared_state.set_Vg_Vsd_calibration_mode(False)
                    self.screens["Settings"].vg_vsd_status_label.setText("Advanced Settings Vg/Vsd")
                    self.screens["Settings"].vg_label.show()
                    self.screens["Settings"].vsd_label.show()          
                    self.screens["Settings"].VgVsd_calibrate_checkbox.hide()          
                else:
                    self.shared_state.set_Vg_Vsd_calibration_mode(True)
                    self.screens["Settings"].VgVsd_calibrate_checkbox.show()
                    if self.screens["Settings"].VgVsd_calibrate_checkbox.isChecked():
                        self.screens["Settings"].vg_vsd_status_label.setText("Calibration Vg/Vsd")                    
                        self.screens["Settings"].vg_label.hide()
                        self.screens["Settings"].vsd_label.hide() 
                    else: 
                        self.screens["Settings"].vg_vsd_status_label.setText("Advanced Settings Vg/Vsd")
                        self.screens["Settings"].vg_label.show()
                        self.screens["Settings"].vsd_label.show()          

        if screen_name == "AdvanceSettings":
            if "AdvanceSettings" not in self.screens:
                print("Not Exist")                
            else:
                print("Already Exist") 
                self.SelectedTemperature_label.setText(f"Selected Temperature : {float(integrate.heater_temperature)}")
                self.screens["AdvanceSettings"].update_status_signal.emit("", "white")
                self.screens["AdvanceSettings"].enable_ui()
                if self.shared_state.Enable_Bricklets:
                    print("Enabled checkbox")
                    self.enable_bricklet_check.setEnabled(True)
                    self.enable_bricklet_check.setChecked(True) 
                elif not self.shared_state.Enable_Bricklets:
                    print("Disabled checkbox")
                    self.enable_bricklet_check.setEnabled(False) 
                    self.enable_bricklet_check.setChecked(False)
        
                
        # Measurement screen Button Click
        if screen_name == "Measure":            
            if "Measure" not in self.screens:
                print("Measurement screen created for the first time.")                
                self.screens["Measure"] = MeasurementScreen(self, self.username, self.shared_state)
                self.stacked_widget.addWidget(self.screens["Measure"])
            else:
                print("Measurement screen already exists, just switching.")
                print("get_temperature_monitor_active",integrate.get_temperature_monitor_active())    
                # Checking the device available space before the measurement
                path = "C:\\" if os.name == "nt" else "/"
                _, _, free = shutil.disk_usage(path)
                free_gb = free / (1024 ** 3)
                print("free Memory: ", free_gb)
                
                if free_gb < 1:
                    self.show_alert("Alert", f"Low Disk Space. Not able to continue the measurement.\nAvailable space: {free_gb:.2f} GB")
                    return       

                rc = self.shared_state.get_continue_measurement()
                if rc:
                    self.show_alert("Alert", "Previous Measurement is not completed")
                    return       

                if not self.screens["Settings"].sn1_edit.text().strip():
                    self.show_alert("Input Error", "SN1 should not be empty")
                    return 

                if not self.screens["Settings"].sn2_edit.text().strip():
                    self.show_alert("Input Error", "SN2 should not be empty")
                    return 

                if not self.screens["Settings"].sample1_edit.text().strip():
                    self.show_alert("Input Error", "Sample ID 1 should not be empty")
                    return 

                if not self.screens["Settings"].sample2_edit.text().strip():
                    self.show_alert("Input Error", "Sample ID 2 should not be empty")
                    return 

                self.shared_state.set_var_Sensor_SN1(self.screens["Settings"].sn1_edit.text().strip())
                self.shared_state.set_var_Sensor_SN2(self.screens["Settings"].sn2_edit.text().strip())
                self.shared_state.set_var_Sample_ID1(self.screens["Settings"].sample1_edit.text().strip())
                self.shared_state.set_var_Sample_ID2(self.screens["Settings"].sample2_edit.text().strip())    

                # Updating T2 Variable
                self.shared_state.updateTvariables_Bricklets()
                integrate.set_Count(0)
                integrate.ttotal = int(integrate.get_t1() + integrate.get_t2() + integrate.get_t3())
                integrate.set_current_argon_flow(0.0)
                if self.shared_state.get_current_test_code():
                    self.screens["Measure"].show_terminal()
                    if self.shared_state.get_device_connection_state():  
                        print("Measurement event button click")
                        integrate.set_view_graph(False)
                        integrate.set_mainmeasurement(True)                        
                        integrate.startmeasurement1_event.set()
                else:
                    msg = QMessageBox()
                    msg.setIcon(QMessageBox.Information)
                    msg.setText("Test code is not selected. Please navigate to the Settings page to set it..")
                    msg.setWindowTitle("Info")
                    msg.setStandardButtons(QMessageBox.Ok)
                    msg.exec_()    
                    return     
                
            self.stacked_widget.setCurrentWidget(self.screens["Measure"])
          
        elif screen_name in ["Connect", "Sample"]:
            if screen_name not in self.screens or self.screens[screen_name] is None:
                self.screens[screen_name] = (ConnectionScreen if screen_name == "Connect" else SampleScreen)(
                    self, self.username, self.shared_state)
                self.stacked_widget.addWidget(self.screens[screen_name])

        if screen_name not in ["Calibrate"]:
            self.stacked_widget.setCurrentWidget(self.screens[screen_name])

        if screen_name == "Sensor_Calibration":
            print("SensorCalibrationScreen Screen Button click")
            self.shared_state.set_Sensor_calibration_measurment_goingon(False)
            self.shared_state.set_Sensor_calibration_Exit_Flag(False)
            self.shared_state.set_Sensor_calibration_measurment_completed(False)
            self.screens["Sensor_Calibration"].update_status.emit("", "white")
            self.screens["Sensor_Calibration"].cleanup_signal.emit() 
            
        self.update_button_visibility()
        # Update the current screen in shared_state
        self.shared_state.set_current_screen(screen_name)
        print(f"Switched to screen: {screen_name}")

    def show_alert(self, title, message):
        msg = QMessageBox()
        msg.setIcon(QMessageBox.Warning)
        msg.setWindowTitle(title)
        msg.setText(message)
        msg.setStandardButtons(QMessageBox.Ok)

        msg.setWindowModality(Qt.ApplicationModal)
        msg.setWindowFlags(msg.windowFlags() | Qt.WindowStaysOnTopHint)
        msg.setWindowFlags(msg.windowFlags() & ~Qt.WindowCloseButtonHint)

        msg.exec_()
