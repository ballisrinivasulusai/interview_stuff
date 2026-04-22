from datetime import datetime
import os
import sys
import logging
import shutil
import faulthandler
import integrate
import colorsys
import numpy as np
from typing import Optional, Tuple, List
from PyQt5.QtCore import Qt, pyqtSignal
from PyQt5.QtWidgets import QWidget, QHBoxLayout, QPushButton, QVBoxLayout, QLabel, QSizePolicy, QMessageBox
from pyqtgraph import GraphicsLayoutWidget, LabelItem, mkPen, mkColor
from TitleBar import TitleBar
from PyQt5.QtGui import QIntValidator

logging.getLogger("pyqtgraph").setLevel(logging.WARNING)
faulthandler.enable()

class AdvanceSettingsScreen(TitleBar):
    update_status_signal = pyqtSignal(str, str)
    set_sweep_signal = pyqtSignal()
    sweep_signal = pyqtSignal()
    measure_signal = pyqtSignal()
    set_measure_signal = pyqtSignal()
    display_di_signal = pyqtSignal()
    sensor_values_changed = pyqtSignal()
    bricklet_checkbox_changed = pyqtSignal(bool)
    sample_changed = pyqtSignal(str)
    logout_signal = pyqtSignal()
    toggle_sidebar_signal = pyqtSignal(bool)
    refresh_samples_signal = pyqtSignal()

    def __init__(self, main_window, username: str, shared_state):
        super().__init__(main_window, username, "Advanced Settings")
        self.shared_state = shared_state
        self.main_window = main_window
        self.current_sample: int = 0
        self.max_samples: int = 0
        self.sample_info: List[Tuple[str, int]] = []
        self._app_path: Optional[str] = None
        self.Measurebuttonclick = False
        
        self.lasttransferred_rawtext_file_path = ""

        # Initialize status label
        self.status_label = QLabel("")
        self.status_label.setStyleSheet("color: white; font-size: 14px; padding: 5px;")
        self.status_label.setAlignment(Qt.AlignCenter)
        self.status_label.setTextInteractionFlags(Qt.TextSelectableByMouse)

        # Initialize PyQtGraph
        self.graph_widget = GraphicsLayoutWidget()
        self.graph_widget.setBackground("#242746")
        self.graph_widget.setContentsMargins(0, 0, 0, 0)

        # Add widgets to layout
        main_layout = self.layout()
        main_layout.setContentsMargins(0, 0, 0, 0)
        main_layout.setSpacing(0)
        main_layout.addWidget(self.status_label, alignment=Qt.AlignTop)

        container_widget = QWidget()
        container_layout = QVBoxLayout()
        container_layout.setContentsMargins(0, 0, 0, 0)
        container_layout.setSpacing(0)
        container_layout.setAlignment(Qt.AlignTop)
        container_widget.setLayout(container_layout)

        container_layout.addWidget(self.graph_widget)
        container_widget.setMinimumHeight(900)
        container_widget.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
        main_layout.addWidget(container_widget, stretch=1)

        self.init_bottom_bar()
        self._connect_signals()
        self.reinitialize_plots()
        self.date_str = datetime.now().strftime('%d_%m_%Y')

        self.refresh_samples_signal.emit()
        self.main_window.enable_bricklet_check.setEnabled(False)

    def _connect_signals(self):
        self.main_window.set_sweep_button.clicked.connect(self.set_sweep_signal.emit)
        self.main_window.sweep_button.clicked.connect(self.sweep_signal.emit)
        self.main_window.measure_button.clicked.connect(self.measure_signal.emit)
        self.main_window.set_measure_button.clicked.connect(self.set_measure_signal.emit)
        self.main_window.display_di_i_button.clicked.connect(self.display_di_signal.emit)
        self.main_window.enable_bricklet_check.stateChanged.connect(self._emit_bricklet_checkbox_changed)
        self.logout_button.clicked.connect(self.logout_signal.emit)
        self.main_window.set_sensors_button.clicked.connect(self.sensor_values_changed.emit)


        self.set_sweep_signal.connect(self.set_sweep)
        self.sweep_signal.connect(self.sweep)
        self.measure_signal.connect(self.Measure)
        self.set_measure_signal.connect(self.set_Measure)
        #self.display_di_signal.connect(self.Display_DI_by_I)
        self.update_status_signal.connect(self._update_status_label)
        self.bricklet_checkbox_changed.connect(self.on_bricklet_checkbox_changed)
        self.sensor_values_changed.connect(self.setSensorValues)
        self.sample_changed.connect(self.change_sample)
        self.logout_signal.connect(self.main_window.logout)
        self.toggle_sidebar_signal.connect(self.main_window.toggle_sidebar)
        self.refresh_samples_signal.connect(self.refresh_samples)

    def _emit_bricklet_checkbox_changed(self, state):
        self.bricklet_checkbox_changed.emit(state == Qt.Checked)

    def on_bricklet_checkbox_changed(self, checked: bool):
        self.shared_state.set_EnableBricklets_Checkbox(checked)
        print(f"Checkbox {'checked' if checked else 'unchecked'}")

    def _update_status_label(self, text: str, color: str):
        self.status_label.setText(text)
        self.status_label.setStyleSheet(f"color: {color}; font-size: 14px; padding: 5px;")

    def setSensorValues(self):   
        if self.shared_state.get_device_connection_state():
            try:
                print("Updating sensor values in file...")
                #Get values from EditTexts
                sensor1_val = int(self.main_window.sensor_sn1.text())
                sensor2_val = int(self.main_window.sensor_sn2.text())
                
                self.shared_state.set_var_Sensor_SN1(sensor1_val)
                self.shared_state.set_var_Sensor_SN2(sensor2_val)
                self.update_status_signal.emit("Sensor values updated successfully.", "white")
        
            except ValueError:
                print("❌ Invalid input. Please enter integer values for sensors.")
                #self.main_window.status_label.setText("Invalid sensor values.")
            except Exception as e:
                print(f"❌ Error updating sensors: {e}")
                #self.main_window.status_label.setText("Error while updating sensor values.")
        else:
            self.update_status_signal.emit("No Device is Connected", "red")

    def set_sweep(self):
        if self.shared_state.get_device_connection_state():
            try:
                vg_min_value = float(self.main_window.vg_min.text())
                vg_max_value = float(self.main_window.vg_max.text())
                vsd_value = float(self.main_window.vsd.text())
                t_dwell = float(self.main_window.t_dwell.text())
                d_step = float(self.main_window.d_step.text())
                g_step = float(self.main_window.g_step.text())

                self.shared_state.var_vg_min = vg_min_value
                self.shared_state.var_vg_max = vg_max_value
                self.shared_state.var_vsd = vsd_value
                self.shared_state.set_var_t_dwell(t_dwell)
                self.shared_state.set_var_d_step(d_step)
                self.shared_state.set_var_g_step(g_step)

                self.update_status_signal.emit("Sweep Values Updated", "white")
            except ValueError:
                self.update_status_signal.emit("Invalid sweep parameters", "red")
        else:
            self.update_status_signal.emit("No Device is Connected", "red")

    def sweep(self):
        if self.shared_state.get_device_connection_state():

            if not self.main_window.screens["Settings"].sn1_edit.text().strip():
                self.show_alert("Input Error", "SN1 should not be empty")
                return 

            if not self.main_window.screens["Settings"].sn2_edit.text().strip():
                self.show_alert("Input Error", "SN2 should not be empty")
                return 

            if not self.main_window.screens["Settings"].sample1_edit.text().strip():
                self.show_alert("Input Error", "Sample ID 1 should not be empty")
                return 

            if not self.main_window.screens["Settings"].sample2_edit.text().strip():
                self.show_alert("Input Error", "Sample ID 2 should not be empty")
                return 
                
            self.shared_state.set_var_Sensor_SN1(self.main_window.screens["Settings"].sn1_edit.text().strip())
            self.shared_state.set_var_Sensor_SN2(self.main_window.screens["Settings"].sn2_edit.text().strip())
            self.shared_state.set_var_Sample_ID1(self.main_window.screens["Settings"].sample1_edit.text().strip())
            self.shared_state.set_var_Sample_ID2(self.main_window.screens["Settings"].sample2_edit.text().strip())  
          
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



            self.disable_ui()
            self.Measurebuttonclick = False
            self.update_status_signal.emit("Sweep Measurement is going on", "yellow")
            integrate.set_Sweep_Measurement(True)

            if self.shared_state.get_EnableBricklets_Checkbox():
                self.shared_state.updateTvariables_Bricklets()
                #integrate.start_bricklet_event.set()
                if self.shared_state.get_device_connection_state():
                    print("Starting Sweep Measurement with bricklet...")
                    integrate.startmeasurement1_event.set()
            else:
                #if integrate.single_device_connected:
                if self.shared_state.get_device_connection_state():
                    print("Starting Sweep Measurement...")
                    integrate.startmeasurement1_event.set()

            
        else:
            self.update_status_signal.emit("No Device is Connected", "red")

    def set_Measure(self):
        if self.shared_state.get_device_connection_state():
            try:
                time_edit = self.main_window.time
                time_edit.setValidator(QIntValidator(1, 9999))
                print("set_Measure")

                # --- Avoid reconnecting multiple times ---
                try:
                    time_edit.editingFinished.disconnect()
                except TypeError:
                    pass

                # --- Validation function ---
                def validate_time():
                    text = time_edit.text()
                    print("validate_time", text)
                    if text == "" or int(text) < 1:
                        time_edit.setText("1")

                # --- Connect once ---
                time_edit.editingFinished.connect(validate_time)

                # --- Immediately validate once (optional) ---
                validate_time()

                # --- Get validated values ---
                var_vg_measure = float(self.main_window.vg_measure.text())
                var_vsd_measure = float(self.main_window.vsd_measure.text())
                var_time = int(time_edit.text())

                self.shared_state.var_vg_measure = var_vg_measure
                self.shared_state.var_vsd_measure = var_vsd_measure
                self.shared_state.var_time = var_time

                self.update_status_signal.emit("Measurement Values Updated", "white")
                
            except ValueError:
                self.update_status_signal.emit("Invalid measurement parameters", "red")
        else:
                self.update_status_signal.emit("No Device is Connected", "red")                    


    def Measure(self):
        if self.shared_state.get_device_connection_state():
            try:        
                if not self.main_window.screens["Settings"].sn1_edit.text().strip():
                    self.show_alert("Input Error", "SN1 should not be empty")
                    return 

                if not self.main_window.screens["Settings"].sn2_edit.text().strip():
                    self.show_alert("Input Error", "SN2 should not be empty")
                    return 

                if not self.main_window.screens["Settings"].sample1_edit.text().strip():
                    self.show_alert("Input Error", "Sample ID 1 should not be empty")
                    return 

                if not self.main_window.screens["Settings"].sample2_edit.text().strip():
                    self.show_alert("Input Error", "Sample ID 2 should not be empty")
                    return 

                self.shared_state.set_var_Sensor_SN1(self.main_window.screens["Settings"].sn1_edit.text().strip())
                self.shared_state.set_var_Sensor_SN2(self.main_window.screens["Settings"].sn2_edit.text().strip())
                self.shared_state.set_var_Sample_ID1(self.main_window.screens["Settings"].sample1_edit.text().strip())
                self.shared_state.set_var_Sample_ID2(self.main_window.screens["Settings"].sample2_edit.text().strip())  

                
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
                
                self.disable_ui()        
                var_vg_measure = float(self.main_window.vg_measure.text())
                var_vsd_measure = float(self.main_window.vsd_measure.text())
                var_time = float(self.main_window.time.text())

                self.shared_state.var_vg_measure = var_vg_measure
                self.shared_state.var_vsd_measure = var_vsd_measure
                self.shared_state.var_time = var_time

                self.Measurebuttonclick = True
                self.update_status_signal.emit("Measurement is going on", "yellow")
                integrate.set_Normal_Measurement(True)

                if self.shared_state.get_EnableBricklets_Checkbox():
                    self.shared_state.updateTvariables_Bricklets()
                    #integrate.start_bricklet_event.set()
                    if self.shared_state.get_device_connection_state():
                        print("Starting Advance Measurement with bricklet...")
                        #integrate.start_bricklet_event.set()
                        integrate.startmeasurement1_event.set()
                else:
                    #if integrate.single_device_connected:
                    if self.shared_state.get_device_connection_state():
                        integrate.startmeasurement1_event.set()

                
            except ValueError:
                self.update_status_signal.emit("Invalid measurement parameters", "red")
        else:
            self.update_status_signal.emit("No Device is Connected", "red")

    def _complete_measurement(self):
        self.update_status_signal.emit("Measurement completed", "green")
        self.enable_ui()
        self.update_available_samples()
        filename, _ = self.get_current_sample_info() if self.sample_info else (None, None)
        # if filename:
        #     self._show_filename_and_update(filename)

    def _show_filename_and_update(self, filename: str):
        self.update_status_signal.emit(filename, "white")
        self.refresh_samples()

    def reinitialize_plots(self):
        self.graph_widget.clear()
        self.title_label_item = LabelItem(justify='center')
        self.title_label_item.setText("", color='white', size='15pt')
        self.graph_widget.addItem(self.title_label_item, row=0, colspan=4)

        self.plot_items = []
        for i in range(4):
            row = []
            for j in range(4):
                p = self.graph_widget.addPlot(row=i+1, col=j)
                p.showGrid(x=True, y=True, alpha=0.1)
                view_box = p.getViewBox()
                view_box.setMouseEnabled(x=False, y=False)
                view_box.setBackgroundColor('#ffffff')
                p.getAxis('left').setPen(mkPen('black'))
                p.getAxis('bottom').setPen(mkPen('black'))
                p.setMinimumSize(150, 150)
                row.append(p)
            self.plot_items.append(row)

    def process_plot_data(self, filename: str, volt_start: float, volt_stop: float, chip_sel0: int, 
                         title: str, measure_button_click: bool, shared_state, app_path):
        try:
            self.date_str = datetime.now().strftime('%d_%m_%Y')
            data_path = os.path.join(app_path, 'data', self.date_str, filename) 
            if not os.path.exists(data_path):
                self._show_error(f"File not found: {filename}")
                return

            if measure_button_click and not filename.startswith(("SM_")):
                # Process normal measurement data
                data = self.get_raw_data_sweep(data_path)
                if data is None:
                    self._show_error("Error loading data")
                    return
                min_volt = min(volt_start, volt_stop)
                max_volt = max(volt_start, volt_stop)
                x0 = np.linspace(volt_start, volt_stop, data.shape[1])
                subarray_list = [0, 2, 4, 6, 8, 10, 12, 14, 1, 3, 5, 7, 9, 11, 13, 15]
                array_display_order = [1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16]
                num_chip_sel0 = (2 - chip_sel0) % 2
                plot_data = []

                for plot_index, array_num in enumerate(array_display_order):
                    k_use0 = subarray_list[plot_index]
                    tmp_list0 = np.arange(16 * k_use0 + 256 * num_chip_sel0, 16 * (k_use0 + 1) + 256 * num_chip_sel0, dtype=int)
                    plot_lines = []
                    count_t = 0
                    for k1 in tmp_list0:
                        if k1 >= data.shape[0]:
                            continue
                        tmp0 = data[k1].copy() * 1e9 + count_t
                        tmp0[tmp0 > 800] = np.nan
                        if not np.isnan(tmp0).all():
                            plot_lines.append((x0, tmp0))
                        count_t += 0  # Adjust if transistor_y_displacement is needed
                    plot_data.append((array_num, plot_lines))

                self.update_plots(plot_data, title, min_volt, max_volt, chip_sel0, filename, measure_button_click)
            else:
                # Process sweep measurement data
                all_values = shared_state.cube.ReadCubeFile(data_path)
                data = np.array(all_values[2])
                if data.size == 0:
                    self._show_error("No data available")
                    return

                samples = data.shape[1]
                min_volt = min(volt_start, volt_stop)
                max_volt = max(volt_start, volt_stop)

                if samples % 2 == 0:
                    x0 = np.linspace(min_volt, max_volt, int(samples / 2))
                    x_axis = np.concatenate([x0, x0[::-1]])
                else:
                    x0 = np.linspace(min_volt, max_volt, int((samples - 1) / 2))
                    x_axis = np.concatenate([x0, x0[::-1]])
                    x_axis = np.append(x_axis, x_axis[-1])

                data1 = 1.e9 * data
                subarray_list = [0, 2, 4, 6, 8, 10, 12, 14, 1, 3, 5, 7, 9, 11, 13, 15]
                array_display_order = [1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16]
                num_chip_sel0 = (2 - chip_sel0) % 2
                plot_data = []

                for plot_index, array_num in enumerate(array_display_order):
                    k_use0 = subarray_list[plot_index]
                    tmp_list0 = np.arange(16 * k_use0 + 256 * num_chip_sel0, 16 * (k_use0 + 1) + 256 * num_chip_sel0, dtype=int)
                    plot_lines = []
                    for k1 in tmp_list0:
                        if k1 >= data1.shape[0]:
                            continue
                        tmp0 = data1[k1].copy()
                        tmp0[tmp0 > 800] = np.nan
                        if not np.isnan(tmp0).all():
                            plot_lines.append((x_axis, tmp0))
                    plot_data.append((array_num, plot_lines))

                self.update_plots(plot_data, title, min_volt, max_volt, chip_sel0, filename, measure_button_click)

        except Exception as e:
            self._show_error(f"Error processing plot data: {str(e)}")

    def get_raw_data_sweep(self, fname: str) -> Optional[np.ndarray]:
        if not os.path.exists(fname):
            return None

        try:
            with open(fname, 'rb') as f:
                np.fromfile(f, dtype=np.uint32, count=10)
                np.fromfile(f, dtype=np.float32, count=8)
                np.fromfile(f, dtype=np.uint32, count=12)
                np.fromfile(f, dtype=np.uint16, count=32)
                f.read(20)

            data = np.fromfile(fname, dtype="single")
            if data.size == 0:
                return None

            columns = 2 * 256
            rows = data.size // columns
            if rows <= 1:
                return None

            return data.reshape((rows, columns))[1:].T
        except Exception as e:
            print(f"Error loading file {fname}: {str(e)}")
            return None

    def plot_graphs(self):
        for row in self.plot_items:
            for plot in row:
                plot.clear()

        if not self.max_samples:
            self._show_error("No samples available")
            return

        filename, sensor_num = self.get_current_sample_info()
        print("Filename", filename)
        print("sensor_num", sensor_num)
        if not filename or not sensor_num:
            self._show_error("Invalid sample selection")
            return

        filename_parts = filename.split("_")
        try:
            start_vg = float(filename_parts[1])
            stop_vg = float(filename_parts[2])
        except (IndexError, ValueError):
            self._show_error(f"Invalid filename format: {filename}")
            return

        title = f"{filename} - Sensor {sensor_num}"
        # Process data directly
        self.process_plot_data(filename, start_vg, stop_vg, sensor_num, 
                            title, self.Measurebuttonclick, 
                            self.shared_state, self.get_app_path())

    def update_plots(self, plot_data: list, title: str, min_volt: float, max_volt: float, 
                    chip_sel0: int, filename: str, measure_button_click: bool):
        # Clear existing plots
        for row in self.plot_items:
            for plot in row:
                plot.clear()

        # Generate 16 distinct HSV-based colors and convert them to RGB
        colors = []
        for i in range(16):
            h = i / 16.0  # Hue value between 0 and 1
            r, g, b = colorsys.hsv_to_rgb(h, 1.0, 1.0)
            rgb_scaled = (int(r * 255), int(g * 255), int(b * 255))
            colors.append(mkColor(rgb_scaled))

        # Plot each array's data
        for plot_index, (array_num, plot_lines) in enumerate(plot_data):
            row = plot_index // 4
            col = plot_index % 4
            plot = self.plot_items[row][col]
            color_count = 0

            for x_axis, y_data in plot_lines:
                # Skip empty or invalid data
                if (x_axis is None or y_data is None or 
                    len(x_axis) == 0 or len(y_data) == 0):
                    continue

                color = colors[color_count % len(colors)]
                pen = mkPen(color, width=1, alpha=1.0)
                plot.plot(x_axis, y_data, pen=pen)
                color_count += 1

            # Customize plot appearance
            plot.setTitle(f'Array {array_num}', color='white', size='12pt')
            plot.setYRange(-50 if measure_button_click else 0, 800)
            plot.setXRange(min_volt, max_volt)
            plot.setLabel('left', 'I_SD [nA]', color='white', **{'font-size': '12pt'})
            if measure_button_click:
                plot.setLabel('bottom', f'Time [{self.shared_state.var_time}]', 
                            color='white', **{'font-size': '12pt'})
            else:
                plot.setLabel('bottom', 'Vg [V]', color='white', **{'font-size': '12pt'})

        # Set the main plot title
        self.title_label_item.setText(title, color='white', size='15pt')

    def get_app_path(self) -> str:
        if self._app_path is None:
            if getattr(sys, 'frozen', False):
                app_dir = os.path.dirname(sys.executable)
                self._app_path = app_dir[:app_dir.rfind("dist")].rstrip(os.sep) if "dist" in app_dir else app_dir
            else:
                self._app_path = os.path.dirname(os.path.abspath(__file__))
        return self._app_path

    def extract_timestamp(self, filename: str) -> str:
        try:
            parts = filename.split('_')
            if len(parts) >= 3:
                return parts[-2] + parts[-1].split('.')[0]
        except (AttributeError, IndexError, ValueError) as e:
            print("Error",e)
            pass
        return "0"

    def update_available_samples(self):
        
        base_path = os.path.join(self.get_app_path(), 'data')
        print("base_path:", base_path)

        # -------------------------------------------------
        # 1. Decide full_path
        # -------------------------------------------------
        if self.lasttransferred_rawtext_file_path == "":
            full_path = base_path
        else:
            full_path = self.lasttransferred_rawtext_file_path

        print("rawtext_file_path:", full_path)

        # -------------------------------------------------
        # 2. Extract correct DATE FOLDER (dd_mm_yyyy)
        # -------------------------------------------------

        # Case 1 → rawtext path is a file inside date folder
        if os.path.isfile(full_path):
            parent_folder = os.path.dirname(full_path)          # .../data/19_11_2025
            date_folder = os.path.basename(parent_folder)       # 19_11_2025

        # Case 2 → full_path is already a date directory
        elif os.path.isdir(full_path):
            # Check if this folder is inside /data
            rel = os.path.relpath(full_path, base_path)
            if rel != ".":
                date_folder = rel
            else:
                # default → today
                date_folder = datetime.now().strftime('%d_%m_%Y')

        # Case 3 → fallback
        else:
            date_folder = datetime.now().strftime('%d_%m_%Y')

        self.date_str = date_folder
        print("Extracted date:", self.date_str)

        # -------------------------------------------------
        # 3. Build full date folder path safely
        # -------------------------------------------------
        self.today_date_folder_path = os.path.join(base_path, self.date_str)
        print("Today date folder:", self.today_date_folder_path)

        # -------------------------------------------------
        # 4. Load sample files
        # -------------------------------------------------
        self.sample_info.clear()

        if os.path.isdir(self.today_date_folder_path):
            files = [
                f for f in os.listdir(self.today_date_folder_path)
                if f.startswith(('SM_', 'AM_')) and 'rawtext' in f
            ]
            files.sort(key=self.extract_timestamp, reverse=True)

            
            for filename in files:
                int(filename[7]) if filename[7].isdigit() else 1
                self.sample_info.extend([(filename, i) for i in (1, 2)])

        self.max_samples = len(self.sample_info)
        self.current_sample = 0 if self.max_samples > 0 else 0
        self.update_sample_display()

    def get_current_sample_info(self) -> Tuple[Optional[str], Optional[int]]:
        return self.sample_info[self.current_sample] if self.sample_info and self.current_sample < self.max_samples else (None, None)

    def _show_error(self, message: str):
        try:
            self.update_status_signal.emit(message, "red")
        except Exception as e:
            print(f"Error displaying error message: {e}")

    def change_sample(self, direction: str):
        if not self.max_samples:
            return

        self.current_sample = (
            max(0, self.current_sample - 1) if direction == "prev" else
            min(self.max_samples - 1, self.current_sample + 1))
        self.update_sample_display()
        self.plot_graphs()
        self.update_button_states()

    def update_sample_display(self):
        self.set_sample_label()

    def set_sample_label(self):
        self.sample_label.setText(
            "NO SAMPLES" if not self.max_samples else
            f"SAMPLE {self.current_sample + 1}/{self.sample_count()}"
        )

    def update_button_states(self):
        self.left_button.setEnabled(self.current_sample > 0)
        self.right_button_states()

    def sample_count(self):
        return self.max_samples

    def right_button_states(self):
        self.right_button.setEnabled(self.current_sample < self.max_samples - 1)

    def init_bottom_bar(self):
        bottom_bar = QWidget()
        bottom_bar.setStyleSheet("""
            QWidget { background-color: #242746; padding: 10px; border-top: 1px solid #444; }
            QPushButton { 
                color: white; background-color: #000000; 
                border: 1px solid #555; border-radius: 4px; 
                padding: 5px 15px; min-width: 80px; 
            }
            QPushButton:hover { background-color: #4a4d6c; }
            QPushButton:pressed { background-color: #2a2d4c; }
            QPushButton:disabled { color: #888; background-color: #333; }
        """)

        layout = QHBoxLayout()
        layout.setContentsMargins(0, 0, 0, 0)
        layout.setSpacing(10)

        self.left_button = QPushButton("◀")
        self.left_button.setFixedSize(40, 40)
        self.left_button.clicked.connect(lambda: self.sample_changed.emit("prev"))

        self.sample_label = QPushButton()
        self.sample_label.setEnabled(False)
        self.sample_label.setFixedSize(200, 40)

        self.right_button = QPushButton("▶")
        self.right_button.setFixedSize(40, 40)
        self.right_button.clicked.connect(lambda: self.sample_changed.emit("next"))

        layout.addStretch()
        layout.addWidget(self.left_button)
        layout.addWidget(self.sample_label)
        layout.addWidget(self.right_button)
        layout.addStretch()

        bottom_bar.setLayout(layout)
        self.layout().addWidget(bottom_bar)

    def refresh_samples(self):
        self.update_available_samples()
        self.update_sample_display()
        self.update_button_states()
        self.plot_graphs()

    def disable_ui(self):
        self.logout_button.setEnabled(False)
        self.toggle_sidebar_signal.emit(False)
        self.main_window.set_sweep_button.setEnabled(False)
        self.main_window.sweep_button.setEnabled(False)
        self.main_window.measure_button.setEnabled(False)
        self.main_window.set_measure_button.setEnabled(False)
        self.main_window.display_di_i_button.setEnabled(False)
        self.main_window.enable_bricklet_check.setEnabled(False)
        self.main_window.presweep_check.setEnabled(False)
        self.right_button.setEnabled(False)
        self.left_button.setEnabled(False)

    def enable_ui(self):
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
        self.right_button.setEnabled(True)
        self.left_button.setEnabled(True)


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




