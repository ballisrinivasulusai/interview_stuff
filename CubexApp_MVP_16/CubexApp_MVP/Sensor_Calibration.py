import sys
import os
import shutil
import time
import threading
import csv
import paramiko
import faulthandler
import numpy as np
import pyqtgraph as pg
import integrate
import tarfile
from TitleBar import TitleBar
from PyQt5.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QHBoxLayout,
    QPushButton, QTextEdit, QLabel,
    QSizePolicy, QScrollArea, QLineEdit,QMessageBox
)
from PyQt5.QtCore import Qt, pyqtSignal, pyqtSlot, QThread, QObject
from datetime import datetime
from scp import SCPClient
from PyQt5.QtGui import QColor
from integrate import VOC_Solenoid_Valve_on, VOC_Solenoid_Valve_off, Exhaust_Solenoid_Valve_on,Exhaust_Solenoid_Valve_off, get_enable_bricklet, get_sv1_relay, MFC_open_nitrogen_flow, set_mfc_zero, get_Argon_flow_rate
faulthandler.enable()
# Constants for SSH
REMOTE_PORT = 22
USERNAME = "root"
PASSWORD = "voc@123"
REMOTE_SCRIPT = "/opt/chip_calibration_controller.py"
REMOTE_CALIB_FOLDER = "/opt" # SM_ and CM_ binary files are generated here
REMOTE_TMP_FOLDER = "/media/media/measurement/tmp"
SCP_GET_HARD_TIMEOUT_SEC = 7  # Hard timeout for each scp.get() in seconds
def create_ssh_clients(ip_address):
    ssh = paramiko.SSHClient()
    ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    
    try:
        ssh.connect(ip_address,port=REMOTE_PORT,username=USERNAME,password=PASSWORD,timeout=10,look_for_keys=False,allow_agent=False)
    except paramiko.AuthenticationException:
        raise Exception("Authentication failed. Please check your SSH username/password.")
    except Exception as e:
        raise Exception(f"SSH connection failed: {str(e)}")

    # Get transport and enable keepalive
    transport = ssh.get_transport()
    transport.set_keepalive(5)  
    transport.sock.settimeout(15)  
    scp = SCPClient(transport)
    return ssh, scp
    
def run_remote_command(self,ssh_client, command): 
    print(f"Executing remote command: {command}")
    stdin, stdout, stderr = ssh_client.exec_command(command)
    """
    check whether chip_calibration_controller.py is still running.
    """
    while not stdout.channel.exit_status_ready():     
        if self.should_stop:
            print("stopping remote command due to network down")
            try:
                ssh_client.close()
            except:
                pass
            return "","stopped by cleanup"
        time.sleep(0.5)

    output = stdout.read().decode()
    error = stderr.read().decode()
    if output:
        print("Output:\n", output)
    if error:
        print("Error:\n", error)
    return output, error
class Worker(QObject):
    terminal_message = pyqtSignal(str)
    update_status = pyqtSignal(str, str)
    update_plots = pyqtSignal(str, float, float, bool, bool)
    measurement_completed = pyqtSignal(list, str)
    measurement_failed = pyqtSignal(str)
    finished = pyqtSignal()
    def __init__(self, shared_state, device_ip, device_id, initial_vsd, target_current,volt_start, volt_stop, date_str):
        super().__init__()
        self.shared_state = shared_state
        self.device_ip = device_ip
        self.device_id = device_id
        self.initial_vsd = initial_vsd
        self.target_current = target_current
        self.volt_start = volt_start 
        self.volt_stop = volt_stop
        self.date_str = date_str
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
        transferred_files = []

        local_tmp_folder = None      

        try:
            # --------------------------------------------------
            # SSH connection
            # --------------------------------------------------
            self.ssh, self.scp = create_ssh_clients(self.device_ip)
            self.terminal_message.emit(self.format_message("SSH connection established"))

            # --------------------------------------------------
            # Run remote measurement script
            # --------------------------------------------------
            command = (
                f"python3 {REMOTE_SCRIPT} -keepIVg "
                f"--initial_vsd {self.initial_vsd} "
                f"--target_current {self.target_current} "
                f"--volt_start {self.volt_start} "
                f"--volt_stop {self.volt_stop}"
            )

            # Bricklet implementations (14/01/26)
            if integrate.get_enable_bricklet():
                integrate.sv1_relay = get_sv1_relay()
                if (not Exhaust_Solenoid_Valve_on(integrate.sv1_relay)):
                    print("<BT> Failed to set Exhaust Solenoid Valve to ON/Open 2f3R SW0")
                    self.terminal_message.emit("<BT> Failed to set Exhaust Solenoid Valve to ON/Open 2f3R SW0")
                    self.shared_state.set_failed_status("<BT> Failed to set Exhaust Solenoid Valve to ON/Open 2f3R SW0 <Bricklet Failure>")
                    self.shared_state.set_measurement_failed(True)
                    return False

                print(f" Turn on Exhaust Solenoid Valve")
                self.terminal_message.emit(f" Turn on Exhaust Solenoid Valve")

                for serial_port, vserial_port in zip(integrate.nirogen_serial_ports, integrate.voc_serial_ports):
                    MFC_open_nitrogen_flow(integrate.mfc_sfc6, serial_port, vserial_port, get_Argon_flow_rate(), None)

            self.terminal_message.emit(self.format_message(f"Running: {command}"))

            output, error = run_remote_command(self,self.ssh, command)  
            # --------------------------------------------------
            # MFC and Solenoid Valve turn off (14/01/26)
            # ------------------------------------------------
            if integrate.get_enable_bricklet():
                for serial_port in integrate.serial_ports:
                    print(f"Serial Port = {serial_port}")
                    if not set_mfc_zero(integrate.mfc_sfc6, serial_port):
                        print(f"<BT> Failed to set MFC <{serial_port}> to 0%")
                        self.terminal_message.emit(f"<BT> Failed to set MFC <{serial_port}> to 0%")
                        self.shared_state.set_failed_status(f"<BT> Failed to set MFC <{serial_port}> to 0% <Bricklet Failure>")
                        self.shared_state.set_measurement_failed(True)

                integrate.sv1_relay = get_sv1_relay()
                if not Exhaust_Solenoid_Valve_off(integrate.sv1_relay):
                    print("<BL> Failed to set Exhaust Solenoid Valve to OFF/Closed 2f3R SW0")
                    self.terminal_message.emit("<BL> Failed to set Exhaust Solenoid Valve to OFF/Closed 2f3R SW0")
                    self.shared_state.set_failed_status("<BT> Failed to set Exhaust Solenoid Valve to OFF/Closed 2f3R SW0 <Bricklet Failure>")
                    self.shared_state.set_measurement_failed(False)
                    return False

                print(f" Turn off Exhaust Solenoid Valve")
                self.terminal_message.emit(f" Turn off Exhaust Solenoid Valve")

            if "RuntimeWarning" in error and "Mean of empty slice" in error:
                self.terminal_message.emit(
                    self.format_message("Warning (ignored): Mean of empty slice in averaging")
                )
            elif error.strip() and "Traceback" in error:
                self.terminal_message.emit(
                    self.format_message(f"Remote script failed:\n{error}")
                )
                self.measurement_failed.emit("Remote script crashed")
                return

            self.terminal_message.emit(self.format_message("Measurement script completed"))

            # --------------------------------------------------
            # Prepare local calibration folder
            # --------------------------------------------------
            #today_folder = os.path.join('data', self.date_str)
            #calib_folder = os.path.join(today_folder, 'calibrationData')
            #os.makedirs(calib_folder, exist_ok=True)
            BASE_DIR = os.path.dirname(os.path.abspath(__file__))
            today_folder = os.path.join(BASE_DIR, 'data', self.date_str)
            calib_folder = os.path.join(today_folder, 'calibrationData')
            os.makedirs(calib_folder, exist_ok=True)
            # --------------------------------------------------
            # Create LOCAL timestamped TMP folder
            # --------------------------------------------------
            timestamp = time.strftime("%Y%m%d_%H%M%S")
            local_tmp_folder = os.path.join(today_folder, f"tmp_{timestamp}")
            os.makedirs(local_tmp_folder, exist_ok=True)

            # --------------------------------------------------
            # Scan remote tmp folder
            # --------------------------------------------------
            self.terminal_message.emit(
                self.format_message("Scanning tmp folder for supporting files...")
            )

            _, stdout, _ = self.ssh.exec_command(
                f"ls -1 {REMOTE_TMP_FOLDER} 2>/dev/null || echo ''"
            )
            tmp_files = [f.strip() for f in stdout.readlines() if f.strip()]

            # --------------------------------------------------
            # Collect target files
            # --------------------------------------------------
            target_files = []
            vg_vsd_2set_files = []

            for f in tmp_files:
                if (
                    (f.startswith('Avg_curr_') and f.endswith('.txt')) or
                    (f.startswith('CM_rawtext_')) or
                    (f.startswith('dev_status_') and (f.endswith('.json') or f.endswith('.npy'))) or
                    (f.startswith('SM_') and 'rawtext' in f)
                ):
                    target_files.append((os.path.join(REMOTE_TMP_FOLDER, f), f))

                elif f.startswith('Vg_Vsd_2set_') and f.endswith('.txt'):
                    vg_vsd_2set_files.append(f)

            # --------------------------------------------------
            # Handle Vg_Vsd_2set files
            # --------------------------------------------------
            if vg_vsd_2set_files:
                vg_vsd_2set_files.sort()
                latest_vg_vsd = vg_vsd_2set_files[-1]
                remote_latest_path = os.path.join(REMOTE_TMP_FOLDER, latest_vg_vsd)

                target_files.append((remote_latest_path, latest_vg_vsd))
                target_files.append((remote_latest_path, 'Vg_Vsd_2set.txt'))

                self.terminal_message.emit(
                    self.format_message(f"Latest Vg_Vsd selected: {latest_vg_vsd}")
                )
            else:
                self.terminal_message.emit(
                    self.format_message("No Vg_Vsd_2set file found")
                )

            # --------------------------------------------------
            # Transfer files into LOCAL TMP folder
            # --------------------------------------------------
            transferred_count = 0
            transferred_files = []
            transferred_tfiles = []
            aborted = False

            for remote_path, local_name in target_files:
                if self.should_stop:                            
                    print("Measurement stopped by cleanup")
                    aborted = True
                    break

                local_tmp_path = os.path.join(local_tmp_folder, local_name)
                try:
                    print(f"Transferring {local_name}")
                    self.terminal_message.emit(self.format_message(f"Transferring {local_name}"))
                    self.scp.get(remote_path, local_tmp_path)
                    transferred_files.append(local_tmp_path)
                    transferred_tfiles.append(local_name)
                    transferred_count += 1
                    time.sleep(0.5)
                except Exception as e:
                    print(f"Failed to transfer {local_name}: {str(e)}")
                    self.terminal_message.emit(
                        self.format_message(f"Failed to transfer {local_name}: {str(e)}")
                    )


            # --------------------------------------------------
            # Validate count = 7 then MOVE or DELETE TMP
            # --------------------------------------------------
            REQUIRED_FILE_COUNT = 7

            if not aborted and transferred_count == REQUIRED_FILE_COUNT:
                for f in transferred_files:
                    self.terminal_message.emit(
                        self.format_message(f"source {f} Moving files {calib_folder} {os.path.basename(f)}")
                    )
                    shutil.move(f, os.path.join(calib_folder, os.path.basename(f)))

                # Define upload_data folder path
                upload_folder = os.path.join(today_folder, "upload_data")

                # Check if folder exists, if not create
                if not os.path.exists(upload_folder):
                    os.makedirs(upload_folder)

                # Create tar file name
                timestamp = self.shared_state.get_timestamp()
                timestamped_filename = f"Sensor_Calib_{timestamp}"
                tar_filename = timestamped_filename + ".tar"
                tar_path = os.path.join(calib_folder, tar_filename)

                # Step 1: Create tar inside calib_folder
                with tarfile.open(tar_path, "w") as tar:
                    for file_name in transferred_tfiles:
                        file_path = os.path.join(calib_folder, file_name)
                
                        self.terminal_message.emit(
                            self.format_message(f"Adding file to tar {file_path} {tar_path} {file_name} {calib_folder}")
                        )

                        if os.path.exists(file_path):
                            tar.add(file_path, arcname=file_name)
                        else:
                            print(f"Warning: {file_name} not found in calib_folder")

                print("Tar file created successfully!", upload_folder, tar_filename, tar_path)
                self.terminal_message.emit(
                    self.format_message(f"upload_folder {upload_folder} filename {tar_filename}, target path {tar_path}")
                )


                # Step 2: Copy tar to upload_folder
                destination_path = os.path.join(upload_folder, tar_filename)
                shutil.copy(tar_path, destination_path)

                print("Tar file copied to upload_folder!")

                # Step 3: Remove tar from calib_folder
                #os.remove(tar_path)

                print("Tar file removed from calib_folder!")

                shutil.rmtree(local_tmp_folder)

                print("All 7 files transferred successfully. TMP folder removed.")
                self.terminal_message.emit(
                    self.format_message("All 7 files transferred successfully. TMP folder removed.")
                )
            else:
                shutil.rmtree(local_tmp_folder)
                print(f"Incomplete transfer ({transferred_count}/7). TMP folder deleted. CalibrationData untouched.")
                self.terminal_message.emit(
                    self.format_message(
                        f"Incomplete transfer ({transferred_count}/7). TMP folder deleted. CalibrationData untouched."
                    )
                )
                self.measurement_failed.emit("Incomplete file transfer")
                print("Incomplete file transfer")
                return

            print("Successfully Recived All the files")

            # ======================================================
            # CSV LOGGING — ONLY CALIBRATION SWEEP
            # ======================================================
            csv_filename = f"Experiment_list_calibration_{self.date_str}.csv"
            csv_file_path = os.path.join(calib_folder, csv_filename)

            if not os.path.isfile(csv_file_path):
                with open(csv_file_path, 'w', newline='') as f:
                    csv.writer(f).writerow([
                        "File Name",
                        "File Type",
                        "Date",
                        "Time",
                        "Associated Sweep Calibration",
                        "SN1 Number",
                        "SN2 Number",
                        "Sample 1 ID",
                        "Sample 2 ID",
                        "Raspberry Pi Serial Number",
                        "Board ID",
                        "Software version"
                    ])

            sm_file = None
            for f in transferred_files:
                name = os.path.basename(f)
                if name.startswith("SM_"):
                    sm_file = name
                    break

            """
            Zaid NewReq
            """
            sn1_number = self.shared_state.get_var_Sensor_SN1()
            sn2_number = self.shared_state.get_var_Sensor_SN2()

            if sm_file:
                sm_parts = sm_file.replace(".txt", "").split('_')
                sm_date = sm_parts[-2]
                sm_time = sm_parts[-1]
            else:
                sm_file = f"Advanced Settings Vsd Value {self.shared_state.var_vsd_measure}"
                sm_date = "-"
                sm_time = "-"

            with open(csv_file_path, 'a', newline='') as f:
                csv.writer(f).writerow([
                    sm_file,
                    "Calibration sweep",
                    sm_date,
                    sm_time,
                    "-",
                    sn1_number,
                    sn2_number
                ])

            # -----------------
            # ADD INPUT VALUES
            # -----------------
            if vg_vsd_2set_files:
                input_block = (
                    "Input values\n"
                    f"Initial Vsd : {self.initial_vsd}\n"
                    f"Target current: {self.target_current}\n"
                    f"volt_start: {self.volt_start}\n"
                    f"volt_stop: {self.volt_stop}\n\n"
                )

                local_timestamped_file = os.path.join(calib_folder, latest_vg_vsd)

                if os.path.exists(local_timestamped_file):
                    with open(local_timestamped_file, "r") as rf:
                        original_content = rf.read()
                    with open(local_timestamped_file, "w") as wf:
                        wf.write(input_block + original_content)

                self.measurement_completed.emit(transferred_files, self.device_id)

            else:
                self.measurement_failed.emit("Vg_Vsd_2set file handling failed")
            print("Sensor Calibrate Successfully completed")

        except Exception as e:
            self.measurement_failed.emit(self.format_message(f"Measurement failed: {str(e)}"))
            
            if integrate.get_enable_bricklet():
                for serial_port in integrate.serial_ports:
                    print(f"Serial Port = {serial_port}")
                    if not set_mfc_zero(integrate.mfc_sfc6, serial_port):
                        print(f"<BT> Failed to set MFC <{serial_port}> to 0%")
                        self.terminal_message.emit(f"<BT> Failed to set MFC <{serial_port}> to 0%")
                        self.shared_state.set_failed_status(f"<BT> Failed to set MFC <{serial_port}> to 0% <Bricklet Failure>")
                        self.shared_state.set_measurement_failed(True)

                integrate.sv1_relay = get_sv1_relay()
                if not Exhaust_Solenoid_Valve_off(integrate.sv1_relay):
                    print("<BL> Failed to set Exhaust Solenoid Valve to OFF/Closed 2f3R SW0")
                    self.terminal_message.emit("<BL> Failed to set Exhaust Solenoid Valve to OFF/Closed 2f3R SW0")
                    self.shared_state.set_failed_status("<BT> Failed to set Exhaust Solenoid Valve to OFF/Closed 2f3R SW0 <Bricklet Failure>")
                    self.shared_state.set_measurement_failed(False)
                    return False            
            
        finally:
            # ....TMP FOLDER REMOVE...               
            if local_tmp_folder and os.path.exists(local_tmp_folder):
                try:
                    shutil.rmtree(local_tmp_folder)
                    print("TMP folder  deleted script_command  running time network down ")
                except Exception as e:
                    print("TMP folder deletes failed ",e)

            if self.scp:
                self.scp.close()
            if self.ssh:
                self.ssh.close()

            self.terminal_message.emit(self.format_message("SSH connection closed"))
            self.finished.emit()


    def stop(self):
        self.should_stop = True

        try:
            if self.scp:
                self.scp.close()
            if self.ssh:
                self.ssh.close()
        except:
            pass


class MeasurementThread(QThread):
    terminal_message = pyqtSignal(str)
    update_status = pyqtSignal(str, str)
    update_plots = pyqtSignal(str, float, float, bool, bool)
    measurement_completed = pyqtSignal(list, str)
    measurement_failed = pyqtSignal(str)

    def __init__(self, shared_state, device_ip, device_id, initial_vsd, target_current,volt_start, volt_stop, date_str):
        super().__init__()
        self.shared_state = shared_state
        self.device_ip = device_ip
        self.device_id = device_id
        self.initial_vsd = initial_vsd
        self.target_current = target_current
        self.volt_stop = volt_stop
        self.date_str = date_str

        self.worker = Worker(shared_state, device_ip, device_id, initial_vsd, target_current,volt_start, volt_stop, date_str)
        self.worker.moveToThread(self)  # Safe: done in main thread during init

        self.worker.terminal_message.connect(self.terminal_message)
        self.worker.update_status.connect(self.update_status)
        self.worker.update_plots.connect(self.update_plots)
        self.worker.measurement_completed.connect(self.measurement_completed)
        self.worker.measurement_failed.connect(self.measurement_failed)
        self.worker.finished.connect(self.quit)

        self.started.connect(self.worker.run)

    def stop(self):
        if hasattr(self.worker, 'stop'):
            self.worker.stop()
        print("befour measurementThread-sensor_calibration  cancle")
        self.quit()
        self.wait()
        print("after measurementThred-sensor_calibration cancle")

class SensorCalibrationScreen(TitleBar):
    sample_changed = pyqtSignal(str)
    terminal_message = pyqtSignal(str)
    button_states_changed = pyqtSignal()
    update_plots = pyqtSignal(str, float, float, bool, bool)
    update_status = pyqtSignal(str, str)
    toggle_sidebar_signal = pyqtSignal(bool)
    cleanup_signal = pyqtSignal()
    def __init__(self, main_window, username, shared_state):
        super().__init__(main_window, username, "SENSOR CALIBRATION")
        self.shared_state = shared_state
        self.main_window = main_window
        self.current_sample = 0
        self.max_samples = 0
        self.sample_info = [] # List of (filename, chip_sel) # chip_sel=1 for sensor1, 2 for sensor2
        self._app_path = None
        self.date_str = datetime.now().strftime('%d_%m_%Y')
        self.total_start_time = None
        self.initUI()
        self.setup_signals()
        self.update_available_samples()
        self.threads = []
    def enable_ui(self):
        self.logout_button.setEnabled(True)
        self.toggle_sidebar_signal.emit(True)
        self.action_buttons_widget.setEnabled(True)
        self.bottom_bar.setEnabled(True)
        self.left_arrow_button.setEnabled(True)
        self.right_arrow_button.setEnabled(True)

        
    def disable_ui(self):
        self.logout_button.setEnabled(False)
        self.toggle_sidebar_signal.emit(False)
        self.action_buttons_widget.setEnabled(False)
        self.bottom_bar.setEnabled(False)
        self.left_arrow_button.setEnabled(False)
        self.right_arrow_button.setEnabled(False)        

        
    def setup_signals(self):
        self.sample_changed.connect(self.change_sample)
        self.terminal_message.connect(self.append_to_terminal)
        self.button_states_changed.connect(self.update_button_states)
        self.update_plots.connect(self.update_plots_slot)
        self.update_status.connect(self.update_status_label)
        self.toggle_sidebar_signal.connect(self.main_window.toggle_sidebar)
        self.cleanup_signal.connect(self.cleanup)
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
    def check_date_and_update(self):
        base_path = os.path.join(self.get_app_path(), 'data')
        today_folder_name = datetime.now().strftime('%d_%m_%Y')
        expected_path = os.path.join(base_path, today_folder_name)
        if expected_path != getattr(self, 'today_date_folder_path', None):
            print("New day detected. Updating folder...")
            self.today_date_folder_path = expected_path
            os.makedirs(self.today_date_folder_path, exist_ok=True)
    def extract_timestamp(self, filename):
        try:
            parts = filename.split('_')
            if len(parts) >= 3:
                date_part = parts[-2]
                time_part = parts[-1].split('.')[0]
                timestamp_str = f"{date_part}_{time_part}"
                return datetime.strptime(timestamp_str, "%Y%m%d_%H%M%S")
        except:
            pass
        return datetime.min
    def update_available_samples(self):
        base_path = os.path.join(self.get_app_path(), 'data', self.date_str, 'calibrationData')
        os.makedirs(base_path, exist_ok=True)
        self.today_date_folder_path = base_path
        self.sample_info.clear()
        if os.path.exists(self.today_date_folder_path):
            #files = [f for f in os.listdir(self.today_date_folder_path) if f.startswith('SM_') and 'rawtext1' in f]
            files = [f for f in os.listdir(self.today_date_folder_path)if ((f.startswith('CM_') or f.startswith('SM_')) and('rawtext' in f or 'rawtext1' in f))]
            files.sort(key=self.extract_timestamp, reverse=True)
            print("Found SM_ rawtext files:", files)
            for filename in files:
                self.sample_info.extend([(filename, i) for i in (1, 2)])
        self.max_samples = len(self.sample_info)
        self.current_sample = 0 if self.max_samples > 0 else 0
        self.update_sample_display()
        self.button_states_changed.emit()
        # Automatically plot the most recent IVSWP_ file if available
        if self.sample_info:
            filename, chip_sel = self.get_current_sample_info()
            if filename and filename.startswith('SM_') or filename.startswith('CM_'):
                # Extract v_min and v_max from filename (e.g., IVSWP_-2.0_0.0_rawtext1_...)
                try:
                    parts = filename.split('_')
                    v_min = float(parts[1]) # e.g., -2.0
                    v_max = float(parts[2]) # e.g., 0.0
                except (IndexError, ValueError):
                    v_min, v_max = -2.0, 0.0 # Default values
                self.plot_sweep_results(filename, v_min, v_max, True, True, chip_sel)
    def get_current_sample_info(self):
        if self.sample_info and 0 <= self.current_sample < self.max_samples:
            return self.sample_info[self.current_sample]
        return None, None
    def append_to_terminal(self, message):
        if message:
            try:
                self.terminal.append(message)
                scrollbar = self.terminal.verticalScrollBar()
                scrollbar.setValue(scrollbar.maximum())
                QApplication.processEvents()
            except RuntimeError as e:
                print(f"Error in append_to_terminal: {str(e)}")

                if integrate.get_enable_bricklet():
                    for serial_port in integrate.serial_ports:
                        print(f"Serial Port = {serial_port}")
                        if not set_mfc_zero(integrate.mfc_sfc6, serial_port):
                            print(f"<BT> Failed to set MFC <{serial_port}> to 0%")
                            self.terminal_message.emit(f"<BT> Failed to set MFC <{serial_port}> to 0%")
                            self.shared_state.set_failed_status(f"<BT> Failed to set MFC <{serial_port}> to 0% <Bricklet Failure>")
                            self.shared_state.set_measurement_failed(True)
                            # Off the Exhaust Valve

                    integrate.sv1_relay = get_sv1_relay()
                    if not Exhaust_Solenoid_Valve_off(integrate.sv1_relay):
                        print("<BL> Failed to set Exhaust Solenoid Valve to OFF/Closed 2f3R SW0")
                        self.terminal_message.emit("<BL> Failed to set Exhaust Solenoid Valve to OFF/Closed 2f3R SW0")
                        self.shared_state.set_failed_status("<BT> Failed to set Exhaust Solenoid Valve to OFF/Closed 2f3R SW0 <Bricklet Failure>")
                        self.shared_state.set_measurement_failed(False)
                        return False                
                
    def update_status_label(self, message, color):
        if color.lower() == "green":
            color = "white"
        self.status_label.setText(message)
        self.status_label.setStyleSheet(f"color: {color}; font-size: 14px; padding: 5px;")
        QApplication.processEvents()
    def update_sample_display(self):
        if self.max_samples == 0:
            self.sample_label.setText("NO SAMPLES")
            self.title_label_item.setText("No valid samples", color='white', size='15pt bold')
        else:
            filename, chip_sel = self.get_current_sample_info()
            base_name = os.path.basename(filename)
            sensor_str = "-sensor1" if chip_sel == 1 else "-sensor2"
            display_name = f"{base_name}{sensor_str}"
            self.sample_label.setText(f"SAMPLE {self.current_sample + 1}/{self.max_samples}")
            self.title_label_item.setText(display_name, color='white', size='15pt bold')
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
        self.terminal.setMinimumHeight(350)
        container_layout.addWidget(self.terminal)
        self.init_action_buttons()
        container_layout.addWidget(self.action_buttons_widget)
        scroll_area.setWidget(container_widget)
        main_layout.addWidget(scroll_area, stretch=1)
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
                view_box.setMouseEnabled(x=False, y=False)
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
        self.left_arrow_button = QPushButton("◄")
        self.left_arrow_button.setFixedSize(40, 40)
        self.right_arrow_button = QPushButton("►")
        self.right_arrow_button.setFixedSize(40, 40)
        self.sample_label = QPushButton("NO SAMPLES")
        self.sample_label.setEnabled(False)
        self.sample_label.setFixedSize(200, 40)
        self.left_arrow_button.clicked.connect(lambda: self.sample_changed.emit("prev"))
        self.right_arrow_button.clicked.connect(lambda: self.sample_changed.emit("next"))
        layout.addStretch()
        layout.addWidget(self.left_arrow_button)
        layout.addWidget(self.sample_label)
        layout.addWidget(self.right_arrow_button)
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
        self.target_current_edit = QLineEdit("350")
        self.target_current_edit.setMaximumWidth(100)
        self.target_current_edit.setStyleSheet(line_edit_style)
        layout.addWidget(self.target_current_edit)
        label_volt_start = QLabel("Volt Start:")
        label_volt_start.setStyleSheet(label_style)
        layout.addWidget(label_volt_start)
        self.volt_start_edit = QLineEdit("0.5")
        self.volt_start_edit.setMaximumWidth(100)
        self.volt_start_edit.setStyleSheet(line_edit_style) 
        layout.addWidget(self.volt_start_edit)    
        label_volt_stop = QLabel("Volt Stop:")
        label_volt_stop.setStyleSheet(label_style)
        layout.addWidget(label_volt_stop)
        self.volt_stop_edit = QLineEdit("-2")
        self.volt_stop_edit.setMaximumWidth(100)
        self.volt_stop_edit.setStyleSheet(line_edit_style)
        layout.addWidget(self.volt_stop_edit)
        run_measurement_button = QPushButton("Run Test")
        run_measurement_button.clicked.connect(self.run_measurement1)
        layout.addStretch()
        layout.addWidget(run_measurement_button)
        layout.addStretch()
        self.action_buttons_widget.setLayout(layout)
    def is_plot_valid(self, plot):
        return plot is not None and hasattr(plot, 'plot')
    def plot_calibration_sweep(self, filepath, chip_sel=1):
        try:
            out_swp, scan_rate = self.get_raw_data(filepath, verbose=True)
            if out_swp is None or out_swp.size == 0:
                self.terminal_message.emit(f"Empty or invalid data in {os.path.basename(filepath)} (size: {os.path.getsize(filepath)/1024:.1f} KB)")
                return
            filename = os.path.basename(filepath)
            volt_start, volt_stop = 0.5, -3.0
            if filename.startswith('SM_'):
                parts = filename.split('_')
                if len(parts) >= 3:
                    try:
                        volt_stop = float(parts[1])
                        volt_start = float(parts[2])
                    except:
                        pass
            self.plot_Vg_sweep_by_array(out_swp, volt_start, volt_stop, chip_sel0=chip_sel,
                                       filename=filename, display_sensor_num=chip_sel)
        except Exception as e:
            self.terminal_message.emit(f"Plot error: {str(e)}")
            
    def plot_Vg_sweep_by_array(self, out1, volt_start, volt_stop, chip_sel0=1,scan_rate = 1200, filename=None, display_sensor_num=None):
        #subarray_list = [0, 2, 4, 6, 8, 10, 12, 14, 1, 3, 9, 11, 15, 5, 7, 13]
        subarray_list = [0, 8, 1, 9, 2, 10, 3, 11, 4, 12, 5, 13, 6, 14, 7, 15]
        array_display_order = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
        #array_display_order = [1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16]
        is_time_plot = filename is not None and filename.startswith("CM_")
        if out1 is None or out1.size == 0:
            return
        sensor_offset = 256 * ((chip_sel0 - 1) % 2)
        for row in self.plot_items:
            for plot in row:
                plot.clear()
        volt_start = float(volt_start)
        volt_stop = float(volt_stop)
        try:
            rows = out1.shape[1]
            if is_time_plot:
                # CM plot → Time axis in seconds starting from 0
                x0 = np.arange(out1.shape[1], dtype=float) / scan_rate
            else:
                # Vg sweep → Voltage axis
                x0 = np.linspace(volt_start, volt_stop, rows)
            colors = [QColor.fromHsv(int(360 * i / 16), 255, 255) for i in range(16)]
            for plot_index, array_num in enumerate(array_display_order):
                row = plot_index // 4
                col = plot_index % 4
                k_use0 = subarray_list[plot_index]
                tmp_list0 = np.arange(16 * k_use0 + sensor_offset, 16 * (k_use0 + 1) + sensor_offset, dtype=int)
                color_count = 0
                for k1 in tmp_list0:
                    if k1 >= out1.shape[0]:
                        continue
                    tmp0 = out1[k1].copy() * 1e9
                    tmp0[tmp0 > 800] = np.nan
                    if np.isnan(tmp0).all():
                        continue
                    if self.is_plot_valid(self.plot_items[row][col]):
                        self.plot_items[row][col].plot(x0, tmp0, pen=pg.mkPen(colors[color_count], width=1))
                    color_count += 1
                if self.is_plot_valid(self.plot_items[row][col]):
                    self.plot_items[row][col].setTitle(f'Array {array_num}', color='white', size='12pt bold')
                    self.plot_items[row][col].setYRange(0, 650) # nA range
                    self.plot_items[row][col].setLabel('left', 'I_SD [nA]', color='white')
                    if is_time_plot:
                        self.plot_items[row][col].setXRange(x0[0], x0[-1]) # Time range
                        self.plot_items[row][col].setLabel('bottom', 'Time [s]', color='white')
                    else:
                        self.plot_items[row][col].setXRange(volt_stop, volt_start) # Voltage range
                        self.plot_items[row][col].setLabel('bottom', 'V_g [V]', color='white')
            if filename and display_sensor_num is not None:
                base_name = os.path.basename(filename)
                sensor_str = "-sensor1" if display_sensor_num == 1 else "-sensor2"
                self.title_label_item.setText(f"{base_name}{sensor_str}", color='white', size='15pt bold')
        except Exception as e:
            self.terminal_message.emit(f"Plotting error: {str(e)}")
            if integrate.get_enable_bricklet():
                for serial_port in integrate.serial_ports:
                    print(f"Serial Port = {serial_port}")
                    if not set_mfc_zero(integrate.mfc_sfc6, serial_port):
                        print(f"<BT> Failed to set MFC <{serial_port}> to 0%")
                        self.terminal_message.emit(f"<BT> Failed to set MFC <{serial_port}> to 0%")
                        self.shared_state.set_failed_status(f"<BT> Failed to set MFC <{serial_port}> to 0% <Bricklet Failure>")
                        self.shared_state.set_measurement_failed(True)
                        # Off the Exhaust Valve

                integrate.sv1_relay = get_sv1_relay()
                if not Exhaust_Solenoid_Valve_off(integrate.sv1_relay):
                    print("<BL> Failed to set Exhaust Solenoid Valve to OFF/Closed 2f3R SW0")
                    self.terminal_message.emit("<BL> Failed to set Exhaust Solenoid Valve to OFF/Closed 2f3R SW0")
                    self.shared_state.set_failed_status("<BT> Failed to set Exhaust Solenoid Valve to OFF/Closed 2f3R SW0 <Bricklet Failure>")
                    self.shared_state.set_measurement_failed(False)
                    return False             
           
    def plot_sweep_results(self, sweep_output_file, v_min, v_max, do_double_sweep, verbose, chip_sel=1):
        calib_folder = os.path.join(self.get_app_path(), 'data', self.date_str, 'calibrationData')
        if isinstance(sweep_output_file, str):
            full_path = os.path.join(calib_folder, sweep_output_file) if not os.path.isabs(sweep_output_file) else sweep_output_file
            self.plot_calibration_sweep(full_path, chip_sel)
    def get_raw_data(self, fname0, filter_bad_ch=False, fullpath=False, verbose=False):
        if not fullpath and not os.path.isabs(fname0):
            calib_folder = os.path.join(self.get_app_path(), 'data', self.date_str, 'calibrationData')
            fname_candidate = os.path.join(calib_folder, fname0)
            if os.path.exists(fname_candidate):
                fname0 = fname_candidate
        if not os.path.exists(fname0):
            if verbose:
                self.terminal_message.emit(f"File not found: {fname0}")
            return None, 0
        file_size_kb = os.path.getsize(fname0) / 1024
        # if verbose:
        #     self.terminal_message.emit(f"Loading {os.path.basename(fname0)} ({file_size_kb:.1f} KB)")
        try:
            out0 = np.fromfile(fname0, dtype="single")
            if out0.size == 0:
                if verbose:
                    self.terminal_message.emit("File is empty")
                return None, 0
            with open(fname0, 'rb') as fid1:
                head_int1 = np.fromfile(fid1, dtype=np.uint32, count=10)
            scan_rate = head_int1[8] if len(head_int1) > 8 else 1200
            if scan_rate <= 0:
                scan_rate = 1200
            columns = 512
            rows = out0.size // columns
            if rows < 2:
                if verbose:
                    self.terminal_message.emit(f"Insufficient data rows ({rows})")
                return None, scan_rate
            out1 = out0.reshape((rows, columns))[1:].T
            channel_list = self.create_subarray_list()
            out1 = out1[channel_list]
            out1 = np.minimum(out1, 650e-9)
            # if verbose:
            #     self.terminal_message.emit(f"Successfully loaded {out1.shape} data")
            return out1, scan_rate
        except Exception as e:
            if verbose:
                self.terminal_message.emit(f"Data load error: {str(e)}")
                
            if integrate.get_enable_bricklet():
                for serial_port in integrate.serial_ports:
                    print(f"Serial Port = {serial_port}")
                    if not set_mfc_zero(integrate.mfc_sfc6, serial_port):
                        print(f"<BT> Failed to set MFC <{serial_port}> to 0%")
                        self.terminal_message.emit(f"<BT> Failed to set MFC <{serial_port}> to 0%")
                        self.shared_state.set_failed_status(f"<BT> Failed to set MFC <{serial_port}> to 0% <Bricklet Failure>")
                        self.shared_state.set_measurement_failed(True)
                        # Off the Exhaust Valve

                integrate.sv1_relay = get_sv1_relay()
                if not Exhaust_Solenoid_Valve_off(integrate.sv1_relay):
                    print("<BL> Failed to set Exhaust Solenoid Valve to OFF/Closed 2f3R SW0")
                    self.terminal_message.emit("<BL> Failed to set Exhaust Solenoid Valve to OFF/Closed 2f3R SW0")
                    self.shared_state.set_failed_status("<BT> Failed to set Exhaust Solenoid Valve to OFF/Closed 2f3R SW0 <Bricklet Failure>")
                    self.shared_state.set_measurement_failed(False)
                    return False                 
                
                
            return None, scan_rate
    def create_subarray_list(self):
        subarray_list = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15] 
        #subarray_list = [0, 8, 1, 9, 2, 10, 3, 11, 4, 12, 5, 13, 6, 14, 7, 15]
        final_ls = [
            np.hstack([np.arange(16 * k + 256 * chip_sel, 16 * (k + 1) + 256 * chip_sel, dtype=int)
                       for k in subarray_list])
            for chip_sel in [1, 0]
        ]
        return np.hstack(final_ls)
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
            calib_folder = os.path.join(self.get_app_path(), 'data', self.date_str, 'calibrationData')
            full_path = os.path.join(calib_folder, filename)
            self.plot_calibration_sweep(full_path, chip_sel)

    def cleanup(self):
        """
        Stop all running measurement threads and disconnect signals
        """
        self.terminal.clear()
        self.enable_ui()
        print("cleanup start")
        for thread in self.threads:
            thread.stop()
    
        self.threads.clear()
        print("cleanup end")
                          
    def run_measurement1(self):        
        self.terminal.clear()
        self.check_date_and_update()    
        print("Pressed Run Test button")
        
       
        if not self.shared_state.get_var_Sensor_SN1():
            self.update_status_label("SN1 is Empty!", "red")
            self.append_to_terminal("Update the Sensor Serial Number1!")
            self.enable_ui()
            return 

        if not self.shared_state.get_var_Sensor_SN2():
            self.update_status_label("SN2 is Empty!", "red")
            self.append_to_terminal("Update the Sensor Serial Number2!")
            self.enable_ui()
            return 

        
        if not self.shared_state.get_device_connection_state():
            self.update_status_label("No device connected!", "red")
            self.append_to_terminal("No device connected!")
            self.enable_ui()
            return

        if self.shared_state.get_device_connection_state():
            # Do not allow if the previous measurement is going on
            rc = self.shared_state.get_continue_measurement()
            if rc:
                self.show_alert("Alert", "Previous Measurement is not completed")
                return

            # Checking the device available space before the measurement
            path = "C:\\" if os.name == "nt" else "/"
            _, _, free = shutil.disk_usage(path)
            free_gb = free / (1024 ** 3)
            
            if free_gb < 1:
                self.show_alert("Alert", f"Low Disk Space. Not able to continue the measurement.\nAvailable space: {free_gb:.2f} GB")
                return

        print("Pressed Run Test button TP1")
        try:
            initial_vsd = float(self.initial_vsd_edit.text())
            target_current = float(self.target_current_edit.text())
            volt_start = float(self.volt_start_edit.text())                          
            volt_stop = float(self.volt_stop_edit.text())
            
            # Validations
            if not (0.005 <= initial_vsd <= 0.5):
                self.append_to_terminal("The initial vsd value should be minimum = 0.005 and maximum = 0.5 !")
                return

            if not (0 <= target_current <= 600):
                self.append_to_terminal("Warning: Target current must be between 0 and 600 nA.")
                return
            
            if not (0 <= volt_start <= 0.5):
                self.append_to_terminal("Warning: Volt start must be 0 or greater up to 0.5 V.")
                return

            if volt_start <= volt_stop:
                self.append_to_terminal("Warning: Volt start must be larger than Volt stop.")
                return            
                        
            if not (-3.5 <= volt_stop <= -1.5):
                self.append_to_terminal("Warning: Volt stop must be -1.5 V or less up to -3.5 V.")
                return
              
            delta_v = abs(volt_stop - volt_start)
            # Minimum voltage difference
            if delta_v < 0.2:
                self.append_to_terminal("Warning: Volt start and stop difference must be at least 0.2 V")
                return

            # Sweep must cross 0V 
            #if not (volt_start <= 0 <= volt_stop or volt_stop <= 0 <= volt_start):
            if not (volt_start >= 0 or volt_stop >= 0):
                self.append_to_terminal("Warning: Voltage sweep must cross 0V (range must include 0)")
                return

            # Ensure enough sweep points 
            # Remote uses ~50 mV steps internally
            estimated_points = delta_v / 0.05
            #if estimated_points < 5:
            if estimated_points <= 5:
                self.append_to_terminal("Warning: Voltage sweep range too small for stable measurement")
                return     
                        
            print("Pressed Run Test button - TP2")
                        
        except ValueError:
            self.update_status_label("Invalid input values!", "red")
            self.append_to_terminal("Warning: Invalid input values!")
            self.enable_ui()
            return
              

        self.update_status_label("Measurement is going on", "yellow")
        self.append_to_terminal("Starting measurement...")
        self.disable_ui()
        self.total_start_time = time.time()        
        for thread in self.threads:
            thread.stop()
        self.threads.clear()   
        time.sleep(1)     
        thread1 = MeasurementThread(
            self.shared_state,
            self.shared_state.device1_ip,
            "Device 1",
            initial_vsd,
            target_current,
            volt_start,
            volt_stop,
            self.date_str
        )
        thread1.terminal_message.connect(self.terminal_message)
        thread1.update_status.connect(self.update_status)
        thread1.update_plots.connect(self.update_plots)
        thread1.measurement_completed.connect(self.handle_measurement_completed)
        thread1.measurement_failed.connect(self.handle_measurement_failed)
        self.threads.append(thread1)
        thread1.start()
    @pyqtSlot(list, str)
    def handle_measurement_completed(self, transferred_files, device_id):
        self.terminal_message.emit(f"Measurement and file transfer completed for {device_id}")
        self.update_available_samples()
        self.update_status.emit("Measurement completed successfully", "green")
        total_time = time.time() - self.total_start_time
        self.terminal_message.emit(f"Total time taken: {total_time:.2f} seconds")
        self.enable_ui()
        for thread in self.threads:
            thread.stop()
        self.threads.clear()
    @pyqtSlot(str)
    def handle_measurement_failed(self, error_message):
        self.terminal_message.emit(error_message)
        self.update_status.emit("Measurement failed!", "red")
        self.enable_ui()
        for thread in self.threads:
            thread.stop()
        self.threads.clear()

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

