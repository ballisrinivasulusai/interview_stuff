from datetime import datetime
import os
import sys
import time
import colorsys
import re
import numpy as np
from typing import Optional, Tuple
from PyQt5.QtCore import Qt, pyqtSignal
from PyQt5.QtGui import QColor
from TitleBar import TitleBar
import pyqtgraph as pg
from pyqtgraph import GraphicsLayoutWidget, LabelItem
from PyQt5.QtWidgets import (
    QSizePolicy,
    QVBoxLayout,
    QWidget
)

import faulthandler
faulthandler.enable()

class SampleScreen(TitleBar):
    update_status_signal = pyqtSignal(str, str)
    update_UI_signal = pyqtSignal()
    update_sample_title_signal = pyqtSignal(str)
    update_arrow_buttons_signal = pyqtSignal(bool, bool)
    update_plot_title_signal = pyqtSignal(str, str)  # Updated to include color
    update_error_message_signal = pyqtSignal(str)
    update_sample_count_signal = pyqtSignal(int, int)  # New signal for sample count
    # New signal for plotting data in the main thread
    plot_data_signal = pyqtSignal(np.ndarray, float, float, int, int, str, int)

    def get_app_path(self):
        if getattr(sys, 'frozen', False):
            app_dir = os.path.dirname(sys.executable)            
            if "dist" in app_dir:
                app_dir = app_dir[:app_dir.rfind("dist")].rstrip(os.sep)
            return app_dir
        else:
            return os.path.dirname(os.path.abspath(__file__))

    def __init__(self, main_window, username, shared_state):
        self.sampletitle = "SAMPLE 1"
        super().__init__(main_window, username, self.sampletitle)
        self.shared_state = shared_state
        self.current_sample = 1
        self.graph_generated = False
        self.main_window = main_window
        self.sorted_files = []
        self.max_samples = 0
        self.TotalDevices = 0
        self.sample_info = []
        self.PlotCalibgraph = False
        self.display_di_flag = False  # Flag to toggle between dI/I and normal plots
        self.lasttransferred_rawtext_file_path = ""       

        # Use existing layout from TitleBar
        layout = self.layout()
        if layout is None:
            layout = QVBoxLayout(self)
            self.setLayout(layout)

        # Remove scroll area and use direct layout
        container_widget = QWidget()
        container_layout = QVBoxLayout()
        container_layout.setAlignment(Qt.AlignTop)
        container_widget.setLayout(container_layout)

        self.graph_widget = GraphicsLayoutWidget()
        self.graph_widget.setBackground("#242746")

        container_layout.addWidget(self.graph_widget)
        container_widget.setMinimumHeight(900)
        container_widget.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
        layout.insertWidget(1, container_widget, stretch=1)
        self.date_str = datetime.now().strftime('%d_%m_%Y')

        self.reinitialize_plots()

        # Connect signals to slots
        self.update_UI_signal.connect(self.update_controls)
        self.update_sample_title_signal.connect(self.set_sample_title)
        self.update_arrow_buttons_signal.connect(self.set_arrow_buttons)
        self.update_plot_title_signal.connect(self.set_plot_title)
        self.update_error_message_signal.connect(self.show_error)
        self.update_status_signal.connect(self.set_status_label)  # Connect status signal
        self.plot_data_signal.connect(self.plot_data_slot)
        # Connect the DISPLAY dI/I button click
        self.main_window.display_di_i_button.clicked.connect(self.handle_display_di_i)

        self._is_plotting = False
        self.refresh_data(1)

    def set_status_label(self, text: str, color: str):
        """Slot to update the status label text and color."""
        self.status_label_item.setText(text, color=color, size='15pt')

    def handle_display_di_i(self):
        """Handle the DISPLAY dI/I button click to toggle between dI/I and normal plots."""
        try:
            sample_name = self.main_window.sample_name_input.text().strip()
            y_axis_text = self.main_window.y_axis_input.text().strip()
            remove_initial_time_text = self.main_window.remove_initial_time_input.text().strip()

            # Validate sample name
            if not sample_name:
                self.update_plot_title_signal.emit("Sample name cannot be empty", "red")
                self.update_status_signal.emit("", "white")
                return

            # Validate and parse y-axis input (expected format: comma-separated numbers, e.g., "95,102")
            if not y_axis_text:
                self.update_plot_title_signal.emit("Y-axis values cannot be empty", "red")
                self.update_status_signal.emit("", "white")  # Clear status
                return

            try:
                y_axis_values = [float(x) for x in y_axis_text.split(',')]
                if len(y_axis_values) != 2:
                    self.update_plot_title_signal.emit("Y-axis must contain exactly two values (e.g., 95,102)", "red")
                    self.update_status_signal.emit("", "white")  # Clear status
                    return
                if not all(isinstance(v, (int, float)) for v in y_axis_values):
                    self.update_status_signal.emit("", "white")  # Clear status
                    self.update_plot_title_signal.emit("Y-axis values must be numbers", "red")
                    return
            except ValueError:
                self.update_plot_title_signal.emit("Invalid Y-axis format. Use comma-separated numbers (e.g., 95,102)", "red")
                self.update_status_signal.emit("", "white")  # Clear status
                return

            # Validate and parse remove initial time input
            try:
                remove_initial_time = float(remove_initial_time_text) if remove_initial_time_text else 0
                if remove_initial_time < 0:
                    self.update_plot_title_signal.emit("Remove initial time must be non-negative", "red")
                    self.update_status_signal.emit("", "white")  # Clear status
                    return
            except ValueError:
                self.update_plot_title_signal.emit("Invalid remove initial time format. Must be a number", "red")
                self.update_status_signal.emit("", "white")  # Clear status
                return

            # Toggle the flag
            self.display_di_flag = not self.display_di_flag

            if self.display_di_flag:
                full_path = os.path.join(self.today_date_folder_path, sample_name)
                if not os.path.exists(full_path):
                    self.update_plot_title_signal.emit("File not found", "red")
                    self.update_status_signal.emit("", "white")  # Clear status
                    self.display_di_flag = False  # Revert
                    return

                # Save original state
                self.original_sorted_files = self.sorted_files[:]
                self.original_sample_info = self.sample_info[:]
                self.original_max_samples = self.max_samples
                self.original_current_sample = self.current_sample

                # Set to the entered file only
                self.sorted_files = [sample_name]
                self.sample_info = [(sample_name, 1), (sample_name, 2)]
                self.max_samples = 2

                # Set current_sample based on previous if same file
                prev_filename, prev_sensor = self.original_sample_info[self.original_current_sample - 1]
                self.current_sample = prev_sensor if prev_filename == sample_name else 1

                # Update sample title and count
                self.update_sample_title_signal.emit(f"SAMPLE {self.current_sample}")
                self.update_sample_count_signal.emit(self.current_sample, self.max_samples)
                self.update_status_signal.emit("Display dI/I Graph", "white")
                # Disable side panel widgets except name, y-axis, and display button
                self.disable_side_panel()
            else:
                # Restore original state
                self.sorted_files = self.original_sorted_files
                self.sample_info = self.original_sample_info
                self.max_samples = self.original_max_samples
                self.current_sample = self.original_current_sample

                # Update sample title and count
                self.update_sample_title_signal.emit(f"SAMPLE {self.current_sample}")
                self.update_sample_count_signal.emit(self.current_sample, self.max_samples)

                self.update_status_signal.emit("Sample Graph", "white")
                self.enable_side_panel()

            self.main_window.display_di_i_button.setText("Plot normal" if self.display_di_flag else "DISPLAY dI/I")

            # Update shared state with input values
            self.shared_state.var_name = sample_name
            self.shared_state.var_y_axis = y_axis_text
            self.shared_state.var_remove_initial_time = remove_initial_time

            # Plot the appropriate graph
            self.plot_graphs()

        except Exception as e:
            print(f"Error in handle_display_di_i: {e}")
            self.update_plot_title_signal.emit("Error processing dI/I data", "red")
            self.update_status_signal.emit("", "white")  # Clear status

    def disable_side_panel(self):
        """Disable all side panel widgets except sample_name_input, y_axis_input, remove_initial_time_input, and display_di_i_button."""
        for widget in [
            self.main_window.sensor_sn1, self.main_window.sensor_sn2,
            self.main_window.set_sensors_button, self.main_window.enable_bricklet_check, self.main_window.presweep_check,
            self.main_window.vg_min, self.main_window.t_dwell, self.main_window.vg_max, self.main_window.d_step,
            self.main_window.vsd, self.main_window.g_step, self.main_window.set_sweep_button, self.main_window.sweep_button,
            self.main_window.vg_measure, self.main_window.vsd_measure, self.main_window.time,
            self.main_window.measure_button, self.main_window.set_measure_button
        ]:
            widget.setEnabled(False)
        #self.main_window.left_arrow.setEnabled(False)
        #self.main_window.right_arrow.setEnabled(False)
        for btn in self.main_window.buttons:
            btn.setEnabled(False)

    def enable_side_panel(self):
        """Enable all side panel widgets."""
        for widget in [
            self.main_window.sensor_sn1, self.main_window.sensor_sn2, 
            self.main_window.set_sensors_button, self.main_window.enable_bricklet_check, self.main_window.presweep_check,
            self.main_window.vg_min, self.main_window.t_dwell, self.main_window.vg_max, self.main_window.d_step,
            self.main_window.vsd, self.main_window.g_step, self.main_window.set_sweep_button, self.main_window.sweep_button,
            self.main_window.vg_measure, self.main_window.vsd_measure, self.main_window.time,
            self.main_window.measure_button, self.main_window.set_measure_button
        ]:
            widget.setEnabled(True)
        #self.main_window.left_arrow.setEnabled(True)
        #self.main_window.right_arrow.setEnabled(True)
        for btn in self.main_window.buttons:
            btn.setEnabled(True)

    def reinitialize_plots(self):
        self.graph_widget.clear()
        # Add status label at the top
        self.status_label_item = LabelItem(justify='center')
        self.status_label_item.setText("", color='w', size='15pt')
        self.graph_widget.addItem(self.status_label_item, row=0, col=0, colspan=4)
        # Add title label below status label
        self.title_label_item = LabelItem(justify='center')
        self.title_label_item.setText("", color='w', size='15pt')
        self.graph_widget.addItem(self.title_label_item, row=1, col=0, colspan=4)

        self.plot_items = []
        for i in range(4):
            row = []
            for j in range(4):
                p = self.graph_widget.addPlot(row=i+2, col=j)
                p.showGrid(x=True, y=True, alpha=0.1)
                view_box = p.getViewBox()
                view_box.setMouseEnabled(x=False, y=False)
                view_box.setBackgroundColor('#ffffff')
                p.getAxis('left').setPen(pg.mkPen('black'))
                p.getAxis('bottom').setPen(pg.mkPen('black'))

                p.setMinimumSize(150, 150)
                row.append(p)
            self.plot_items.append(row)

    def set_sample_title(self, title: str):
        if self.max_samples > 0:
            self.sampletitle = title
            self.main_window.sample_label.setText(f"Sample {self.current_sample}/{self.max_samples}")
            self.title_label.setText(title)
        else:
            self.sampletitle = "NO SAMPLES"
            self.main_window.sample_label.setText("NO SAMPLES")
            self.title_label.setText("NO SAMPLES") 
        

    def set_arrow_buttons(self):
        self.update_arrow_buttons_signal.emit(
            self.current_sample > 1,
            self.current_sample < self.max_samples
        )

    def set_plot_title(self, title: str, color: str = 'w'):
        self.title_label_item.setText(title, color=color, size='15pt')

    def show_error(self, message: str):
        if hasattr(self, 'plot_items') and self.plot_items and self.is_plot_valid(self.plot_items[0][0]):
            self.plot_items[0][0].setTitle(message, color='w', size='16pt')
            self.plot_items[0][0].showAxis('left', False)
            self.plot_items[0][0].showAxis('bottom', False)

    def update_controls(self):
        self.update_sample_title_signal.emit(f'SAMPLE {self.current_sample}')
        self.update_sample_count_signal.emit(self.current_sample, self.max_samples)

    def refresh_data(self, flag):
        self.sorted_files = self.get_sorted_files()
        self.max_samples = self.determine_max_samples()
        self.update_sample_count_signal.emit(self.current_sample, self.max_samples)
        if flag == 1:
            self.plot_graphs()

    def get_sorted_files(self):
        # base_path = os.path.join(self.get_app_path(), 'data')
        # self.today_date_folder_path = os.path.join(base_path, self.date_str)
        base_path = os.path.join(self.get_app_path(), 'data')
        print("base_path:", base_path)
        if self.lasttransferred_rawtext_file_path == "":
            full_path = base_path
        else:
            full_path = self.lasttransferred_rawtext_file_path

        print("rawtext_file_path:", full_path)

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

        self.today_date_folder_path = os.path.join(base_path, self.date_str)       
        print("today_date_folder_path", self.today_date_folder_path)
        if not os.path.exists(self.today_date_folder_path):
            return []

        raw_files = []
        for f in os.listdir(self.today_date_folder_path):
            if f.startswith(('CM_', 'MM_')) and '_' in f:
                try:
                    parts = f.split('_')
                    if len(parts) >= 5 and parts[3].replace('rawtext', '').isdigit():
                        file_path = os.path.join(self.today_date_folder_path, f)
                        mod_time = os.path.getmtime(file_path)
                        raw_files.append((f, mod_time))
                except Exception as e:
                    print("Error", e)
                    continue

        raw_files.sort(key=lambda x: x[1], reverse=True)
        return [f[0] for f in raw_files]

    def determine_max_samples(self):
        if not self.sorted_files:
            self.update_sample_title_signal.emit("NO SAMPLES")
            #self.update_arrow_buttons_signal.emit(False, False)
            self.sample_info = []
            return 0

        self.max_samples = len(self.sorted_files) * 2
        self.sample_info = []
        for file_idx, filename in enumerate(self.sorted_files):
            self.sample_info.append((filename, 1))
            self.sample_info.append((filename, 2))

        if self.current_sample > self.max_samples:
            self.current_sample = max(1, self.max_samples)

        return self.max_samples

    def get_current_sample_info(self) -> Tuple[Optional[str], Optional[int]]:
        if self.sample_info and 0 < self.current_sample <= len(self.sample_info):
            return self.sample_info[self.current_sample - 1]
        return (None, None)

    def plot_graphs(self):
        if self._is_plotting:
            print("Skipping recursive plot_graphs call")
            return

        self._is_plotting = True
        try:
            if not self.sorted_files:
                self.update_sample_title_signal.emit("NO SAMPLES")
                #self.update_arrow_buttons_signal.emit(False, False)
                self.update_sample_count_signal.emit(self.current_sample, self.max_samples)
                return

            base_path = os.path.join(self.get_app_path(), 'data')
            self.today_date_folder_path = os.path.join(base_path, self.date_str)
            filename, sensor_num = self.get_current_sample_info()

            if filename is None or sensor_num is None:
                self.update_error_message_signal.emit("Invalid sample selection")
                self.update_sample_count_signal.emit(self.current_sample, self.max_samples)
                return

            # Update the sample name input field with the current filename only if not in dI/I mode
            if not self.display_di_flag:
                self.main_window.sample_name_input.setText(filename)
            # Update sample title and count
            self.update_sample_title_signal.emit(f"SAMPLE {self.current_sample}")
            self.update_sample_count_signal.emit(self.current_sample, self.max_samples)

            rawtext_file_path = os.path.join(self.today_date_folder_path, filename)
            if not os.path.exists(rawtext_file_path):
                self.refresh_data(1)
                return

            if self.display_di_flag:
                # Plot dI/I graph
                y_axis_str = self.main_window.y_axis_input.text()
                remove_initial_time = float(self.main_window.remove_initial_time_input.text()) if self.main_window.remove_initial_time_input.text() else 0
                if y_axis_str:
                    try:
                        y_lim_norm = tuple(map(float, y_axis_str.split(',')))
                        if len(y_lim_norm) != 2:
                            raise ValueError("Y-axis limits must be two comma-separated numbers")
                    except ValueError as e:
                        self.update_plot_title_signal.emit(f"Invalid y-axis limits: {str(e)}", "red")
                        self.update_status_signal.emit("", "white")  # Clear status
                        return
                else:
                    y_lim_norm = (95, 102)  # Default y-axis limits

                out_m, scan_rate, file_name = self.get_raw_data(rawtext_file_path, remove_upper=True, filter=False, fullpath=False, verbose=False, current_cutoff=650, remove_initial_time=remove_initial_time)
                if out_m is None or out_m.size == 0 or out_m.shape[1] == 0:
                    self.update_plot_title_signal.emit("No valid data available after loading", "red")
                    self.update_status_signal.emit("", "white")  # Clear status
                    return

                normalized_out_wave = self.remove_background_wave(out_m, scan_rate, fname=filename, window_size=10, bg_sec=2, verbose=False)
                if normalized_out_wave is None or normalized_out_wave.size == 0 or normalized_out_wave.shape[1] == 0:
                    self.update_plot_title_signal.emit("No valid data available after processing", "red")
                    self.update_status_signal.emit("", "white")  # Clear status
                    return

                self.plot_by_array_pyqt(normalized_out_wave, x_axis_type="measurement", normalized=True, fig_name=filename, chip_sel0=sensor_num, save_figure=False, verbose=False, plot_avg=False, y_lim_norm=y_lim_norm, scan_rate=scan_rate)
                self.update_plot_title_signal.emit(f"{filename} - Sensor {sensor_num}", "white")
            else:
                # Plot normal graph
                plot_sensor_num = 1 if sensor_num == 2 else 2
                rawfilename = os.path.basename(rawtext_file_path)
                out1 = self.get_raw_data_sweep(rawtext_file_path)

                print(f"Plotting sample {self.current_sample}, filename={filename}, original sensor_num={sensor_num}, plotting sensor_num={plot_sensor_num}")
                print(f"out1 shape: {out1.shape if out1 is not None else 'None'}")

                if out1 is None or out1.size == 0:
                    self.update_sample_title_signal.emit("INVALID DATA")
                    self.update_sample_count_signal.emit(self.current_sample, self.max_samples)
                    self.update_status_signal.emit("", "white")  # Clear status
                    return

                start_Vg = 0
                if self.PlotCalibgraph:
                    stop_Vg = self.shared_state.calib_time
                else:
                    self.Ttotal = int(self.shared_state.get_T1() + self.shared_state.get_T2() + self.shared_state.get_T3())
                    stop_Vg = self.Ttotal

                if not all(self.is_plot_valid(plot) for row in self.plot_items for plot in row):
                    print("Invalid PlotItem detected, reinitializing plots")
                    self.reinitialize_plots()

                # Emit signal to plot data in the main thread
                self.plot_data_signal.emit(
                    out1,
                    start_Vg,
                    stop_Vg,
                    0,
                    plot_sensor_num,
                    rawfilename,
                    sensor_num
                )
                self.update_UI_signal.emit()

        except Exception as e:
            print(f"Error in plot_graphs: {e}")
            self.update_sample_title_signal.emit("PLOT ERROR")
            self.update_sample_count_signal.emit(self.current_sample, self.max_samples)
            self.update_status_signal.emit("", "white")  # Clear status
        finally:
            self._is_plotting = False

    def plot_data_slot(self, out1, volt_start, volt_stop, transistor_y_displacement, chip_sel0, filename, display_sensor_num):
        """Slot to handle plotting in the main thread."""
        try:
            self.plot_Vg_sweep_by_array(
                out1,
                volt_start,
                volt_stop,
                transistor_y_displacement,
                chip_sel0,
                filename,
                display_sensor_num
            )
        except Exception as e:
            print(f"Error in plot_data_slot: {e}")
            self.update_sample_title_signal.emit("PLOT ERROR")
            self.update_sample_count_signal.emit(self.current_sample, self.max_samples)
            self.update_status_signal.emit("", "white")  # Clear status

    def change_sample(self, direction):
        if direction == "prev":
            self.current_sample = max(1, self.current_sample - 1)
        elif direction == "next":
            self.current_sample = min(self.max_samples, self.current_sample + 1)
        else:
            return

        self.update_sample_count_signal.emit(self.current_sample, self.max_samples)
        self.plot_graphs()

    def is_plot_valid(self, plot):
        try:
            plot.getViewBox()
            return True
        except RuntimeError:
            return False

    def func_calc_outlier(self, arr_in):
        arr_in = arr_in[:, 1:-1]
        line_mean = arr_in.mean(axis=0).reshape((1, arr_in.shape[1]))
        arr_diff = np.abs(arr_in - line_mean)
        tmp0 = arr_diff / np.abs(line_mean)
        tmp0 = tmp0.sum(axis=1) / tmp0.size
        if np.abs(line_mean).max() < 2:
            tmp1 = (tmp0 == 0)
        else:
            tmp1 = tmp0 > 0.3
        if tmp1.sum() == tmp1.size:
            tmp1[0:] = False
        return tmp1

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

    def get_raw_data(self, fname0, remove_upper=True, filter=False, fullpath=False, verbose=False, current_cutoff=650, remove_initial_time=0):
        print("\n\n===> ENTER: get_raw_data()")
        num_sensors = 2
        if fname0 != '':
            t0 = time.time()
            if fullpath:
                fname0 = fname0[0]
            print(f"[INFO] Reading binary file: {fname0}")
            out0 = np.fromfile(fname0, dtype="single")
            out_len = len(out0)
            file_name = fname0.split('/')[-1]
            print(f"[INFO] Raw file name: {file_name}")
            print(f"[INFO] Raw data length: {out_len}")
            if verbose:
                print("  -> output shape:", out0.shape)
                print("  -> first 10 items:", out0[:10])
            print("\n[STEP] Reading header...")
            with open(fname0, 'rb') as fid1:
                head_int1 = np.fromfile(fid1, dtype=np.uint32, count=10)
                head_float = np.fromfile(fid1, dtype=np.float32, count=8)
                head_int2 = np.fromfile(fid1, dtype=np.uint32, count=12)
                head_hex = np.fromfile(fid1, dtype=np.uint16, count=32)
                head_barcode = fid1.read(20).strip(b'\x00').decode('utf-8')
            if verbose:
                print("head_int1:", head_int1)
                print("head_float:", head_float)
                print("head_int2:", head_int2)
                print("head_hex:", head_hex)
                print("head_barcode:", head_barcode)
            Ch1_serial = head_int1[2]
            Ch2_serial = head_int1[5]
            scan_rate = head_int1[8]
            print(f"[INFO] Chip1 Serial: {Ch1_serial}, Chip2 Serial: {Ch2_serial}, Scan rate: {scan_rate}")
            print("\n[STEP] Reshaping data...")
            columns = num_sensors * 256
            rows = int(out0.size / columns)
            print(f"  -> Expected rows: {rows}, columns: {columns}")
            out1 = out0.reshape((rows, columns))[1:].T
            print(f"  -> Reshaped out1 shape: {out1.shape}")
            if verbose:
                print("out1[0] PRE sort:", out1[0])
            print("[STEP] Applying channel sorting...")
            channel_list = self.create_subarray_list()
            out1 = out1[channel_list]
            print(f"  -> out1 shape POST sort: {out1.shape}")
            if verbose:
                print("out1[0] POST sort:", out1[0])
            if remove_initial_time > 0:
                print("\n[STEP] Removing initial time")
                print(f"  -> remove_initial_time: {remove_initial_time} sec")
                print(f"  -> scan_rate: {scan_rate}")
                out1 = out1[:, int(remove_initial_time * scan_rate):]
                print(f"  -> Shape after removal: {out1.shape}")
            if remove_upper:
                print("\n[STEP] Applying cutoff filter")
                print(f"  -> cutoff: {current_cutoff} nA")
                out1 = np.minimum(out1, current_cutoff * 1e-9)
                print(f"  -> Shape after cutoff filter: {out1.shape}")
            if verbose:
                print("\n[SUMMARY]")
                print(f"Chip1 Serial: {Ch1_serial}")
                print(f"Chip2 Serial: {Ch2_serial}")
                print(f"Barcode: {head_barcode}")
                print(f"Scan rate: {scan_rate}")
            if filter:
                print("\n[STEP] Filtering outliers...")
                filtered_out = self.filter_outliers(out1)
                print(f"  -> Shape after filtering: {filtered_out.shape}")
                out1 = filtered_out
            elapsed = time.time() - t0
            print(f"\n===> EXIT: get_raw_data() | Time taken: {elapsed:.3f} sec")
        return out1, scan_rate, file_name

    def create_subarray_list(self, verbose=False):
        subarray_list = [0, 8, 1, 9, 2, 10, 3, 11, 4, 12, 5, 13, 6, 14, 7, 15]
        final_ls = [
            np.hstack([np.arange(16 * k + 256 * chip_sel, 16 * (k + 1) + 256 * chip_sel, dtype=int)
                       for k in subarray_list])
            for chip_sel in [1, 0]
        ]
        return np.hstack(final_ls)

    def filter_outliers(self, out, verbose=False):
        row_min = np.min(out, axis=1)
        row_max = np.max(out, axis=1)
        row_mean = np.mean(out, axis=1)
        valid_rows = (row_min >= 5e-9) & (row_max <= 5e-6) & (row_mean >= 20e-9)
        filtered_out = np.where(valid_rows[:, None], out, np.nan)
        if verbose: 
            print('out shape', out.shape)
        if verbose: 
            print('filtered_out shape', filtered_out.shape)
        return filtered_out

    def safe_nanstd(self, a, axis=None, ddof=0):
        """Compute nanstd but return 0 instead of warning when dof <= 0"""
        # Count non-nan values
        count = np.sum(~np.isnan(a), axis=axis)
        # If less than 2 valid points → std = 0 (no variation possible)
        if np.any(count <= ddof):
            result_shape = list(a.shape)
            if axis is not None:
                result_shape.pop(axis)
            return np.zeros(result_shape, dtype=a.dtype)
        
        return np.nanstd(a, axis=axis, ddof=ddof)

    def remove_background_wave(self, out1, scan_rate, fname="", window_size=10, bg_sec=2, verbose=False):
        out1 = out1 * 1e9
        smoothed_out1 = self.smooth_data(out1, window_size)
        row_means = np.mean(smoothed_out1, axis=1)
        valid_rows = row_means >= 20
        smoothed_out1[~valid_rows, :] = np.nan
        valid_rows_not_high = row_means < 650
        smoothed_out1[~valid_rows_not_high, :] = np.nan
        subarray_avg = np.nanmean(smoothed_out1.reshape(-1, 16, smoothed_out1.shape[1]), axis=1)
        subarray_std = np.nanstd(smoothed_out1.reshape(-1, 16, smoothed_out1.shape[1]), axis=1)
        expanded_avg = np.repeat(subarray_avg, 16, axis=0)
        expanded_std = np.repeat(subarray_std, 16, axis=0)
        deviation_mask = np.abs(smoothed_out1 - expanded_avg) > (4 * expanded_std)
        smoothed_out1[deviation_mask] = np.nan
        end_bg_start_point = int(scan_rate * bg_sec)
        start_bg = np.mean(smoothed_out1[:, :scan_rate * bg_sec], axis=1, keepdims=True)
        end_bg = np.mean(smoothed_out1[:, -end_bg_start_point:], axis=1, keepdims=True)
        start_bg_flat = start_bg.flatten()
        end_bg_flat = end_bg.flatten()
        bg_line = np.array([np.linspace(start, end, smoothed_out1.shape[1]) for start, end in zip(start_bg_flat, end_bg_flat)])
        bg_array = bg_line
        bg_array[bg_array == 0] = 1e-10
        normalized_out1 = smoothed_out1 / bg_array
        grouped = normalized_out1.reshape(-1, 16, normalized_out1.shape[1])
        group_stds = np.nanstd(grouped, axis=2)
        median_group_std = np.nanmedian(group_stds, axis=1)
        noisy_rows = np.zeros(normalized_out1.shape[0], dtype=bool)
        for group_idx, med_std in enumerate(median_group_std):
            group_start = group_idx * 16
            group_end = group_start + 16
            noisy_rows[group_start:group_end] = group_stds[group_idx] > (2.5 * med_std)
        normalized_out1[noisy_rows, :] = np.nan
        return normalized_out1

    def smooth_data(self, data, window_size=10):
        kernel = np.ones(window_size) / window_size
        smoothed = np.array([np.convolve(row, kernel, mode='same') for row in data])
        edge_size = window_size // 2
        for row in range(data.shape[0]):
            smoothed[row, :edge_size] = smoothed[row, edge_size]
            smoothed[row, -edge_size:] = smoothed[row, -edge_size - 1]
        return smoothed

    def plot_by_array_pyqt(self, out1, x_axis_type, xstart=-2, xend=0, normalized=False, transistor_y_displacement=0, fig_name="", chip_sel0=1, save_figure=False, verbose=False, plot_avg=False, y_lim_norm=(95, 102), y_lim=(-50, 700), scan_rate=1200):
        time_stamps = out1.shape[1]
        rows = out1.shape[0]
        if verbose: 
            print('out1 shape', out1.shape, 'rows', rows, 'time_stamps', time_stamps)
        num_chip_sel0 = (chip_sel0 - 1) % 2
        if verbose: 
            print('out1 shape', out1.shape)
        min_volt = min(xstart, xend)
        max_volt = max(xstart, xend)
        match = re.search(r'_(-?\d+\.?\d*)_(-?\d+\.?\d*)_', fig_name)
        if match:
            x_start = float(match.group(1))
            x_end = float(match.group(2))
            min_volt = min(x_start, x_end)
            max_volt = max(x_start, x_end)
        else:
            min_volt = 0
            max_volt = out1.shape[1] / scan_rate
        if x_axis_type == 'measurement':
            if normalized:
                x_axis = np.arange(out1.shape[1], dtype=float) / scan_rate
            else:
                x_axis = np.linspace(xstart, xend, out1.shape[1])
        else:
            x_axis = np.linspace(xstart, xend, out1.shape[1])
        colors = []
        for i in range(16):
            h = i / 16.0
            r, g, b = colorsys.hsv_to_rgb(h, 1.0, 1.0)
            rgb_scaled = (int(r * 255), int(g * 255), int(b * 255))
            colors.append(rgb_scaled)
        array_avg = np.nanmean(out1.reshape(-1, 16, out1.shape[1]), axis=1)
        for row in self.plot_items:
            for plot in row:
                plot.clear()
        for k0 in range(16):
            row = k0 // 4
            col = k0 % 4
            count_t = 0
            color_count = 0
            tmp_list0 = np.arange(16 * k0 + 256 * num_chip_sel0, 16 * (k0 + 1) + 256 * num_chip_sel0, dtype=int)
            if verbose: 
                print('tmp_list0 Ch', num_chip_sel0, "  : ", tmp_list0)
            plot = self.plot_items[row][col]
            for k1 in tmp_list0:
                tmp0 = out1[k1] + count_t
                tmp_avg = array_avg[k0]
                if not normalized:
                    tmp0 = tmp0 * 1e9 + count_t
                    tmp_avg = tmp_avg * 1e9
                if normalized:
                    tmp0 = tmp0 * 100
                    tmp_avg = tmp_avg * 100
                color = colors[color_count]
                alpha = int(255 * 0.2)
                pen = pg.mkPen(color=(color[0], color[1], color[2], alpha), width=1)
                plot.plot(x_axis, tmp0, pen=pen)
                if plot_avg:
                    alpha_avg = int(255 * 0.4)
                    pen_avg = pg.mkPen(color=(0, 0, 0, alpha_avg), width=1)
                    plot.plot(x_axis, tmp_avg, pen=pen_avg)
                count_t += transistor_y_displacement
                color_count += 1
            plot.setTitle(f'Array {k0 + 1}', color='white', size='12pt')
            plot.showGrid(x=True, y=True, alpha=0.1)
            if normalized:
                plot.setYRange(y_lim_norm[0], y_lim_norm[1])
                plot.setLabel('left', 'ΔI_{a.u.} [%]', color='white', **{'font-size': '12pt'})
            else:
                plot.setYRange(y_lim[0], y_lim[1])
                plot.setLabel('left', 'I_SD [nA]', color='white', **{'font-size': '12pt'})
            if x_axis_type == 'measurement':
                plot.setXRange(min_volt, max_volt)
                plot.setLabel('bottom', 'Time [s]', color='white', **{'font-size': '12pt'})
            else:
                plot.setXRange(min_volt, max_volt)
                plot.setLabel('bottom', 'Vg [V]', color='white', **{'font-size': '12pt'})
        return f"{fig_name}_Sensor{chip_sel0}_measurement.png" if x_axis_type == 'measurement' else f"{fig_name}_Sensor{chip_sel0}_sweep.png"

    def plot_Vg_sweep_by_array(self, out1, volt_start, volt_stop, transistor_y_displacement=0, chip_sel0=1, filename=None, display_sensor_num=None):
        subarray_list = [0, 2, 4, 6, 8, 10, 12, 14, 1, 3, 5, 7, 9, 11, 13, 15]
        array_display_order = [1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16]

        num_chip_sel0 = (chip_sel0 - 1) % 2
        sensor_offset = 256 * num_chip_sel0
        print(f"Plotting sensor {chip_sel0}, num_chip_sel0={num_chip_sel0}, offset={sensor_offset}")

        filename_str = str(filename)
        filename_parts = filename_str.split("_")
        Measurementtype = filename_parts[0]

        for row in self.plot_items:
            for plot in row:
                plot.clear()

        if Measurementtype in ['CM', 'AM']:
            try:
                #xStart = float(filename_parts[1])
                xEnd = float(filename_parts[2])
            except (ValueError, IndexError) as e:
                print(f"Error converting xStart or xEnd to float: {e}")
                return

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
                        print(f"Skipping invalid index {k1} for out1 shape {out1.shape}")
                        continue
                    try:
                        tmp0 = out1[k1].copy() * 1e9 + count_t
                        tmp0[tmp0 > 800] = np.nan
                        if np.isnan(tmp0).all():
                            continue
                        if self.is_plot_valid(self.plot_items[row][col]):
                            self.plot_items[row][col].plot(x0, tmp0, pen=pg.mkPen('b', width=1))
                        count_t += transistor_y_displacement
                    except Exception as e:
                        print(f"Error plotting array {array_num}, device {k1}: {e}")
                        continue

                try:
                    if self.is_plot_valid(self.plot_items[row][col]):
                        self.plot_items[row][col].setTitle(f'Array {array_num}', color='white', size='12pt')
                        self.plot_items[row][col].setYRange(0, 750)
                        self.plot_items[row][col].setXRange(0, max(xEnd, 0))
                        self.plot_items[row][col].setLabel('left', 'I_SD [nA]', color='white', **{'font-size': '12pt'})
                        self.plot_items[row][col].setLabel('bottom', 'Time [s]', color='white', **{'font-size': '12pt'})
                except Exception as e:
                    print(f"Error setting axes properties for array {array_num}: {e}")
                    continue
        else:
            try:
                volt_start = float(filename_parts[1])
                volt_stop = float(filename_parts[2])
            except (ValueError, IndexError) as e:
                print(f"Error converting volt_start or volt_stop to float: {e}")
                return

            x0 = np.linspace(volt_start, volt_stop, out1.shape[1])

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
                        print(f"Skipping invalid index {k1} for out1 shape {out1.shape}")
                        continue
                    try:
                        tmp0 = out1[k1].copy() * 1e9 + count_t
                        tmp0[tmp0 > 800] = np.nan
                        if np.isnan(tmp0).all():
                            continue
                        if self.is_plot_valid(self.plot_items[row][col]):
                            self.plot_items[row][col].plot(x0, tmp0, pen=pg.mkPen(colors[color_count], width=1))
                        count_t += transistor_y_displacement
                        color_count += 1
                    except Exception as e:
                        print(f"Error plotting array {array_num}, device {k1}: {e}")
                        continue

                try:
                    if self.is_plot_valid(self.plot_items[row][col]):
                        self.plot_items[row][col].setTitle(f'Array {array_num}', color='w', size='12pt')
                        self.plot_items[row][col].setYRange(0, 800)
                        self.plot_items[row][col].setXRange(0, max(volt_stop, 0))
                        self.plot_items[row][col].setLabel('left', 'I_SD [nA]', color='white', **{'font-size': '12pt'})
                        self.plot_items[row][col].setLabel('bottom', f'Time [{self.Ttotal}]', color='white', **{'font-size': '12pt'})
                except Exception as e:
                    print(f"Error setting axes properties for array {array_num}: {e}")
                    continue

        try:
            if filename:
                self.update_plot_title_signal.emit(f"{filename} - Sensor {display_sensor_num}", "white")
        except Exception as e:
            print(f"Error setting window title: {e}")

    def generate_graph(self):
        self.current_sample = 1
        self.refresh_data(1)




