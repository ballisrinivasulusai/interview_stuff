import sys
import os
import time
import threading
import paramiko
import faulthandler
import numpy as np
import pyqtgraph as pg
from TitleBar import TitleBar
from PyQt5.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QHBoxLayout,
    QPushButton, QTextEdit, QLabel, 
    QSizePolicy, QScrollArea, QLineEdit
)
from PyQt5.QtCore import Qt, pyqtSignal, pyqtSlot, QThread, QObject
from datetime import datetime
from scp import SCPClient
from PyQt5.QtGui import QColor
faulthandler.enable()

# Constants for SSH commands
CMDID_SET_FULL_SAMPLE_RATE = 23
CMDID_SET_PREVIOUS_SAMPLE_RATE = 24
REMOTE_PORT = 22  
USERNAME = "root"  
PASSWORD = "voc@123"  
REMOTE_SCRIPT = "/opt/test_interface.py"  

def create_ssh_clients(ip_address):
    ssh = paramiko.SSHClient()
    ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    try:
        ssh.connect(ip_address, port=REMOTE_PORT, username=USERNAME, password=PASSWORD)
    except paramiko.AuthenticationException:
        raise Exception("Authentication failed. Please check your SSH username/password.")
    except Exception as e:
        raise Exception(f"SSH connection failed: {str(e)}")
    
    scp = SCPClient(ssh.get_transport())
    return ssh, scp

# === Run command remotely using SSH ===
def run_remote_command(ssh_client, cmd_id, subcmd_id=None):
    if subcmd_id is not None:
        command = f"python3 {REMOTE_SCRIPT} {cmd_id} {subcmd_id}"
    else:
        command = f"python3 {REMOTE_SCRIPT} {cmd_id}"

    print(f"ðŸš€ Executing remote command: {command}")
    stdin, stdout, stderr = ssh_client.exec_command(command)
    output = stdout.read().decode()
    error = stderr.read().decode()
    if output:
        print("ðŸ“„ Output:\n", output)
    if error:
        print("âš ï¸� Error:\n", error)

class Worker(QObject):
    terminal_message = pyqtSignal(str)
    update_status = pyqtSignal(str, str)  # message, color
    update_plots = pyqtSignal(str, float, float, bool, bool)  # filename, v_min, v_max, do_double_sweep, verbose
    measurement_completed = pyqtSignal(dict, dict, dict, list, str)  # final_Vsd, final_Vg, final_current, temp_files, device_id
    measurement_failed = pyqtSignal(str)  # error message
    finished = pyqtSignal()

    def __init__(self, shared_state, device_ip, device_id, v_min, v_max, vsd, verbose, date_str, shared_timestamp):
        super().__init__()
        self.shared_state = shared_state
        self.device_ip = device_ip
        self.device_id = device_id
        self.v_min = v_min
        self.v_max = v_max
        self.Vsd = vsd
        self.verbose = verbose
        self.cube = shared_state.cube
        self.date_str = date_str
        self.shared_timestamp = shared_timestamp
        self.should_stop = False
        self.ssh = None
        self.scp = None

    def format_message(self, message):
        if message:
            timestamp = datetime.now().strftime("%H:%M:%S")
            return f"[{timestamp}] {self.device_id} ({self.device_ip}): {message}"
        return ""

    def run(self):
        self.should_stop = False
        try:
            # Create SSH and SCP clients
            self.ssh, self.scp = create_ssh_clients(self.device_ip)
            self.terminal_message.emit(self.format_message("SSH connection established"))

            # Run command to set full sample rate
            run_remote_command(self.ssh, 1, 1)
            time.sleep(1)
            run_remote_command(self.ssh, CMDID_SET_FULL_SAMPLE_RATE)
            time.sleep(1)
            run_remote_command(self.ssh, 1, 0)
            self.terminal_message.emit(self.format_message("Set full sample rate command executed"))

            # Perform the sweep
            sweep_file = self.swpVg(self.v_min, self.v_max, float(self.Vsd), do_double_sweep=True)
            
            # Run command to set previous sample rate
            run_remote_command(self.ssh, 1, 1)
            time.sleep(1)
            run_remote_command(self.ssh, CMDID_SET_PREVIOUS_SAMPLE_RATE)
            self.terminal_message.emit(self.format_message("Set previous sample rate command executed"))
            time.sleep(1)
            run_remote_command(self.ssh, 1, 0)

            if sweep_file:
                temp_files = [sweep_file]
                self.measurement_completed.emit({}, {}, {}, temp_files, self.device_id)
            else:
                self.measurement_failed.emit(self.format_message("Sweep measurement failed"))
        except Exception as e:
            self.update_status.emit(f"Measurement Failed on {self.device_id}", "red")
            self.measurement_failed.emit(self.format_message(f"Measurement failed: {str(e)}"))
            self.shared_state.set_Disconnect_Cubedevice(True)  
        finally:
            # Close SSH and SCP connections
            if self.scp:
                self.scp.close()
            if self.ssh:
                self.ssh.close()
            self.terminal_message.emit(self.format_message("SSH connection closed"))
        self.finished.emit()

    def stop(self):
        self.should_stop = True

    def swpVg(self, v_min, v_max, Vsd, do_double_sweep):
        base_path = os.path.join('data', self.date_str)
        os.makedirs(base_path, exist_ok=True)
        status = self.cube.GetDeviceStatus(self.device_ip)
        if not status:
            self.terminal_message.emit(self.format_message("Device not ready"))
            return None
        self.terminal_message.emit(self.format_message("Device is ready"))

        self.terminal_message.emit(self.format_message(f"Setting dwell time to {self.cube.d_time}"))
        rc = self.cube.SetDwell_Time_Value(self.device_ip, self.cube.d_time)
        self.terminal_message.emit(self.format_message("Dwell time set successfully" if rc else "Failed to set dwell time"))

        self.terminal_message.emit(self.format_message(f"Setting d_step0 to {self.cube.d_step}"))
        rc = self.cube.Setd_step0Value(self.device_ip, self.cube.d_step)
        self.terminal_message.emit(self.format_message("d_step0 set successfully" if rc else "Failed to set d_step0"))

        self.terminal_message.emit(self.format_message(f"Setting g_step0 to {self.cube.g_step}"))
        rc = self.cube.Setg_step0Value(self.device_ip, self.cube.g_step)
        self.terminal_message.emit(self.format_message("g_step0 set successfully" if rc else "Failed to set g_step0"))

        self.terminal_message.emit(self.format_message("Applying DAC channel values..."))
        rc = self.cube.ApplyDACChannels(self.device_ip)
        self.terminal_message.emit(self.format_message("DAC values applied successfully" if rc else "Failed to update DACs"))

        dac_value = Vsd
        self.terminal_message.emit(self.format_message(f"Setting all Vsd to 1-16 channels to {dac_value} V"))
        for dac_channel in range(0, 16):
            rc = self.cube.SetDAC1ChannelValue(self.device_ip, dac_channel + 1, float(dac_value))
            self.terminal_message.emit(self.format_message(f"Failed for DAC1 channel {dac_channel + 1}, value {dac_value}") if not rc else "")
            rc = self.cube.SetDAC2ChannelValue(self.device_ip, dac_channel + 1, float(dac_value))
            self.terminal_message.emit(self.format_message(f"Failed for DAC2 channel {dac_channel}, value {dac_value}") if not rc else "")
        rc = self.cube.ApplyDACChannels(self.device_ip)
        dac1_value = self.cube.GetDAC1ChannelValue(self.device_ip, 1)
        self.terminal_message.emit(self.format_message(f"DAC1 = {dac1_value}"))
        dac2_value = self.cube.GetDAC2ChannelValue(self.device_ip, 1)
        self.terminal_message.emit(self.format_message(f"DAC2 = {dac2_value}"))

        dac_value = v_min
        for dac_channel in range(16, 32):
            rc = self.cube.SetDAC1ChannelValue(self.device_ip, dac_channel + 1, float(dac_value))
            self.terminal_message.emit(self.format_message(f"Failed for DAC1 channel {dac_channel + 1}, value {dac_value}") if not rc else "")
            rc = self.cube.SetDAC2ChannelValue(self.device_ip, dac_channel + 1, float(dac_value))
            self.terminal_message.emit(self.format_message(f"Failed for DAC2 channel {dac_channel}, value {dac_value}") if not rc else "")
        rc = self.cube.ApplyDACChannels(self.device_ip)
        dac1_value = self.cube.GetDAC1ChannelValue(self.device_ip, 17)
        self.terminal_message.emit(self.format_message(f"DAC1 = {dac1_value}"))
        dac2_value = self.cube.GetDAC2ChannelValue(self.device_ip, 17)
        self.terminal_message.emit(self.format_message(f"DAC2 = {dac2_value}"))

        x_axis_time = (0.001 * self.cube.d_time) * ((v_max - v_min) / self.cube.g_step)
        sampling_time = x_axis_time + 2
        self.terminal_message.emit(self.format_message(f"Sampling time: {sampling_time} s"))
        t0 = time.time()
        self.terminal_message.emit(self.format_message("Starting Data Acquisition..."))
        rc = self.cube.StartDataAcq(self.device_ip, 2 * sampling_time if do_double_sweep else sampling_time)
        if not rc:
            self.terminal_message.emit(self.format_message("Failed to start Data Acquisition"))
            return None

        dac_value = v_max
        self.terminal_message.emit(self.format_message(f"Updating dac_value to {dac_value} V"))
        for dac_channel in range(16, 32):
            rc = self.cube.SetDAC1ChannelValue(self.device_ip, dac_channel + 1, float(dac_value))
            self.terminal_message.emit(self.format_message(f"Failed for DAC1 channel {dac_channel + 1}, value {dac_value}") if not rc else "")
            rc = self.cube.SetDAC2ChannelValue(self.device_ip, dac_channel + 1, float(dac_value))
            self.terminal_message.emit(self.format_message(f"Failed for DAC2 channel {dac_channel}, value {dac_value}") if not rc else "")
        rc = self.cube.ApplyDACChannels(self.device_ip)
        self.terminal_message.emit(self.format_message("Failed to update DACs" if not rc else ""))

        while (time.time() < t0 + sampling_time) and not self.should_stop:
            time.sleep(0.1)

        dac1_value = self.cube.GetDAC1ChannelValue(self.device_ip, 1)
        self.terminal_message.emit(self.format_message(f"DAC1 after ramp up = {dac1_value}"))
        dac2_value = self.cube.GetDAC2ChannelValue(self.device_ip, 1)
        self.terminal_message.emit(self.format_message(f"DAC2 after ramp up = {dac2_value}"))

        if do_double_sweep:
            dac_value = v_min
            for dac_channel in range(16, 32):
                rc = self.cube.SetDAC1ChannelValue(self.device_ip, dac_channel + 1, float(dac_value))
                self.terminal_message.emit(self.format_message(f"Failed for DAC1 channel {dac_channel + 1}, value {dac_value}") if not rc else "")
                rc = self.cube.SetDAC2ChannelValue(self.device_ip, dac_channel + 1, float(dac_value))
                self.terminal_message.emit(self.format_message(f"Failed for DAC2 channel {dac_channel}, value {dac_value}") if not rc else "")
            rc = self.cube.ApplyDACChannels(self.device_ip)
            self.terminal_message.emit(self.format_message("Failed to update DACs" if not rc else ""))

            while (time.time() < t0 + 2 * sampling_time) and not self.should_stop:
                time.sleep(0.1)

        self.terminal_message.emit(self.format_message("Aborting Measurement..."))
        self.cube.AbortMeasurement(self.device_ip)

        self.terminal_message.emit(self.format_message("Transferring Raw file..."))
        output_file = f'IVSWP_{v_min}_{v_max}_rawtext1_{self.shared_timestamp}'
        self.terminal_message.emit(self.format_message(f"Output file: {output_file}"))
        remote_path = os.path.join('data', self.date_str, output_file)
        local_file_path = os.path.join('data', self.date_str, output_file)

        self.terminal_message.emit(self.format_message(f"Current working directory: {os.getcwd()}"))
        self.terminal_message.emit(self.format_message(f"Expected local file path: {local_file_path}"))

        rc = self.cube.StartTransferMeasurementFile(self.device_ip, remote_path)
        if not rc:
            self.terminal_message.emit(self.format_message(f"Failed to transfer measurement file {remote_path} from Cube device"))
            return None

        local_file_path = self.wait_for_file(local_file_path)
        if local_file_path:
            self.update_plots.emit(local_file_path, v_min, v_max, do_double_sweep, True)
            self.update_status.emit(f"File transferred and plotted for {self.device_id}", "white")
        return local_file_path

    def wait_for_file(self, file_path, timeout=10):
        start_time = time.time()
        if not os.path.exists(file_path):
            data_folder_path = os.path.join('data', self.date_str, os.path.basename(file_path))
            if os.path.exists(data_folder_path):
                return data_folder_path
        while not os.path.exists(file_path) and not self.should_stop:
            if time.time() - start_time > timeout:
                self.terminal_message.emit(self.format_message(f"File {file_path} not found after {timeout} seconds"))
                return None
            time.sleep(0.5)
        try:
            with open(file_path, 'rb') as f:
                data = f.read()
                if len(data) < 100:
                    self.terminal_message.emit(self.format_message(f"File {file_path} is too small or corrupted"))
                    return None
        except Exception as e:
            self.terminal_message.emit(self.format_message(f"Error reading {file_path}: {str(e)}"))
            return None
        return file_path

class MeasurementThread(QThread):
    terminal_message = pyqtSignal(str)
    update_status = pyqtSignal(str, str)
    update_plots = pyqtSignal(str, float, float, bool, bool)
    measurement_completed = pyqtSignal(dict, dict, dict, list, str)
    measurement_failed = pyqtSignal(str)

    def __init__(self, shared_state, device_ip, device_id, v_min, v_max, vsd, verbose, date_str, shared_timestamp):
        super().__init__()
        self.shared_state = shared_state
        self.device_ip = device_ip
        self.device_id = device_id
        self.v_min = v_min
        self.v_max = v_max
        self.vsd = vsd
        self.verbose = verbose
        self.date_str = date_str
        self.shared_timestamp = shared_timestamp
        self.worker = None
        self.worker_thread = None

    def run(self):
        self.worker = Worker(self.shared_state, self.device_ip, self.device_id, self.v_min, self.v_max, self.vsd, self.verbose, self.date_str, self.shared_timestamp)
        self.worker_thread = QThread()
        self.worker.moveToThread(self.worker_thread)
        self.worker.terminal_message.connect(self.terminal_message)
        self.worker.update_status.connect(self.update_status)
        self.worker.update_plots.connect(self.update_plots)
        self.worker.measurement_completed.connect(self.measurement_completed)
        self.worker.measurement_failed.connect(self.measurement_failed)
        self.worker.finished.connect(self.worker_thread.quit)
        self.worker_thread.started.connect(self.worker.run)
        self.worker_thread.start()

    def stop(self):
        if self.worker:
            self.worker.stop()
        if self.worker_thread and self.worker_thread.isRunning():
            self.worker_thread.quit()
            self.worker_thread.wait()

class IdentifyVOCScreen(TitleBar):
    sample_changed = pyqtSignal(str)
    terminal_message = pyqtSignal(str)
    button_states_changed = pyqtSignal()
    update_plots = pyqtSignal(str, float, float, bool, bool)
    update_status = pyqtSignal(str, str)
    toggle_sidebar_signal = pyqtSignal(bool)

    def __init__(self, main_window, username, shared_state):
        super().__init__(main_window, username, "IDENTIFY VOC")
        self.shared_state = shared_state
        self.main_window = main_window
        self.current_sample = 0
        self.max_samples = 0
        self.sample_info = []
        self._app_path = None
        self.date_str = datetime.now().strftime('%d_%m_%Y')
        self.sample_voltages = {}
        self.v_min_sensor1_device1 = np.full(16, -2.0)
        self.v_min_sensor2_device1 = np.full(16, -2.0)
        self.sweep_workers = {}
        self.device1_event = threading.Event()
        self.device_results = {}
        self.temp_files = []
        self.total_start_time = None
        self.initUI()
        self.setup_signals()
        self.update_available_samples()

    def enable_ui(self):
        self.logout_button.setEnabled(True)
        self.toggle_sidebar_signal.emit(True)
        self.action_buttons_widget.setEnabled(True)
        self.bottom_bar.setEnabled(True)

    def disable_ui(self):
        self.logout_button.setEnabled(False)
        self.toggle_sidebar_signal.emit(False)
        self.action_buttons_widget.setEnabled(False)
        self.bottom_bar.setEnabled(False)

    def setup_signals(self):
        self.sample_changed.connect(self.change_sample)
        self.terminal_message.connect(self.append_to_terminal)
        self.button_states_changed.connect(self.update_button_states)
        self.update_plots.connect(self.update_plots_slot)
        self.update_status.connect(self.update_status_label)
        self.toggle_sidebar_signal.connect(self.main_window.toggle_sidebar)

    def update_plots_slot(self, filename, v_min, v_max, do_double_sweep, verbose):
        self.plot_sweep_results(filename, v_min, v_max, do_double_sweep, verbose, chip_sel=None)

    def get_app_path(self):
        if self._app_path is None:
            if getattr(sys, 'frozen', False):
                app_dir = os.path.dirname(sys.executable)
                self._app_path = app_dir[:app_dir.rfind("dist")].rstrip(os.sep) if "dist" in app_dir else app_dir
            else:
                self._app_path = os.path.dirname(os.path.abspath(__file__))
        return self._app_path

    def extract_timestamp(self, filename):
        try:
            parts = filename.split('_')
            if len(parts) >= 3:
                return parts[-2] + parts[-1].split('.')[0]
        except (AttributeError, IndexError, TypeError) as e:
            print("Error",e)
            pass
        return "0"
   
    
    def check_date_and_update(self):
        base_path = os.path.join(self.get_app_path(), 'data')
        # Get today's folder name (only the date string)
        today_folder_name = datetime.now().strftime('%d_%m_%Y')

        # Build expected full folder path
        expected_path = os.path.join(base_path, today_folder_name)

        # If day changed? Create new folder & update path
        if expected_path != self.today_date_folder_path:
            print("New day detected. Updating folder...")

            # Update the variable
            self.today_date_folder_path = expected_path
            os.makedirs(self.today_date_folder_path, exist_ok=True)

            # Also create inside application data folder
            app_data_folder = os.path.join(self.get_app_path(), 'data', today_folder_name)

            if not os.path.exists(app_data_folder):
                os.makedirs(app_data_folder)
                print(f"New date folder created: {app_data_folder}")

    def update_available_samples(self):
        base_path = os.path.join(self.get_app_path(), 'data', self.date_str)
        os.makedirs(base_path, exist_ok=True)
        self.today_date_folder_path = base_path
        self.sample_info.clear()

        if os.path.exists(self.today_date_folder_path):
            files = [f for f in os.listdir(self.today_date_folder_path) if f.startswith('IVSWP_') and 'rawtext1' in f]
            files.sort(key=self.extract_timestamp, reverse=True)

            for filename in files:
                self.sample_info.extend([(filename, i) for i in (1, 2)])

        self.max_samples = len(self.sample_info)
        self.current_sample = 0 if self.max_samples > 0 else 0
        self.update_sample_display()
        self.button_states_changed.emit()

        # Automatically plot the most recent IVSWP_ file if available
        if self.sample_info:
            filename, chip_sel = self.get_current_sample_info()
            if filename and filename.startswith('IVSWP_'):
                # Extract v_min and v_max from filename (e.g., IVSWP_-2.0_0.0_rawtext1_...)
                try:
                    parts = filename.split('_')
                    v_min = float(parts[1])  # e.g., -2.0
                    v_max = float(parts[2])  # e.g., 0.0
                except (IndexError, ValueError):
                    v_min, v_max = -2.0, 0.0  # Default values
                self.plot_sweep_results(filename, v_min, v_max, True, True, chip_sel)

    def get_current_sample_info(self):
        return self.sample_info[self.current_sample] if self.sample_info and self.current_sample < self.max_samples else (None, None)

    def append_to_terminal(self, message):
        if message:
            try:                
                self.terminal.append(message)
                scrollbar = self.terminal.verticalScrollBar()
                scrollbar.setValue(scrollbar.maximum())
                QApplication.processEvents()
            except RuntimeError as e:
                print(f"Error in append_to_terminal: {str(e)}")

    def update_status_label(self, message, color):
        if color.lower() == "green":
            color = "white"
        self.status_label.setText(message)
        self.status_label.setStyleSheet(f"color: {color}; font-size: 14px; padding: 5px;")
        QApplication.processEvents()

    def update_sample_display(self):
        self.sample_label.setText(
            "NO SAMPLES" if self.max_samples == 0 else
            f"SAMPLE {self.current_sample + 1}/{self.max_samples}"
        )
        if self.max_samples == 0:
            self.title_label_item.setText("No samples available", color='white', size='15pt bold')
        QApplication.processEvents()

    def update_button_states(self):
        left_button = self.bottom_bar.layout().itemAt(1).widget()
        right_button = self.bottom_bar.layout().itemAt(3).widget()
        left_button.setEnabled(self.current_sample > 0)
        right_button.setEnabled(self.current_sample < self.max_samples - 1)
        QApplication.processEvents()

    def initUI(self):
        main_layout = self.layout()       
                
        self.status_label = QLabel("")
        self.status_label.setStyleSheet("color: white; font-size: 14px; padding: 5px;")
        self.status_label.setAlignment(Qt.AlignCenter)
        main_layout.addWidget(self.status_label, alignment=Qt.AlignCenter)

        scroll_area = QScrollArea()
        scroll_area.setWidgetResizable(True)
        scroll_area.setStyleSheet("background-color: #242746; border: none;")
        scroll_area.setMinimumSize(900, 900)

        container_widget = QWidget()
        container_layout = QVBoxLayout()
        container_layout.setAlignment(Qt.AlignTop)
        container_widget.setLayout(container_layout)
        container_widget.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)

        self.graph_widget = pg.GraphicsLayoutWidget()
        self.graph_widget.setBackground("#242746")
        self.graph_widget.setMinimumSize(800, 800)
        container_layout.addWidget(self.graph_widget, stretch=1)

        self.plot_items = []
        self.initialize_plots()

        self.init_bottom_bar()
        container_layout.addWidget(self.bottom_bar)

        self.terminal = QTextEdit()
        self.terminal.setReadOnly(True)
        self.terminal.setStyleSheet("background-color: black; color: white; font-family: monospace; font-size: 12px;")
        self.terminal.setMinimumHeight(250)
        container_layout.addWidget(self.terminal)

        self.init_action_buttons()
        container_layout.addWidget(self.action_buttons_widget)

        scroll_area.setWidget(container_widget)
        main_layout.addWidget(scroll_area, stretch=1)

        self.terminal_message.emit("Started VOC Identification System")

    def initialize_plots(self):
        self.graph_widget.clear()
        self.plot_items = []
        self.title_label_item = pg.LabelItem(justify='center')
        self.title_label_item.setText("", color='white', size='15pt bold')
        self.graph_widget.addItem(self.title_label_item, row=0, colspan=4)

        for i in range(4):
            row = []
            for j in range(4):
                p = self.graph_widget.addPlot(row=i+1, col=j)
                p.showGrid(x=True, y=True, alpha=0.1)
                view_box = p.getViewBox()
                view_box.setBackgroundColor('#ffffff')
                p.getAxis('left').setPen(pg.mkPen('k'))
                p.getAxis('bottom').setPen(pg.mkPen('k'))
                p.setLabel('left', 'I_SD [nA]', color='white', **{'font-size': '12pt', 'font-weight': 'bold'})
                p.setLabel('bottom', 'V_g [V]', color='white', **{'font-size': '12pt', 'font-weight': 'bold'})
                p.setMinimumSize(150, 150)
                row.append(p)
            self.plot_items.append(row)

    def init_bottom_bar(self):
        self.bottom_bar = QWidget()
        self.bottom_bar.setStyleSheet("""
            QWidget { background-color: #242746; padding: 10px; border-top: 1px solid #444; }
            QPushButton { 
                color: black; background-color: #242746; 
                border: 1px solid #555; border-radius: 4px; 
                padding: 5px 15px; min-width: 80px; 
            }
            QPushButton:hover { background-color: #e0e0e0; }
            QPushButton:pressed { background-color: #c0c0c0; }
            QPushButton:disabled { color: #888; background-color: #f0f0f0; }
        """)

        layout = QHBoxLayout()
        layout.setContentsMargins(20, 5, 20, 5)
        layout.setSpacing(20)

        left_arrow_button = QPushButton("◄")
        left_arrow_button.setFixedSize(40, 40)
        right_arrow_button = QPushButton("►")
        right_arrow_button.setFixedSize(40, 40)
        self.sample_label = QPushButton("NO SAMPLES")
        self.sample_label.setEnabled(False)
        self.sample_label.setFixedSize(200, 40)

        left_arrow_button.clicked.connect(lambda: self.sample_changed.emit("prev"))
        right_arrow_button.clicked.connect(lambda: self.sample_changed.emit("next"))

        layout.addStretch()
        layout.addWidget(left_arrow_button)
        layout.addWidget(self.sample_label)
        layout.addWidget(right_arrow_button)
        layout.addStretch()

        self.bottom_bar.setLayout(layout)

    def init_action_buttons(self):
        self.action_buttons_widget = QWidget()
        self.action_buttons_widget.setStyleSheet("""
            QWidget { background-color: #242746; padding: 10px; }
            QPushButton { 
                color: black; background-color: #ffffff; 
                border: 1px solid #555; border-radius: 4px; 
                padding: 5px 15px; min-width: 80px; 
            }
            QPushButton:hover { background-color: #e0e0e0; }
            QPushButton:pressed { background-color: #c0c0c0; }
        """)

        layout = QHBoxLayout()
        layout.setContentsMargins(20, 5, 20, 5)
        layout.setSpacing(20)

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
        label_style = """
            QLabel {
                color: white;
                font-size: 14px;
            }
        """

        label_vsd = QLabel("Initial VSD:")
        label_vsd.setStyleSheet(label_style)
        layout.addWidget(label_vsd)
        
        self.initial_vsd_edit = QLineEdit("0.02")
        self.initial_vsd_edit.setMaximumWidth(100)
        self.initial_vsd_edit.setStyleSheet(line_edit_style)
        layout.addWidget(self.initial_vsd_edit)

        label_target_current = QLabel("Target Current:")
        label_target_current.setStyleSheet(label_style)
        layout.addWidget(label_target_current)

        self.target_current_edit = QLineEdit("300")
        self.target_current_edit.setMaximumWidth(100)
        self.target_current_edit.setStyleSheet(line_edit_style)
        layout.addWidget(self.target_current_edit)

        run_measurement_button = QPushButton("Run Test")
        run_measurement_button.clicked.connect(self.run_measurement1)

        layout.addStretch()
        layout.addWidget(run_measurement_button)
        layout.addStretch()

        self.action_buttons_widget.setLayout(layout)

    def is_plot_valid(self, plot):
        return plot is not None and hasattr(plot, 'plot')

    def plot_Vg_sweep_by_array(self, out1, volt_start, volt_stop, chip_sel0=1, filename=None, display_sensor_num=None):
        subarray_list = [0, 2, 4, 6, 8, 10, 12, 14, 1, 3, 9, 11, 15, 5, 7, 13]
        array_display_order = [1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16]

        if out1 is None or out1.size == 0:
            self.terminal_message.emit("No valid data to plot")
            return

        num_chip_sel0 = (chip_sel0 - 1) % 2
        sensor_offset = 256 * num_chip_sel0

        filename_str = str(filename)
        filename_parts = filename_str.split("_")
        Measurementtype = filename_parts[0] if filename_parts else 'IV'

        for row in self.plot_items:
            for plot in row:
                plot.clear()

        # Ensure volt_start and volt_stop are valid numbers
        volt_start = float(volt_start) if volt_start is not None else -2.0
        volt_stop = float(volt_stop) if volt_stop is not None else 0.0

        if Measurementtype == 'IV' and 'Time' in filename_str:
            try:
                scan_rate = 1200
                rows = out1.shape[1]
                x0 = np.arange(rows, dtype=float) / scan_rate

                for plot_index, array_num in enumerate(array_display_order):
                    row = plot_index // 4
                    col = plot_index % 4
                    k_use0 = subarray_list[plot_index]
                    tmp_list0 = np.arange(16 * k_use0 + sensor_offset, 16 * (k_use0 + 1) + sensor_offset, dtype=int)

                    count_t = 0
                    for k1 in tmp_list0:
                        if k1 >= out1.shape[0]:
                            continue
                        try:
                            tmp0 = out1[k1].copy() * 1e9 + count_t
                            tmp0[tmp0 > 800] = np.nan
                            if np.isnan(tmp0).all():
                                continue
                            if self.is_plot_valid(self.plot_items[row][col]):
                                self.plot_items[row][col].plot(x0, tmp0, pen=pg.mkPen('b', width=1))
                            count_t += 0
                        except Exception as e:
                            self.terminal_message.emit(f"Error plotting array {array_num}, device {k1}: {e}")
                            continue

                    if self.is_plot_valid(self.plot_items[row][col]):
                        self.plot_items[row][col].setTitle(f'Array {array_num}', color='white', size='12pt bold')
                        self.plot_items[row][col].setYRange(0, 750)
                        self.plot_items[row][col].setXRange(0, rows / scan_rate)
                        self.plot_items[row][col].setLabel('left', 'I_SD [nA]', color='white', **{'font-size': '12pt', 'font-weight': 'bold'})
                        self.plot_items[row][col].setLabel('bottom', 'Time [s]', color='white', **{'font-size': '12pt', 'font-weight': 'bold'})
            except Exception as e:
                self.terminal_message.emit(f"Error setting axes properties: {e}")
        else:
            try:
                rows = out1.shape[1]
                x0 = np.linspace(volt_start, volt_stop, rows)
                colors = [QColor.fromHsv(int(360 * i / 16), 255, 255) for i in range(16)]

                for plot_index, array_num in enumerate(array_display_order):
                    row = plot_index // 4
                    col = plot_index % 4
                    k_use0 = subarray_list[plot_index]
                    tmp_list0 = np.arange(16 * k_use0 + sensor_offset, 16 * (k_use0 + 1) + sensor_offset, dtype=int)

                    count_t = 0
                    color_count = 0

                    for k1 in tmp_list0:
                        if k1 >= out1.shape[0]:
                            continue
                        try:
                            tmp0 = out1[k1].copy() * 1e9 + count_t
                            tmp0[tmp0 > 800] = np.nan
                            if np.isnan(tmp0).all():
                                continue
                            if self.is_plot_valid(self.plot_items[row][col]):
                                self.plot_items[row][col].plot(x0, tmp0, pen=pg.mkPen(colors[color_count], width=1))
                            count_t += 0
                            color_count += 1
                        except Exception as e:
                            self.terminal_message.emit(f"Error plotting array {array_num}, device {k1}: {e}")
                            continue

                    if self.is_plot_valid(self.plot_items[row][col]):
                        self.plot_items[row][col].setTitle(f'Array {array_num}', color='white', size='12pt bold')
                        self.plot_items[row][col].setYRange(0, 800)
                        self.plot_items[row][col].setXRange(volt_start, volt_stop)
                        self.plot_items[row][col].setLabel('left', 'I_SD [nA]', color='white', **{'font-size': '12pt', 'font-weight': 'bold'})
                        self.plot_items[row][col].setLabel('bottom', 'V_g [V]', color='white', **{'font-size': '12pt', 'font-weight': 'bold'})
            except Exception as e:
                self.terminal_message.emit(f"Error setting axes properties: {e}")

        try:
            if filename:
                self.title_label_item.setText(f"{filename} - Sensor {display_sensor_num}", color='white', size='15pt bold')
        except Exception as e:
            self.terminal_message.emit(f"Error setting window title: {e}")

    def plot_sweep_results(self, sweep_output_file, v_min, v_max, do_double_sweep, verbose, chip_sel=1):
        try:
            # Validate v_min and v_max with default values
            v_min = float(v_min) if v_min is not None and isinstance(v_min, (int, float)) else -2.0
            v_max = float(v_max) if v_max is not None and isinstance(v_max, (int, float)) else 0.0

            if not sweep_output_file.startswith(os.path.join('data', self.date_str)):
                data_path = os.path.join('data', self.date_str, sweep_output_file)
            else:
                data_path = sweep_output_file

            if not os.path.exists(data_path):
                self.terminal_message.emit(f"File {data_path} does not exist")
                return None

            out_swp, scan_rate = self.get_raw_data(data_path, verbose=verbose)
            if out_swp is None or out_swp.size == 0:
                self.terminal_message.emit(f"Failed to load data from {data_path}")
                return None

            if verbose:
                if 'SWP' in sweep_output_file or 'sweep' in sweep_output_file.lower():
                    self.plot_Vg_sweep_by_array(out_swp, v_min, v_max, chip_sel0=chip_sel, filename=sweep_output_file, display_sensor_num=chip_sel)
                else:
                    self.plot_Vg_sweep_by_array(out_swp, 0, 0, chip_sel0=chip_sel, filename=sweep_output_file, display_sensor_num=chip_sel)
            return out_swp
        except Exception as e:
            self.terminal_message.emit(f"Error in plot_sweep_results: {str(e)}")
            return None

    def get_raw_data(self, fname0, filter_bad_ch=False, fullpath=False, verbose=False):
        num_sensors = 2
        if not fullpath and not fname0.startswith(os.path.join('data', self.date_str)):
            fname0 = os.path.join('data', self.date_str, fname0)
        if not os.path.exists(fname0):
            self.terminal_message.emit(f"File {fname0} does not exist")
            return None, 0

        try:
            out0 = np.fromfile(fname0, dtype="single")
            if out0.size == 0:
                self.terminal_message.emit(f"File {fname0} is empty or corrupted")
                return None, 0

            with open(fname0, 'rb') as fid1:                
                try:
                    head_int1 = np.fromfile(fid1, dtype=np.uint32, count=10)
                except UnicodeDecodeError:
                    self.terminal_message.emit(f"Warning: Could not decode barcode in {fname0}, using default value")
        except Exception as e:
            self.terminal_message.emit(f"Error reading {fname0}: {str(e)}")
            return None, 0

        scan_rate = head_int1[8] if len(head_int1) > 8 else 1200  # Default scan rate if not available

        if scan_rate <= 0:
            self.terminal_message.emit(f"Invalid scan rate {scan_rate} in {fname0}, using default 1200")
            scan_rate = 1200

        columns = num_sensors * 256
        rows = int(out0.size / columns) if out0.size > 0 else 0
        if rows == 0:
            self.terminal_message.emit(f"Empty data in {fname0}")
            return None, scan_rate

        try:
            out1 = out0.reshape((rows, columns))[1:].T
            channel_list = self.create_subarray_list()
            if channel_list.size == 0:
                self.terminal_message.emit(f"Invalid channel list for {fname0}")
                return None, scan_rate
            out1 = out1[channel_list]
            out1 = np.minimum(out1, 650 * 1e-9)

            if filter_bad_ch:
                out1 = self.filter_outliers(out1)

            return out1, scan_rate
        except Exception as e:
            self.terminal_message.emit(f"Error processing data from {fname0}: {str(e)}")
            return None, scan_rate

    def create_subarray_list(self):
        subarray_list = [0, 8, 1, 9, 2, 10, 3, 11, 4, 12, 5, 13, 6, 14, 7, 15]
        final_ls = [
            np.hstack([np.arange(16 * k + 256 * chip_sel, 16 * (k + 1) + 256 * chip_sel, dtype=int)
                       for k in subarray_list])
            for chip_sel in [1, 0]
        ]
        return np.hstack(final_ls)

    def filter_outliers(self, out):
        if out is None or out.size == 0:
            return out
        row_min = np.min(out, axis=1)
        row_max = np.max(out, axis=1)
        row_mean = np.mean(out, axis=1)
        valid_rows = (row_min >= 5e-9) & (row_max <= 5e-6) & (row_mean >= 20e-9)
        filtered_out = np.where(valid_rows[:, None], out, np.nan)
        return filtered_out

    @pyqtSlot(str)
    def change_sample(self, direction):
        if direction == "prev" and self.current_sample > 0:
            self.current_sample -= 1
        elif direction == "next" and self.current_sample < self.max_samples - 1:
            self.current_sample += 1
        self.update_sample_display()
        self.button_states_changed.emit()
        filename, chip_sel = self.get_current_sample_info()
        if filename:
            # Extract v_min and v_max from filename for IVSWP_ files
            try:
                parts = filename.split('_')
                v_min = float(parts[1]) if filename.startswith('IVSWP_') else -2.0
                v_max = float(parts[2]) if filename.startswith('IVSWP_') else 0.0
            except (IndexError, ValueError):
                v_min, v_max = -2.0, 0.0
            self.plot_sweep_results(filename, v_min, v_max, True, True, chip_sel)

    def run_measurement1(self):
        self.terminal.clear()
        self.terminal_message.emit("Starting measurement...")
        self.disable_ui()  # Disable UI at the start
        self.shared_state.identifyVOC_measurement = True
        self.temp_files = []
        # Check the Current date Folder is present or not.
        self.check_date_and_update()   
        if not self.shared_state.get_device_connection_state():
            self.update_status.emit("No device connected!", "red")
            self.terminal_message.emit("No device connected!")
            self.enable_ui()  # Re-enable UI if no device
            return

        v_min = -2.0
        v_max = 0.0
        verbose = True
        try:
            initial_vsd = float(self.initial_vsd_edit.text())
        except ValueError:
            self.update_status.emit("Invalid input for Initial VSD!", "red")
            self.enable_ui()  # Re-enable UI on invalid input
            return
        self.total_start_time = time.time()

        self.device_results = {}
        self.threads = []
        shared_timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")

        thread1 = MeasurementThread(self.shared_state, self.shared_state.device1_ip, "Device 1", v_min, v_max, initial_vsd, verbose, self.date_str, shared_timestamp)
        thread1.terminal_message.connect(self.terminal_message)
        thread1.update_status.connect(self.update_status)
        thread1.update_plots.connect(self.update_plots)
        thread1.measurement_completed.connect(self.handle_measurement_completed)
        thread1.measurement_failed.connect(self.handle_measurement_failed)
        self.threads.append(thread1)
        thread1.start()

    @pyqtSlot(dict, dict, dict, list, str)
    def handle_measurement_completed(self, final_Vsd, final_Vg, final_current, temp_files, device_id):
        self.device_results[device_id] = (final_Vsd, final_Vg, final_current)
        self.temp_files.extend(temp_files)
        self.terminal_message.emit(f"Measurement completed for {device_id}")
        self.update_status.emit(f"Measurement completed for {device_id}", "white")
        self.enable_ui()  # Re-enable UI on completion
        self.terminal_message.emit("All measurements completed")
        self.update_status.emit("All measurements completed successfully", "white")
        total_time = time.time() - self.total_start_time
        self.terminal_message.emit(f"Total measurement time: {total_time:.2f} seconds")
        self.shared_state.identifyVOC_measurement = False
        self.update_available_samples()
        self.device1_event.set()
        for thread in self.threads:
            thread.stop()
        self.threads.clear()

    @pyqtSlot(str)
    def handle_measurement_failed(self, error_message):
        self.terminal_message.emit(error_message)
        self.update_status.emit("Measurement failed!", "red")
        self.enable_ui()  # Re-enable UI on failure
        self.shared_state.identifyVOC_measurement = False
        self.device1_event.set()
        for thread in self.threads:
            thread.stop()
        self.threads.clear()
   



