import os
import sys
import sqlite3
import time
from TitleBar import TitleBar
from PyQt5.QtWidgets import (
    QWidget, QVBoxLayout, QHBoxLayout, QGridLayout,QLabel, QLineEdit, 
    QPushButton, QTableWidget, QTableWidgetItem, QHeaderView, QMessageBox,
    QStackedWidget, QStyledItemDelegate, QAbstractItemView, QCheckBox,QApplication
)
from PyQt5.QtCore import QObject,Qt, pyqtSignal,pyqtSlot,QThread
from PyQt5.QtGui import QIntValidator, QDoubleValidator,QPixmap
import integrate
from TinkerForge.Vial_Base import vial_base_raise,vial_base_lower,vial_base_is_home_position,VIAL_UPPER_LIMIT,VIAL_LOWER_LIMIT,VIAL_1_3RD_POSITION
from TinkerForge.SolenoidValve_Qrelay import sv1_backflush_on,sv1_backflush_off
import faulthandler
faulthandler.enable()           

class VialWorker(QObject):
    error = pyqtSignal(str)
    operation_completed = pyqtSignal()
    operation_error = pyqtSignal()
    status_update = pyqtSignal(str)

    @pyqtSlot(str, int)
    def run(self, command, value=0):
        try:
            if command == "vial_base_UP":
                result = vial_base_raise(value)
                if not result:
                    print("Failed to move vial base up")
                    self.status_update.emit("Status : Failed to move vial base up")
                    self.operation_error.emit()
                    return  

            elif command == "vial_base_1_3RD_Down":
                result = vial_base_raise(value)  
                if not result:
                    print("Failed to move vial base to 1/3rd position")
                    self.status_update.emit("Status : Failed to move vial base to 1/3rd position")
                    self.operation_error.emit()
                    return


            elif command == "vial_base_FULL_Down":
                result = vial_base_lower(value)
                if not result:
                    print("Failed to move vial base down")
                    self.status_update.emit("Status : Failed to move vial base down")
                    self.operation_error.emit()
                    return

            elif command == "BackFlush_operations":    
                print("BackFlush_operations started in VialWorker")
                
                # 1. vials move down to -1000000 steps
                print( "Raising vial base to 1/3rd position for backflush")
                self.status_update.emit("Status : Reaching 1/3rd Position")
                result = vial_base_raise(VIAL_1_3RD_POSITION)
                if not result:
                    print("Failed to move vial base to 1/3rd position")
                    self.status_update.emit("Status : Failed to move vial base to 1/3rd position")
                    self.operation_error.emit()
                    return

                # 2. back flush solenoid valves open
                print("Turn ON Backflush")
                self.status_update.emit("Status : Activating BackFlush")                
                ret = sv1_backflush_on(integrate.get_sv1_relay())               
                if not ret:
                    print("Failed to enable backflash on")
                    self.status_update.emit("Status : Failed to enable backflash on")
                    self.operation_error.emit()
                    return
                
                # 3. back flush N2 time runs
                print( "back flush N2 time running")
                self.status_update.emit(f"Status : waiting for {integrate.get_N2_Period()} seconds")              
                time.sleep(integrate.get_N2_Period())                   

                # 4. back flush solenoids close
                print( "deactivating backflush")
                self.status_update.emit("Status : Deactivating BackFlush")     
                ret = sv1_backflush_off(integrate.get_sv1_relay())
                if not ret:
                    print("Failed to enable backflash off")
                    self.status_update.emit("Status : Failed to enable backflash off")
                    self.operation_error.emit()
                    return

                # 5. Raise vial base to upper limit for backflush
                print( "vials move back up to -1260000.")
                self.status_update.emit("Status : vials move back to upper limit")
                result = vial_base_raise(VIAL_UPPER_LIMIT)
                if not result:
                    print("Failed to move vial base to upper limit")
                    self.status_update.emit("Status : Failed to move vial base to upper limit")
                    self.operation_error.emit()
                    return

            command =""  # Clear command after execution
            self.operation_completed.emit()
        except Exception as e:
            self.error.emit(str(e))                             
            
class CenterAlignDelegate(QStyledItemDelegate):
    def initStyleOption(self, option, index):
        super().initStyleOption(option, index)
        option.displayAlignment = Qt.AlignCenter

class IntDelegate(QStyledItemDelegate):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.validator = QIntValidator(bottom=0, top=999999)

    def createEditor(self, parent, option, index):
        editor = QLineEdit(parent)
        editor.setValidator(self.validator)
        return editor

class FloatDelegate(QStyledItemDelegate):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.validator = QDoubleValidator()
        self.validator.setNotation(QDoubleValidator.StandardNotation)
        self.validator.setDecimals(2)

    def createEditor(self, parent, option, index):
        editor = QLineEdit(parent)
        editor.setValidator(self.validator)
        return editor

class SettingsScreen(TitleBar): 
    Reset_checkbox_signal = pyqtSignal(bool)
    vial_command_signal = pyqtSignal(str, int)
    toggle_sidebar_signal = pyqtSignal(bool)
    def __init__(self, main_window, username, shared_state):
        super().__init__(main_window, username, "Settings")
        self.shared_state = shared_state
        self.argondata = 0
        self.VOCdata = 0
        self.completed_devices = set()
        self.failed_devices = set()
        self.base_path = os.path.join(self.get_app_path1(), 'data')
        self.db_path = os.path.join(self.get_app_path1(), 'res', 'measurementtable.db')
        self.current_test_code = None
        self.current_pulse_value = None
        self.setFixedHeight(920)
        # Create res folder if it doesn't exist
        res_path = os.path.join(self.get_app_path1(), 'res')
        if not os.path.exists(res_path):
            os.makedirs(res_path)
        
        # Initialize database
        self.initialize_database()
        
        # Create a stacked widget to switch between measurement and dialog
        self.stacked_widget = QStackedWidget()
        self.layout().addWidget(self.stacked_widget)
        
        # Create container widget for measurement screen
        self.measurement_container = QWidget()
        self.measurement_layout = QVBoxLayout(self.measurement_container)
        
        self.load_data_from_db()
        self.init_measurement_ui()
        
        # Add measurement widget to stacked widget
        self.stacked_widget.addWidget(self.measurement_container)
        
        self.table = None
        
        # Vial GUI thread
        print("Setting Screen VialWorker created")
        self.vial_thread = QThread()
        self.vial_worker = VialWorker()
        self.vial_worker.moveToThread(self.vial_thread)   
        self.vial_command_signal.connect(self.vial_worker.run)
        self.vial_worker.operation_completed.connect(self.on_vial_operation_completed)
        self.vial_worker.operation_error.connect(self.on_vial_operation_error)
        self.vial_worker.error.connect(self.on_vial_error)
        self.vial_worker.status_update.connect(self.on_vial_status_update)
        self.vial_thread.start()          

    def on_vial_status_update(self, status_text):
        """Update the backflush status label from the worker thread"""
        self.backflush_status_label.setText(status_text)

    def on_vial_operation_completed(self):
        """Re-enable UI when vial operation completes"""
        self.UI_Visiblity(True)
            
        self.backflush_status_label.setText("Status : Completed")    
        print("Vial operation completed")
    
    def on_vial_operation_error(self):
        """Re-enable UI when vial operation completes"""
        self.UI_Visiblity(True)
        print("Vial operation Error : Reenabling UI")

    def on_vial_error(self, error_message):
        """Handle vial operation errors"""
        self.UI_Visiblity(True)
        print(f"Vial operation error: {error_message}")
        self.backflush_status_label.setText("Status : BackFlush Failed")  

    def connect_signals(self):
        """Connect signals from MeasurementScreen after initialization."""
        try:
            self.Reset_checkbox_signal.connect(self.reset_checkboxes)           
            integrate.update_steps.Update_steps_count_signal.connect(self.update_steps_count_label)
            self.toggle_sidebar_signal.connect(self.main_window.toggle_sidebar)
            measurement_screen = self.main_window.screens["Measure"]
            if measurement_screen:
                measurement_screen.sample_temperature_update_signal.connect(self.update_sample_temperature)
                measurement_screen.coil_temperature_update_signal.connect(self.update_coil_temperature)
                measurement_screen.aircube_temperature_update_signal.connect(self.update_aircube_temperature)
                measurement_screen.aircube_humidity_update_signal.connect(self.update_aircube_humidity)
                measurement_screen.aircube_pressure_update_signal.connect(self.update_aircube_pressure)
        except KeyError as e:
            print(f"Error: MeasurementScreen not found in screens dictionary: {e}")
        except AttributeError as e:
            print(f"Error: Unable to access screens dictionary: {e}")

    def update_steps_count_label(self, steps: int):
        if hasattr(self, "stepscount_label"):
            self.stepscount_label.setText(f"Steps Count : {steps}")


    def update_sample_temperature(self, sample_temp):
        """Slot to update the sample heat label with the latest temperature."""
        if hasattr(self, 'temp_table'):
            item = QTableWidgetItem(f"{sample_temp:.1f}")
            item.setTextAlignment(Qt.AlignCenter)
            item.setFlags(item.flags() & ~Qt.ItemIsEditable)
            self.temp_table.setItem(0, 0, item)

    def update_coil_temperature(self, coil_temp):
        """Slot to update the coil heat label with the latest temperature."""
        if hasattr(self, 'temp_table'):
            item = QTableWidgetItem(f"{coil_temp:.1f}")
            item.setTextAlignment(Qt.AlignCenter)
            item.setFlags(item.flags() & ~Qt.ItemIsEditable)
            self.temp_table.setItem(0, 1, item)

    def update_aircube_temperature(self, aircube_temp):
        """Slot to update the aircube heat label with the latest temperature."""
        print(f"Updating aircube_heat_label with temperature: {aircube_temp}")
        if hasattr(self, 'aircube_table'):
            item = QTableWidgetItem(f"{aircube_temp:.3f}")
            item.setTextAlignment(Qt.AlignCenter)
            item.setFlags(item.flags() & ~Qt.ItemIsEditable)
            self.aircube_table.setItem(0, 0, item)

    def update_aircube_humidity(self, aircube_humidity):
        """Slot to update the aircube humidity label with the latest humidity."""
        if hasattr(self, 'aircube_table'):
            item = QTableWidgetItem(f"{aircube_humidity:.3f}")
            item.setTextAlignment(Qt.AlignCenter)
            item.setFlags(item.flags() & ~Qt.ItemIsEditable)
            self.aircube_table.setItem(0, 1, item)

    def update_aircube_pressure(self, aircube_pressure):
        """Slot to update the aircube pressure label with the latest pressure."""
        if hasattr(self, 'aircube_table'):
            item = QTableWidgetItem(f"{aircube_pressure:.3f}")
            item.setTextAlignment(Qt.AlignCenter)
            item.setFlags(item.flags() & ~Qt.ItemIsEditable)
            self.aircube_table.setItem(0, 2, item)

    # App Path    
    def get_app_path1(self):
        if getattr(sys, 'frozen', False): # For .app/.dmg file app path
            app_dir = os.path.dirname(sys.executable)            
            if "dist" in app_dir:
                app_dir = app_dir[:app_dir.rfind("dist")].rstrip(os.sep)            
            data_path = os.path.join(app_dir)
            return data_path
        else: # Normal Run
            app_dir = os.path.dirname(os.path.abspath(__file__))
            return app_dir    
        
    # DataBase Initialization with DB File creation if not available.
    def initialize_database(self):
        if os.path.exists(self.db_path):
            return
        try:
            with sqlite3.connect(self.db_path) as conn:
                cursor = conn.cursor()
                cursor.execute('''
                    CREATE TABLE IF NOT EXISTS argon_data (
                        "Nitrogen Pulse Code" TEXT,
                        "Nitrogen % of Full Scale" REAL,
                        "T0 (sec)" REAL,
                        "T3 (sec)" REAL
                    )
                ''')
                argon_data = [
                    ('A1', 0, 0.0, 30.0), ('A2', 10, 0.0, 30.0), ('A3', 20, 0.0, 30.0),
                    ('A4', 30, 0.0, 30.0), ('A5', 40, 0.0, 30.0), ('A6', 50, 0.0, 30.0),
                    ('A7', 60, 0.0, 30.0), ('A8', 70, 0.0, 30.0), ('A9', 80, 0.0, 30.0),
                    ('A10', 90, 0.0, 30.0), ('A11', 10, 0.0, 30.0)
                ]
                cursor.executemany('INSERT INTO argon_data VALUES (?, ?, ?, ?)', argon_data)
                
                cursor.execute('''
                    CREATE TABLE IF NOT EXISTS voc_data (
                        "VOC Pulse Code" TEXT,
                        "Mixed Constants (Nitrogen + VOC)" REAL,
                        "T1 (sec)" REAL,
                        "T2 (sec)" REAL
                    )
                ''')
                voc_data = [
                    ('V1', 0, 10.0, 20.0), ('V2', 10, 10.0, 20.0), ('V3', 20, 10.0, 20.0),
                    ('V4', 30, 10.0, 20.0), ('V5', 40, 10.0, 20.0), ('V6', 50, 10.0, 20.0),
                    ('V7', 60, 10.0, 20.0), ('V8', 70, 10.0, 20.0), ('V9', 80, 10.0, 20.0),
                    ('V10', 90, 10.0, 20.0), ('V11', 10, 10.0, 20.0)
                ]
                cursor.executemany('INSERT INTO voc_data VALUES (?, ?, ?, ?)', voc_data)
                
                cursor.execute('''
                    CREATE TABLE IF NOT EXISTS test_code_data (
                        "Test Code" TEXT,
                        "Pulses" TEXT,
                        "SH(°C)" INTEGER,
                        "CH(°C)" INTEGER,
                        "S1V(°C)" INTEGER,
                        "S2V(°C)" INTEGER,
                        "N2H(°C)" INTEGER
                    )
                ''')
                test_code_data = [
                    ('T1', 'A1+V1', 40, 40, 40, 40, 40), ('T2', 'A2+V2',40, 40, 40, 40, 40), ('T3', 'A3+V3',40, 40, 40, 40, 40),
                    ('T4', 'A4+V4', 40, 40, 40, 40, 40), ('T5', 'A5+V5',40, 40, 40, 40, 40), ('T6', 'A6+V6',40, 40, 40, 40, 40),
                    ('T7', 'A7+V7', 40, 40, 40, 40, 40), ('T8', 'A8+V8',40, 40, 40, 40, 40), ('T9', 'A9+V9',40, 40, 40, 40, 40),
                    ('T10', 'A10+V10',40, 40, 40, 40, 40), ('T11', 'A11+V11',40, 40, 40, 40, 40)
                ]
                cursor.executemany('INSERT INTO test_code_data VALUES (?, ?, ?, ?, ?, ?, ?)', test_code_data)
                conn.commit()
        except sqlite3.Error as e:
            print(f"Database initialization error: {e}")
      
    # Fetching the Data from res/Measurement.db file.        
    def load_data_from_db(self):
        try:
            with sqlite3.connect(self.db_path) as conn:
                cursor = conn.cursor()
                cursor.execute("SELECT * FROM test_code_data")
                self.test_code_data = cursor.fetchall()
                cursor.execute("SELECT * FROM voc_data")
                self.voc_data = cursor.fetchall()
                cursor.execute("SELECT * FROM argon_data")
                self.argon_data = cursor.fetchall()
        except sqlite3.Error as e:
            print(f"Database error: {e}")
            self.test_code_data = []
            self.voc_data = []
            self.argon_data = []    
         
    # Save Table Values to the DB File.        
    def save_data_to_db(self):
        print("save_data_to_db called")
        try:
            with sqlite3.connect(self.db_path) as conn:
                cursor = conn.cursor()

                # --- Update test_code_data ---
                for row in range(self.test_code_table.rowCount()):
                    test_code_item = self.test_code_table.item(row, 0)
                    pulses_item = self.test_code_table.item(row, 1)
                    sample_heat_item = self.test_code_table.item(row, 2)
                    coil_heat_item = self.test_code_table.item(row, 3)
                    S1V_heat_item = self.test_code_table.item(row, 4)
                    S2V_heat_item = self.test_code_table.item(row, 5)
                    N2H_heat_item = self.test_code_table.item(row, 6)
                    if not all([test_code_item, pulses_item, sample_heat_item, coil_heat_item, S1V_heat_item, S2V_heat_item, N2H_heat_item]):
                        continue

                    # Validate Sample Heat
                    try:
                        sample_heat = float(sample_heat_item.text())
                        if not (0 <= sample_heat <= 100):
                            sample_heat = 0
                            item_sample = QTableWidgetItem(str(sample_heat))
                            item_sample.setTextAlignment(Qt.AlignCenter)
                            self.test_code_table.setItem(row, 2, item_sample)
                    except ValueError:
                        sample_heat = 0
                        item_sample = QTableWidgetItem(str(sample_heat))
                        item_sample.setTextAlignment(Qt.AlignCenter)
                        self.test_code_table.setItem(row, 2, item_sample)

                    # Validate Coil Heat
                    try:
                        coil_heat = float(coil_heat_item.text())
                        if not (0 <= coil_heat <= 100):
                            coil_heat = 0
                            item_coil = QTableWidgetItem(str(coil_heat))
                            item_coil.setTextAlignment(Qt.AlignCenter)
                            self.test_code_table.setItem(row, 3, item_coil)
                    except ValueError:
                        coil_heat = 0
                        item_coil = QTableWidgetItem(str(coil_heat))
                        item_coil.setTextAlignment(Qt.AlignCenter)
                        self.test_code_table.setItem(row, 3, item_coil)
                        
                    # Validate S1V
                    try:
                        S1V_heat = float(S1V_heat_item.text())
                        if not (0 <= S1V_heat <= 100):
                            S1V_heat = 0
                            item_S1V = QTableWidgetItem(str(S1V_heat))
                            item_S1V.setTextAlignment(Qt.AlignCenter)
                            self.test_code_table.setItem(row, 4, item_S1V)
                    except ValueError:
                        S1V_heat = 0
                        item_S1V = QTableWidgetItem(str(S1V_heat))
                        item_S1V.setTextAlignment(Qt.AlignCenter)
                        self.test_code_table.setItem(row, 4, item_S1V)                        
                        
                    # Validate S2V
                    try:
                        S2V_heat = float(S2V_heat_item.text())
                        if not (0 <= S2V_heat <= 100):
                            S2V_heat = 0
                            item_S2V = QTableWidgetItem(str(S2V_heat))
                            item_S2V.setTextAlignment(Qt.AlignCenter)
                            self.test_code_table.setItem(row, 5, item_S2V)
                    except ValueError:
                        S2V_heat = 0
                        item_S2V = QTableWidgetItem(str(S2V_heat))
                        item_S2V.setTextAlignment(Qt.AlignCenter)
                        self.test_code_table.setItem(row, 5, item_S2V)     
                                              
                    # Validate N2H
                    try:
                        N2H_heat = float(N2H_heat_item.text())
                        if not (0 <= S2V_heat <= 100):
                            N2H_heat = 0
                            item_N2H = QTableWidgetItem(str(N2H_heat))
                            item_N2H.setTextAlignment(Qt.AlignCenter)
                            self.test_code_table.setItem(row, 6, item_N2H)
                    except ValueError:
                        N2H_heat = 0
                        item_N2H = QTableWidgetItem(str(N2H_heat))
                        item_N2H.setTextAlignment(Qt.AlignCenter)
                        self.test_code_table.setItem(row, 6, item_N2H)   

                    test_code = test_code_item.text()
                    pulses = pulses_item.text()
                    cursor.execute(
                        """UPDATE test_code_data 
                        SET "Pulses" = ?, "SH(°C)" = ?, "CH(°C)" = ?, "S1V(°C)" = ?, "S2V(°C)" = ?, "N2H(°C)" = ?
                        WHERE "Test Code" = ?""",
                        (pulses, sample_heat, coil_heat, S1V_heat, S2V_heat, N2H_heat, test_code)
                    )

                # --- Update voc_data ---
                voc_expected_columns = ["VOC Pulse Code", "Mixed Constants (Nitrogen + VOC)", "T1 (sec)", "T2 (sec)"]
                voc_mapping = self.get_column_mapping(cursor, "voc_data", voc_expected_columns)
                if "Mixed Constants (Nitrogen + VOC)" not in voc_mapping:
                    print("Error: 'Mixed Constants (Nitrogen + VOC)' not found in voc_data table")
                    return

                cleanse_delay = int(integrate.get_cleanse_delay())

                for row in range(self.voc_table.rowCount()):
                    items = [self.voc_table.item(row, col) for col in range(4)]
                    if not all(items):
                        print(f"Missing item in voc_table at row {row}")
                        continue
                    voc_pulse_code, voc_percent, t1, t2 = [item.text() for item in items]

                    # Validate T1 and T2
                    try:
                        t1 = float(t1)
                        if t1 < cleanse_delay:
                            t1 = cleanse_delay
                            item_t1 = QTableWidgetItem(str(t1))
                            item_t1.setTextAlignment(Qt.AlignCenter)
                            self.voc_table.setItem(row, 2, item_t1)
                    except ValueError:
                        t1 = cleanse_delay
                        item_t1 = QTableWidgetItem(str(t1))
                        item_t1.setTextAlignment(Qt.AlignCenter)
                        self.voc_table.setItem(row, 2, item_t1)

                    try:
                        t2 = float(t2)
                        if t2 < 0:
                            t2 = 0
                            item_t2 = QTableWidgetItem(str(t2))
                            item_t2.setTextAlignment(Qt.AlignCenter)
                            self.voc_table.setItem(row, 3, item_t2)
                    except ValueError:
                        t2 = 0
                        item_t2 = QTableWidgetItem(str(t2))
                        item_t2.setTextAlignment(Qt.AlignCenter)
                        self.voc_table.setItem(row, 3, item_t2)

                    cursor.execute(
                        f"""UPDATE voc_data 
                            SET "{voc_mapping['Mixed Constants (Nitrogen + VOC)']}" = ?, 
                                "{voc_mapping['T1 (sec)']}" = ?,
                                "{voc_mapping['T2 (sec)']}" = ?
                            WHERE "{voc_mapping['VOC Pulse Code']}" = ?""",
                        (voc_percent, t1, t2, voc_pulse_code)
                    )

                # --- Update argon_data ---
                argon_expected_columns = ["Nitrogen Pulse Code", "Nitrogen % of Full Scale", "T0 (sec)", "T3 (sec)"]
                argon_mapping = self.get_column_mapping(cursor, "argon_data", argon_expected_columns)

                for row in range(self.argon_table.rowCount()):
                    items = [self.argon_table.item(row, col) for col in range(4)]
                    if not all(items):
                        print(f"Missing item in argon_table at row {row}")
                        continue
                    argon_pulse_code, argon_percent, t0, t3 = [item.text() for item in items]

                    # Validate T0 and T3
                    try:
                        t0 = float(t0)
                        if t0 != 0:
                            t0 = 0
                            item_t0 = QTableWidgetItem(str(t0))
                            item_t0.setTextAlignment(Qt.AlignCenter)
                            self.argon_table.setItem(row, 2, item_t0)
                    except ValueError:
                        t0 = 0
                        item_t0 = QTableWidgetItem(str(t0))
                        item_t0.setTextAlignment(Qt.AlignCenter)
                        self.argon_table.setItem(row, 2, item_t0)

                    try:
                        t3 = float(t3)
                        if t3 < 5:
                            t3 = 5
                            item_t3 = QTableWidgetItem(str(t3))
                            item_t3.setTextAlignment(Qt.AlignCenter)
                            self.argon_table.setItem(row, 3, item_t3)
                    except ValueError:
                        t3 = 5
                        item_t3 = QTableWidgetItem(str(t3))
                        item_t3.setTextAlignment(Qt.AlignCenter)
                        self.argon_table.setItem(row, 3, item_t3)

                    cursor.execute(
                        f"""UPDATE argon_data 
                            SET "{argon_mapping['Nitrogen % of Full Scale']}" = ?,
                                "{argon_mapping['T0 (sec)']}" = ?,
                                "{argon_mapping['T3 (sec)']}" = ?
                            WHERE "{argon_mapping['Nitrogen Pulse Code']}" = ?""",
                        (argon_percent, t0, t3, argon_pulse_code)
                    )

                conn.commit()
                print("Data saved successfully to database!")
                self.load_data_from_db()

        except sqlite3.Error as e:
            print(f"Database error: {e}")

    # Checkbox event handlers
    def on_sample_heat_checkbox_changed(self, state):
        if state == Qt.Checked:
            self.shared_state.set_Flag_Sample_Heat_chkbox(True)
        else:
            self.shared_state.set_Flag_Sample_Heat_chkbox(False)

        # Enable or disable the Temperature Monitor
        if self.shared_state.get_Flag_Sample_Heat_chkbox() or self.shared_state.get_Flag_Coil_Heat_chkbox() or self.shared_state.get_Flag_Air_quality_chkbox():
            integrate.set_temperature_monitor_active(True)
        else:
            integrate.set_temperature_monitor_active(False)

        # Update table
        current_sample_temp = integrate.get_sample_temperature() 
        if hasattr(self, 'temp_table'):
            item = QTableWidgetItem(f"{current_sample_temp:.1f}")
            item.setTextAlignment(Qt.AlignCenter)
            item.setFlags(item.flags() & ~Qt.ItemIsEditable)
            self.temp_table.setItem(0, 0, item)
    
    def on_coil_heat_checkbox_changed(self, state):
        if state == Qt.Checked:
            self.shared_state.set_Flag_Coil_Heat_chkbox(True)
        else:
            self.shared_state.set_Flag_Coil_Heat_chkbox(False)
        
        # Enable or disable the Temperature Monitor
        if self.shared_state.get_Flag_Sample_Heat_chkbox() or self.shared_state.get_Flag_Coil_Heat_chkbox() or self.shared_state.get_Flag_Air_quality_chkbox():
            integrate.set_temperature_monitor_active(True)
        else:
            integrate.set_temperature_monitor_active(False)
        
        # Update table
        current_tubing_temp = integrate.get_tubing_temperature()     
        if hasattr(self, 'temp_table'):
            item = QTableWidgetItem(f"{current_tubing_temp:.1f}")
            item.setTextAlignment(Qt.AlignCenter)
            item.setFlags(item.flags() & ~Qt.ItemIsEditable)
            self.temp_table.setItem(0, 1, item)
            
    def on_S1V_heat_checkbox_changed(self, state):
        if state == Qt.Checked:
            self.shared_state.Flag_S1V_Heat_chkbox = True
        else:
            self.shared_state.Flag_S1V_Heat_chkbox = False
        # Update table
        current_S1V_temp = integrate.get_tubing_temperature()     
        if hasattr(self, 'temp_table'):
            item = QTableWidgetItem(f"{current_S1V_temp:.1f}")
            item.setTextAlignment(Qt.AlignCenter)
            item.setFlags(item.flags() & ~Qt.ItemIsEditable)
            self.temp_table.setItem(0, 1, item)   
            
    def on_S2V_heat_checkbox_changed(self, state):
        if state == Qt.Checked:
            self.shared_state.Flag_S2V_Heat_chkbox = True
        else:
            self.shared_state.Flag_S2V_Heat_chkbox = False
        # Update table
        current_S2V_temp = integrate.get_tubing_temperature()     
        if hasattr(self, 'temp_table'):
            item = QTableWidgetItem(f"{current_S2V_temp:.1f}")
            item.setTextAlignment(Qt.AlignCenter)
            item.setFlags(item.flags() & ~Qt.ItemIsEditable)
            self.temp_table.setItem(0, 1, item)            
                     
    def on_N2H_heat_checkbox_changed(self, state):
        if state == Qt.Checked:
            self.shared_state.Flag_N2H_Heat_chkbox = True
        else:
            self.shared_state.Flag_N2H_Heat_chkbox = False
        # Update table
        current_N2H_temp = integrate.get_tubing_temperature()     
        if hasattr(self, 'temp_table'):
            item = QTableWidgetItem(f"{current_N2H_temp:.1f}")
            item.setTextAlignment(Qt.AlignCenter)
            item.setFlags(item.flags() & ~Qt.ItemIsEditable)
            self.temp_table.setItem(0, 1, item)            
  
    def on_aircube_heat_checkbox_changed(self, state):
        if state == Qt.Checked:
            self.shared_state.set_Flag_Air_quality_chkbox(True)
        else:
            self.shared_state.set_Flag_Air_quality_chkbox(False)
        
        # Enable or disable the Temperature Monitor
        if self.shared_state.get_Flag_Sample_Heat_chkbox() or self.shared_state.get_Flag_Coil_Heat_chkbox() or self.shared_state.get_Flag_Air_quality_chkbox():
            integrate.set_temperature_monitor_active(True)
        else:
            integrate.set_temperature_monitor_active(False)
        # Update table
        current_aircube_temp = float(integrate.get_air_quality_temperature())
        current_aircube_humidity = float(integrate.get_air_quality_humidity())
        current_aircube_pressure = float(integrate.get_air_quality_pressure())
        if hasattr(self, 'aircube_table'):
            temp_item = QTableWidgetItem(f"{current_aircube_temp:.3f}")
            temp_item.setTextAlignment(Qt.AlignCenter)
            temp_item.setFlags(temp_item.flags() & ~Qt.ItemIsEditable)
            self.aircube_table.setItem(0, 0, temp_item)

            humidity_item = QTableWidgetItem(f"{current_aircube_humidity:.3f}")
            humidity_item.setTextAlignment(Qt.AlignCenter)
            humidity_item.setFlags(humidity_item.flags() & ~Qt.ItemIsEditable)
            self.aircube_table.setItem(0, 1, humidity_item)

            pressure_item = QTableWidgetItem(f"{current_aircube_pressure:.3f}")
            pressure_item.setTextAlignment(Qt.AlignCenter)
            pressure_item.setFlags(pressure_item.flags() & ~Qt.ItemIsEditable)
            self.aircube_table.setItem(0, 2, pressure_item)

    # Measurement Table UI Part.
    def init_measurement_ui(self):
        measurement_widget = self.create_measurement_widget()
        self.measurement_layout.addWidget(measurement_widget)
        
    def create_table(self, rows, columns, headers, data, width, height, int_columns=[], float_columns=[]):
        table = QTableWidget(rows, columns)
        table.setRowCount(rows)
        table.setColumnCount(columns)
        table.setHorizontalHeaderLabels(headers)    
        table.setFixedSize(width, height)
        table.setEditTriggers(QAbstractItemView.AllEditTriggers)
    
        # Apply center alignment delegate to all columns
        table.setItemDelegate(CenterAlignDelegate())

        for col in int_columns:
            if 0 <= col < columns:
                table.setItemDelegateForColumn(col, IntDelegate(table))

        for col in float_columns:
            if 0 <= col < columns:
                table.setItemDelegateForColumn(col, FloatDelegate(table))

        table.setHorizontalHeaderLabels(headers)
        table.setFixedSize(width, height)
        table.setStyleSheet("""
            QTableWidget { 
                background-color: #1D2B3A; 
                color: #FFF; 
                font-size: 14px; 
                font-family: 'Times New Roman'; 
                font-weight: bold; 
                border: 1px solid #3A4F63; 
                border-radius: 10px; 
            }
            QTableWidget::item {
                text-align: center; 
                font-weight: bold; 
                font-size: 14px; 
                font-family: 'Times New Roman';
            }
            QHeaderView::section { 
                background-color: white; 
                color: #1D2B3A; 
                font-size: 14px; 
                font-weight: bold; 
                text-align: center; 
                border: 1px solid #3A4F63; 
            }
            QLineEdit {
                color: white;
                background-color: #2D3B4A;
                selection-background-color: #72B6DC;
                selection-color: white;
                text-align: center; 
                font-size: 14px; 
                font-weight: bold; 
                font-family: 'Times New Roman';
                border: 1px solid #3A4F63;
                padding: 2px;
            }
            QScrollBar:vertical {
                background: #3A4F63;
                width: 12px;
                margin: 0px;
                border: 1px solid #FFFFFF;
                border-radius: 4px;
            }
            QScrollBar::handle:vertical {
                background: #FFFFFF;
                min-height: 20px;
                border-radius: 4px;
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                background: none;
                height: 0px;
            }
            QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
                background: none;
            }
            QScrollBar:horizontal {
                background: #3A4F63;
                height: 12px;
                margin: 0px;
                border: 1px solid #2D3B4A;
                border-radius: 4px;
            }
            QScrollBar::handle:horizontal {
                background: #72B6DC;
                min-width: 20px;
                border-radius: 4px;
            }
            QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
                background: none;
                width: 0px;
            }
            QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
                background: none;
            }
        """)
        table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        table.verticalHeader().setVisible(False)
        table.setSelectionMode(QTableWidget.NoSelection)
        # Populate table and set editable only for float_columns
        for row in range(rows):
            for col in range(columns):
                item = QTableWidgetItem(str(data[row][col]))
                item.setTextAlignment(Qt.AlignCenter)

                if len(float_columns) > 0:
                    if col not in float_columns:
                        item.setFlags(item.flags() & ~Qt.ItemIsEditable)  # Make non-float columns read-only

                if len(int_columns) > 0:
                    if col not in int_columns:
                        item.setFlags(item.flags() & ~Qt.ItemIsEditable)  # Make non-float columns read-only

                table.setItem(row, col, item)
        
        return table

    def create_column(self, widgets, align, spacing=None):
        column = QVBoxLayout()
        column.setAlignment(align)
        if spacing:
            column.setSpacing(spacing)
        for widget, alignment in widgets:
            if widget:
                column.addWidget(widget, alignment=alignment if alignment else Qt.AlignCenter)
        return column

    def get_column_mapping(self, cursor, table_name, expected_names):
        cursor.execute(f"PRAGMA table_info({table_name});")
        columns = cursor.fetchall()
        actual_names = [col[1] for col in columns]
        mapping = {}
        for expected in expected_names:
            for actual in actual_names:
                if actual.strip() == expected.strip():
                    mapping[expected] = actual
                    break
            else:
                print(f"Warning: Expected column '{expected}' not found in table '{table_name}'")
        return mapping

    def on_sn1_changed(self, text):
        print("on_sn1_changed text:", text)
        self.shared_state.set_var_Sensor_SN1(self.sn1_edit.text().strip())

    def on_sn2_changed(self, text):
        print("on_sn2_changed text:", text)
        self.shared_state.set_var_Sensor_SN2(self.sn2_edit.text().strip())

    def on_sample1_changed(self, text):
        print("on_sample1_changed text:", text)
        self.shared_state.set_var_Sample_ID1(self.sample1_edit.text().strip())

    def on_sample2_changed(self, text):
        print("on_sample2_changed text:", text)
        self.shared_state.set_var_Sample_ID2(self.sample2_edit.text().strip())


    def create_measurement_widget(self):
        measurement_widget = QWidget()
        layout = QHBoxLayout(measurement_widget)
        layout.setSpacing(15)
        layout.setContentsMargins(10, 10, 10, 10)

        # ────────────────────────────────
        #        FIRST COLUMN (left sidebar controls)
        # ────────────────────────────────
        # ─── Vg / Vsd Status Section ───

        # Checkbox
        self.VgVsd_calibrate_checkbox = QCheckBox()
        self.VgVsd_calibrate_checkbox.setStyleSheet("QCheckBox { color: white; font-size: 16px; }")
        self.VgVsd_calibrate_checkbox.setChecked(True)
        self.VgVsd_calibrate_checkbox.hide()
        self.VgVsd_calibrate_checkbox.stateChanged.connect(self.on_vgvsd_checkbox_changed)

        vg_vsd_title_label = QLabel("Vg / VSD Config:")
        vg_vsd_title_label.setStyleSheet("color: #72B6DC; font-size: 18px; font-weight: bold; font-family: 'Times New Roman';")
        vg_vsd_title_label.setAlignment(Qt.AlignCenter)

        # Container for checkbox + label
        vg_vsd_container = QWidget()
        vg_vsd_layout = QHBoxLayout(vg_vsd_container)
        vg_vsd_layout.setContentsMargins(0, 0, 0, 0)
        vg_vsd_layout.setSpacing(6)

        vg_vsd_layout.addWidget(self.VgVsd_calibrate_checkbox)
        vg_vsd_layout.addWidget(vg_vsd_title_label)
        vg_vsd_layout.setAlignment(Qt.AlignCenter)

        self.vg_vsd_status_label = QLabel("Calibration Vg / VSD")
        self.vg_vsd_status_label.setStyleSheet(
            "color: #FFFFFF; font-size: 16px; font-weight: bold; font-family: 'Times New Roman';"
        )
        self.vg_vsd_status_label.setAlignment(Qt.AlignCenter)

        pre_sweep_label = QLabel("Pre-Sweep Delay")
        pre_sweep_label.setStyleSheet("color: #72B6DC; font-size: 16px; font-weight: bold; font-family: 'Times New Roman';")
        pre_sweep_label.setAlignment(Qt.AlignCenter)

        self.pre_sweep_edit = QLineEdit()
        self.pre_sweep_edit.setFixedSize(160, 40)
        self.pre_sweep_edit.setStyleSheet(
            "QLineEdit { background-color: #1D2B3A; color: #FFF; font-size: 20px; border: 2px solid #3A4F63; border-radius: 10px; text-align: center; }"
        )
        self.pre_sweep_edit.setText("5")
        self.pre_sweep_edit.setAlignment(Qt.AlignCenter)
        self.pre_sweep_edit.setValidator(QIntValidator(0, 999999))

        title_label = QLabel("  SELECT \n TEST CODE")
        title_label.setStyleSheet("color: #72B6DC; font-size: 20px; font-weight: bold; font-family: 'Times New Roman';")
        title_label.setAlignment(Qt.AlignCenter)

        self.test_code_edit = QLineEdit()
        self.test_code_edit.setFixedSize(160, 80)
        self.test_code_edit.setStyleSheet(
            "QLineEdit { background-color: #1D2B3A; color: #FFF; font-size: 60px; border: 2px solid #3A4F63; border-radius: 20px; text-align: center; }"
        )
        self.test_code_edit.setText("T5")
        self.test_code_edit.setAlignment(Qt.AlignCenter)

        self.set_button = QPushButton("SET")
        self.set_button.setFixedSize(150, 50)
        self.set_button.setStyleSheet(
            "QPushButton { background-color: #1D2B3A; color: #72B6DC; font-size: 16px; border: 2px solid #3A4F63; border-radius: 15px; padding: 8px; }"
        )
        self.set_button.clicked.connect(self.on_set_button_clicked)

        self.status_label = QLabel("")
        self.status_label.setStyleSheet("color: white; font-size: 14px; font-weight: bold; font-family: 'Times New Roman';")
        self.status_label.setAlignment(Qt.AlignCenter)

        # ─── Sample Information Section ───
        # Create a container widget for the grid layout
        sample_form_container = QWidget()
        sample_form_layout = QGridLayout(sample_form_container)
        sample_form_layout.setSpacing(6)

        label_style = "color: white; font-size: 16px; font-family: 'Times New Roman'; font-weight: bold;"
        edit_style = """
        QLineEdit {
            background-color: #1D2B3A;
            color: #FFF;
            font-size: 16px;
            border: 2px solid #3A4F63;
            border-radius: 8px;
            padding: 4px;
        }
        """

        # SN1
        sn1_label = QLabel("SN1")
        sn1_label.setStyleSheet(label_style)

        self.sn1_edit = QLineEdit()
        self.sn1_edit.setText("")
        self.sn1_edit.setPlaceholderText("Enter SN1")
        self.sn1_edit.setFixedWidth(150)
        self.sn1_edit.setMaxLength(16)
        self.sn1_edit.setStyleSheet("""
            QLineEdit {
                background: white;
                color: black;
                border: 1px solid #4A5F7A;
                border-radius: 6px;
                font-size: 14px;
                font-weight: bold;
                padding: 4px;
            }
        """)
        self.sn1_edit.textChanged.connect(self.on_sn1_changed)

        # SN2
        sn2_label = QLabel("SN2")
        sn2_label.setStyleSheet(label_style)

        self.sn2_edit = QLineEdit()
        self.sn2_edit.setText("")
        self.sn2_edit.setPlaceholderText("Enter SN2")
        self.sn2_edit.setFixedWidth(150)
        self.sn2_edit.setMaxLength(16)
        self.sn2_edit.setStyleSheet(self.sn1_edit.styleSheet()) 
        self.sn2_edit.textChanged.connect(self.on_sn2_changed)
        

        # Sample ID 1
        sample1_label = QLabel("Sample ID 1")
        sample1_label.setStyleSheet(label_style)

        self.sample1_edit = QLineEdit()
        self.sample1_edit.setText("")
        self.sample1_edit.setPlaceholderText("Enter Sample ID 1")
        self.sample1_edit.setFixedWidth(150)
        self.sample1_edit.setMaxLength(16)
        self.sample1_edit.setStyleSheet(self.sn1_edit.styleSheet())
        self.sample1_edit.textChanged.connect(self.on_sample1_changed)

        # Sample ID 2
        sample2_label = QLabel("Sample ID 2")
        sample2_label.setStyleSheet(label_style)

        self.sample2_edit = QLineEdit()
        self.sample2_edit.setText("")
        self.sample2_edit.setPlaceholderText("Enter Sample ID 2")
        self.sample2_edit.setFixedWidth(150)
        self.sample2_edit.setMaxLength(16)
        self.sample2_edit.setStyleSheet(self.sn1_edit.styleSheet())
        self.sample2_edit.textChanged.connect(self.on_sample2_changed)


        # Add to grid layout
        sample_form_layout.addWidget(sn1_label, 0, 0)
        sample_form_layout.addWidget(self.sn1_edit, 0, 1)
        sample_form_layout.addWidget(sn2_label, 1, 0)
        sample_form_layout.addWidget(self.sn2_edit, 1, 1)
        sample_form_layout.addWidget(sample1_label, 2, 0)
        sample_form_layout.addWidget(self.sample1_edit, 2, 1)
        sample_form_layout.addWidget(sample2_label, 3, 0)
        sample_form_layout.addWidget(self.sample2_edit, 3, 1)

        # Create first column
        first_column = self.create_column(
            [
                (vg_vsd_container, Qt.AlignCenter),       
                (self.vg_vsd_status_label, Qt.AlignCenter),
                (pre_sweep_label, Qt.AlignCenter),
                (self.pre_sweep_edit, Qt.AlignCenter),
                (title_label, Qt.AlignCenter),
                (self.test_code_edit, Qt.AlignCenter),
                (self.set_button, Qt.AlignCenter),
                (self.status_label, Qt.AlignCenter),
                (sample_form_container, Qt.AlignCenter)
            ],
            Qt.AlignCenter
        )
        layout.addLayout(first_column)

        # ────────────────────────────────
        #       SECOND COLUMN (main central content)
        # ────────────────────────────────
        self.vg_label = QLabel(f"Vg: {self.shared_state.var_vg_measure}")
        self.vg_label.setStyleSheet("color: #FFFFFF; font-size: 20px; font-weight: bold; font-family: 'Times New Roman';")
        self.vg_label.setAlignment(Qt.AlignCenter)

        self.vsd_label = QLabel(f"Vsd: {self.shared_state.var_vsd_measure}")
        self.vsd_label.setStyleSheet("color: #FFFFFF; font-size: 20px; font-weight: bold; font-family: 'Times New Roman';")
        self.vsd_label.setAlignment(Qt.AlignCenter)

        Ttotal = int(self.shared_state.T1 + self.shared_state.T2 + self.shared_state.T3)
        self.t_total_label = QLabel(f"T-Total: {Ttotal}")
        self.t_total_label.setStyleSheet("color: #FFFFFF; font-size: 20px; font-weight: bold; font-family: 'Times New Roman';")
        self.t_total_label.setAlignment(Qt.AlignCenter)

        image_label = QLabel()
        image_path = os.path.join(self.get_app_path1(), 'res', 'images', 'Asset.png')
        if os.path.exists(image_path):
            pixmap = QPixmap(image_path)
            image_label.setPixmap(pixmap.scaled(600, 480, Qt.KeepAspectRatio))
        image_label.setAlignment(Qt.AlignCenter)

        print("Creating Test code table")
        self.test_code_table = self.create_table(11, 7, ["Test Code", "Pulses", "SH(°C)", "CH(°C)", "S1V(°C)", "S2V(°C)", "N2H(°C)"], self.test_code_data, 600, 230, int_columns=[2, 3, 4, 5, 6], float_columns=[])

        self.temp_label = QLabel("Temperature")
        self.temp_label.setStyleSheet("color: #72B6DC; font-size: 18px; font-weight: bold; font-family: 'Times New Roman';")
        self.temp_label.setAlignment(Qt.AlignCenter)

        temp_headers = ["SH(°C)", "CH(°C)", "S1V(°C)", "S2V(°C)", "N2H(°C)"]
        temp_data = [[
            f"{integrate.get_sample_temperature():.1f}",
            f"{integrate.get_tubing_temperature():.1f}",
            "0.0",
            "0.0",
            "0.0"
        ]]
        self.temp_table = QTableWidget(1, 5)
        self.temp_table.setHorizontalHeaderLabels(temp_headers)
        self.temp_table.setFixedSize(400, 65)
        self.temp_table.setItemDelegate(CenterAlignDelegate())
        self.temp_table.setStyleSheet(self.get_table_style())
        self.temp_table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.temp_table.verticalHeader().setVisible(False)
        self.temp_table.setSelectionMode(QTableWidget.NoSelection)
        for col in range(5):
            item = QTableWidgetItem(temp_data[0][col])
            item.setTextAlignment(Qt.AlignCenter)
            item.setFlags(item.flags() & ~Qt.ItemIsEditable)
            self.temp_table.setItem(0, col, item)

        # Temperature checkboxes
        temp_check_container = QWidget()
        temp_check_layout = QHBoxLayout(temp_check_container)
        temp_check_layout.setSpacing(8)
        temp_check_layout.setContentsMargins(0, 0, 0, 0)

        self.sample_heat_check = QCheckBox("SH")
        self.sample_heat_check.setStyleSheet("color: #72B6DC; font-size: 14px; font-weight: bold; font-family: 'Times New Roman';")
        self.sample_heat_check.stateChanged.connect(self.on_sample_heat_checkbox_changed)
        
        self.coil_heat_check = QCheckBox("CH")
        self.coil_heat_check.setStyleSheet("color: #72B6DC; font-size: 14px; font-weight: bold; font-family: 'Times New Roman';")
        self.coil_heat_check.stateChanged.connect(self.on_coil_heat_checkbox_changed)

        self.s1v_heat_check = QCheckBox("S1V")
        self.s1v_heat_check.setStyleSheet("color: #72B6DC; font-size: 14px; font-weight: bold; font-family: 'Times New Roman';")
        self.s1v_heat_check.stateChanged.connect(self.on_S1V_heat_checkbox_changed)

        self.s2v_heat_check = QCheckBox("S2V")
        self.s2v_heat_check.setStyleSheet("color: #72B6DC; font-size: 14px; font-weight: bold; font-family: 'Times New Roman';")
        self.s2v_heat_check.stateChanged.connect(self.on_S2V_heat_checkbox_changed)

        self.n2h_heat_check = QCheckBox("N2H")
        self.n2h_heat_check.setStyleSheet("color: #72B6DC; font-size: 14px; font-weight: bold; font-family: 'Times New Roman';")
        self.n2h_heat_check.stateChanged.connect(self.on_N2H_heat_checkbox_changed)

        temp_check_layout.addWidget(self.sample_heat_check)
        temp_check_layout.addWidget(self.coil_heat_check)
        temp_check_layout.addWidget(self.s1v_heat_check)
        temp_check_layout.addWidget(self.s2v_heat_check)
        temp_check_layout.addWidget(self.n2h_heat_check)
        temp_check_layout.setAlignment(Qt.AlignCenter)

        self.aircube_heat_check = QCheckBox("AIRQ")
        self.aircube_heat_check.setStyleSheet("color: #72B6DC; font-size: 14px; font-weight: bold; font-family: 'Times New Roman';")
        self.aircube_heat_check.stateChanged.connect(self.on_aircube_heat_checkbox_changed)

        aircube_headers = ["Temperature(°C)", "Humidity(%)", "Pressure(hPa)"]
        aircube_data = [[
            f"{float(integrate.get_air_quality_temperature()):.3f}",
            f"{float(integrate.get_air_quality_humidity()):.3f}",
            f"{float(integrate.get_air_quality_pressure()):.3f}"
        ]]
        self.aircube_table = QTableWidget(1, 3)
        self.aircube_table.setHorizontalHeaderLabels(aircube_headers)
        self.aircube_table.setFixedSize(400, 65)
        self.aircube_table.setItemDelegate(CenterAlignDelegate())
        self.aircube_table.setStyleSheet(self.get_table_style())
        self.aircube_table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.aircube_table.verticalHeader().setVisible(False)
        self.aircube_table.setSelectionMode(QTableWidget.NoSelection)
        for col in range(3):
            item = QTableWidgetItem(aircube_data[0][col])
            item.setTextAlignment(Qt.AlignCenter)
            item.setFlags(item.flags() & ~Qt.ItemIsEditable)
            self.aircube_table.setItem(0, col, item)

        aircube_container = QWidget()
        aircube_layout = QVBoxLayout(aircube_container)
        aircube_layout.addWidget(self.aircube_heat_check, alignment=Qt.AlignLeft)
        aircube_layout.addWidget(self.aircube_table, alignment=Qt.AlignCenter)
        aircube_layout.setAlignment(Qt.AlignCenter)
        aircube_layout.setSpacing(6)

        second_column = self.create_column(
            [
                (self.vg_label, Qt.AlignCenter),
                (self.vsd_label, Qt.AlignCenter),
                (self.t_total_label, Qt.AlignCenter),
                (image_label, Qt.AlignCenter),
                (self.test_code_table, None),
                (self.temp_label, Qt.AlignCenter),
                (temp_check_container, Qt.AlignCenter),
                (self.temp_table, Qt.AlignCenter),
                (aircube_container, Qt.AlignCenter)
            ],
            Qt.AlignCenter
        )
        layout.addLayout(second_column, stretch=2)

        # ────────────────────────────────
        #       THIRD COLUMN (right side)
        # ────────────────────────────────
        print("Creating VOC table")
        self.voc_table = self.create_table(
            11, 4,
            ["VOC\nPulse Code", "Mixed Constants\n(Nitrogen + VOC)", "T1\n(sec)", "T2\n(sec)"],
            self.voc_data, 600, 240,
            int_columns=[0,1,2,3], float_columns=[]
        )

        print("Creating ARGON table")
        self.argon_table = self.create_table(
            11, 4,
            ["Nitrogen\nPulse Code", "Nitrogen %\nof Full Scale", "T0\n(sec)", "T3\n(sec)"],
            self.argon_data, 600, 240,
            int_columns=[0,1,2,3], float_columns=[]
        )

        # ─── Back Flush compact block ───
        self.backflush_block = QWidget()
        self.backflush_block.setStyleSheet("""
            QWidget {
                background-color: #1D2B3A;
                color: white;
                font-size: 21px;
                font-family: 'Times New Roman';
                font-weight: bold;
                border: none;
            }
        """)

        self.backflush_layout = QVBoxLayout(self.backflush_block)
        self.backflush_layout.setSpacing(10)
        self.backflush_layout.setContentsMargins(12, 12, 12, 12)
        self.backflush_block.setFixedWidth(380)

        # ─── Vial Buttons Section ───
        vial_section = QHBoxLayout()
        vial_section.setSpacing(10)

        btn_style = """
            QPushButton {
                background: #2A4A6A;
                color: white;
                border: none;
                border-radius: 6px;
                font-size: 14px;
                font-weight: bold;
            }
            QPushButton:hover { background: #3A5A7A; }
        """

        # UP Button
        self.vial_up_btn = QPushButton("UP")
        self.vial_up_btn.setFixedHeight(38)
        self.vial_up_btn.setStyleSheet(btn_style)
        self.vial_up_btn.clicked.connect(self.Up_Button_Click)

        # 1/3 Down Button
        self.one_third_down_btn = QPushButton("1/3 Position")
        self.one_third_down_btn.setFixedHeight(38)
        self.one_third_down_btn.setStyleSheet(btn_style)
        self.one_third_down_btn.clicked.connect(self.one_third_down_btn_click)

        # Full Down Button
        self.full_down_btn = QPushButton("Full Down")
        self.full_down_btn.setFixedHeight(38)
        self.full_down_btn.setStyleSheet(btn_style)
        self.full_down_btn.clicked.connect(self.full_down_btn_click)

        # Add buttons horizontally
        vial_section.addWidget(self.vial_up_btn)
        vial_section.addWidget(self.one_third_down_btn)
        vial_section.addWidget(self.full_down_btn)

        self.backflush_layout.addLayout(vial_section)

        bf_title = QLabel("Back Flush")
        bf_title.setAlignment(Qt.AlignCenter)

        time_row = QHBoxLayout()
        time_row.setSpacing(8)
        self.backflush_time_edit = QLineEdit()
        self.backflush_time_edit.setPlaceholderText("Enter the Time")
        self.backflush_time_edit.setFixedHeight(38)
        self.backflush_time_edit.setText("10")
        self.backflush_time_edit.setAlignment(Qt.AlignCenter)
        self.backflush_time_edit.setStyleSheet("""
            QLineEdit {
                background: white;
                color: black;
                border: 1px solid #4A5F7A;
                border-radius: 6px;
                font-size: 14px;
                padding: 4px;
            }s
        """)


        self.set_n2_btn = QPushButton("SET N2 Time")
        self.set_n2_btn.setFixedHeight(38)
        self.set_n2_btn.setStyleSheet("""
            QPushButton {
                background: #2A4A6A;
                color: white;
                border: none;
                border-radius: 6px;
                font-size: 14px;
                font-weight: bold;
            }
            QPushButton:hover { background: #3A5A7A; }
        """)
        self.set_n2_btn.clicked.connect(self.on_set_n2time_clicked)
        
        time_row.addWidget(self.backflush_time_edit, stretch=1)
        time_row.addWidget(self.set_n2_btn, stretch=1)

        # ── Status + controls BELOW ──
        status_container = QVBoxLayout()
        status_container.setSpacing(8)

        self.backflush_status_label = QLabel("Status : OFF")
        self.backflush_status_label.setStyleSheet("color: #FFFFFF; font-size: 20px; font-family: 'Times New Roman'; font-weight: bold;")
        self.backflush_status_label.setAlignment(Qt.AlignLeft)

        btn_row = QHBoxLayout()
        btn_row.setSpacing(12)

        self.on_btn = QPushButton("BACK FLUSH \n ON")
        self.on_btn.setFixedSize(95, 40)
        self.on_btn.setStyleSheet("""
            QPushButton {
                background: #2A4A6A;
                color: white;
                border: none;
                border-radius: 6px;
                font-size: 14px;
                font-weight: bold;
            }
            QPushButton:hover { background: #3A5A7A; }
        """)
        self.on_btn.clicked.connect(self.on_backflush_on_clicked)

        self.off_btn = QPushButton("BACK FLUSH \n OFF")
        self.off_btn.setFixedSize(95, 40)
        self.off_btn.setStyleSheet("""
            QPushButton {
                background: #2A4A6A;
                color: white;
                border: none;
                border-radius: 6px;
                font-size: 14px;
                font-weight: bold;
            }
            QPushButton:hover { background: #3A5A7A; }
        """)
        self.off_btn.clicked.connect(self.on_backflush_off_clicked)

        self.BackFlush_btn = QPushButton("BACK FLUSH \n Manual")
        self.BackFlush_btn.setFixedSize(95, 40)
        self.BackFlush_btn.setStyleSheet("""
            QPushButton {
                background: #2A4A6A;
                color: white;
                border: none;
                border-radius: 6px;
                font-size: 14px;
                font-weight: bold;
            }
            QPushButton:hover { background: #3A5A7A; }
        """)
        self.BackFlush_btn.clicked.connect(self.on_BackFlush_Button_Click)


        self.auto_checkbox = QCheckBox("Auto")
        self.auto_checkbox.setChecked(True)
        self.auto_checkbox.setStyleSheet("color: white; font-size: 14px; font-weight: bold;")
        self.auto_checkbox.stateChanged.connect(self.on_backflush_auto_changed)

        btn_row.addWidget(self.on_btn)
        btn_row.addWidget(self.off_btn)
        btn_row.addWidget(self.BackFlush_btn)
        btn_row.addWidget(self.auto_checkbox)
        btn_row.addStretch()

        status_container.addWidget(self.backflush_status_label, alignment=Qt.AlignLeft)
        status_container.addLayout(btn_row)

        # Assemble backflush block
        self.backflush_layout.addLayout(vial_section)
        self.backflush_layout.addWidget(bf_title)
        self.backflush_layout.addLayout(time_row)
        self.backflush_layout.addLayout(status_container)

        # Advanced Settings button
        self.advanced_button = QPushButton("Advanced\nSettings")
        self.advanced_button.setFixedSize(160, 80)
        self.advanced_button.setStyleSheet("""
            QPushButton {
                background-color: #1D2B3A;
                color: #72B6DC;
                font-size: 17px;
                border: 2px solid #4A5F7A;
                border-radius: 10px;
                padding: 8px;
            }
            QPushButton:hover {
                border: 2px solid #72B6DC;
            }
            QPushButton:pressed {
                background: #72B6DC;
                color: #0F1C2A;
            }
        """)
        self.advanced_button.clicked.connect(lambda: (
            print("Advanced settings clicked"),
            self.main_window.change_screen("AdvanceSettings"),
            self.main_window.sidebar_stack.setCurrentIndex(1)
        ))

        # ── Final third column layout ──
        third_column = QVBoxLayout()
        third_column.setSpacing(16)
        third_column.setAlignment(Qt.AlignTop)

        third_column.addWidget(self.voc_table)

        # New combined layout under Argon
        under_argon_layout = QHBoxLayout()
        under_argon_layout.setSpacing(16)
        under_argon_layout.addWidget(self.backflush_block)

        advanced_container = QVBoxLayout()
        advanced_container.addStretch(1)
        advanced_container.addWidget(self.advanced_button, alignment=Qt.AlignRight)
        under_argon_layout.addLayout(advanced_container)

        third_column.addWidget(self.argon_table)
        third_column.addLayout(under_argon_layout)
        third_column.addStretch(1)

        layout.addLayout(third_column, stretch=1)
        return measurement_widget

    def on_vgvsd_checkbox_changed(self):
        enabled = self.VgVsd_calibrate_checkbox.isChecked()
        if enabled :
            self.shared_state.set_Vg_Vsd_calibration_mode(True)
            print("Vg/Vsd Calibration Mode Enabled")
            self.vg_vsd_status_label.setText("Calibration Vg/Vsd")                    
            self.vg_label.hide()
            self.vsd_label.hide() 
        else:
            self.shared_state.set_Vg_Vsd_calibration_mode(False)
            print("Vg/Vsd Calibration Mode Disabled")           
            self.vg_vsd_status_label.setText("Advanced Settings Vg/Vsd")
            self.vg_label.show()
            self.vsd_label.show() 

            
    def on_backflush_on_clicked(self):
        try:
            self.shared_state.set_BackFlush_ON(True)
            self.shared_state.set_BackFlush_OFF(False)
            self.UI_Visiblity(False)
            #down vials
            if vial_base_is_home_position() == False:
                print("measurement over vials going down")
                self.backflush_status_label.setText("Status : Moving to Home Position")
                QApplication.processEvents()
                vial_base_lower(0)    

            ret = sv1_backflush_on(integrate.get_sv1_relay())        
            if not ret:
                print("Failed to enable backflash on clicked")
                self.backflush_status_label.setText("Status : ON Error")  
            else:
                self.backflush_status_label.setText("Status : ON")             
            self.UI_Visiblity(True)
            self.on_btn.setEnabled(False)  
        except Exception as e:
            print(f"Error in on_backflush_on_clicked: {e}")    
            self.backflush_status_label.setText(f"Status : Error Turning ON")
            self.UI_Visiblity(True)

    def on_backflush_off_clicked(self):
        try:
            self.UI_Visiblity(False)
            self.shared_state.set_BackFlush_OFF(True)
            self.shared_state.set_BackFlush_ON(False)
            ret = sv1_backflush_off(integrate.get_sv1_relay())
            if not ret:
                print("Failed to enable backflash off clicked")
                self.backflush_status_label.setText("Status : OFF Error")  
            else:
                self.backflush_status_label.setText("Status : OFF")  
            self.UI_Visiblity(True)
            self.off_btn.setEnabled(False) 
        except Exception as e:
            print(f"Error in on_backflush_off_clicked: {e}")  
            self.backflush_status_label.setText(f"Status : Error Turning OFF")
            self.UI_Visiblity(True)

    def on_BackFlush_Button_Click(self):
        try:          
            print("BackFlush Button Clicked")  
            self.backflush_status_label.setText("Status : Backflush is Running")   
            self.UI_Visiblity(False)
            integrate.set_stop_vial_Base(False)
            self.vial_command_signal.emit("BackFlush_operations", int(VIAL_1_3RD_POSITION))  
          
        except Exception as e:
            print(f"Error in BackFlush_operations: {e}")
            self.backflush_status_label.setText(f"Status : Error Running Backflush")
            self.UI_Visiblity(True)
     
       
    def on_backflush_auto_changed(self, state):
        val = (state == Qt.Checked)
        if val:
            self.shared_state.set_BackFlush_Auto(val)
            self.backflush_status_label.setText("Status : Auto")
        else:   
            self.shared_state.set_BackFlush_Auto(val)            
            if self.shared_state.get_BackFlush_ON():
                self.shared_state.set_BackFlush_OFF(False)
                self.backflush_status_label.setText("Status : ON")
            else:
                self.shared_state.set_BackFlush_ON(False)
                self.backflush_status_label.setText("Status : OFF")    

    def Up_Button_Click(self): 
        try: 
            print("UP Button Clicked") 
            self.backflush_status_label.setText("Status : Moving UP")
            self.UI_Visiblity(False)
            value = VIAL_UPPER_LIMIT
            self.vial_command_signal.emit("vial_base_UP", value)
        except Exception as e:
            print(f"Error in Up_Button_Click: {e}")

    def one_third_down_btn_click(self):       
        try: 
            print("1/3 Down Button Clicked")
            self.backflush_status_label.setText("Status : Moving to 1/3 Down Position")
            self.UI_Visiblity(False)
            value = VIAL_1_3RD_POSITION
            self.vial_command_signal.emit("vial_base_1_3RD_Down", value)
        except Exception as e:
            print(f"Error in Up_Button_Click: {e}")

                
    def full_down_btn_click(self):        
        try: 
            print("Full Down Button Clicked")
            self.backflush_status_label.setText("Status : Moving to Full Down Position")
            self.UI_Visiblity(False)
            value = VIAL_LOWER_LIMIT
            self.vial_command_signal.emit("vial_base_FULL_Down", value)
        except Exception as e:
            print(f"Error in Up_Button_Click: {e}")
            self.UI_Visiblity(True)
    
    def on_set_n2time_clicked(self):
        try:
            value_text = self.backflush_time_edit.text().strip()
            
            # Validate N2 Time - should not be empty
            if not value_text:
                self.show_alert("Validation Error", "N2 Time cannot be empty")
                return
            
            # Validate N2 Time - should be integer
            try:
                seconds = int(value_text)
            except ValueError:
                self.show_alert("Validation Error", "N2 Time must be a valid integer")
                return
            
            # Validate N2 Time - must be greater than 0
            if seconds < 0:
                self.show_alert("Validation Error", f"N2 Time minimum value is 0 seconds\nCurrent value: {seconds}")
                return
            
            print(f"Setting N2 backflush time to: {seconds} seconds")
            self.backflush_status_label.setText("Status : N2 Time is Updated")
            integrate.set_N2_Period(seconds)
            
        except Exception as e:
            print(f"Error in on_set_n2time_clicked: {e}")
            self.show_alert("Error", f"Unexpected error: {str(e)}")

    def on_generic_heat_checkbox_changed(self, state, heat_type):
        if state == Qt.Checked:
            setattr(self.shared_state, f"Flag_{heat_type}_Heat_chkbox", True)
        else:
            setattr(self.shared_state, f"Flag_{heat_type}_Heat_chkbox", False)
        # Update table
        if hasattr(self, 'temp_table'):
            col_index = {"S1V": 2, "S2V": 3, "N2H": 4}
            if heat_type in col_index:
                item = QTableWidgetItem("0.0")
                item.setTextAlignment(Qt.AlignCenter)
                item.setFlags(item.flags() & ~Qt.ItemIsEditable)
                self.temp_table.setItem(0, col_index[heat_type], item)

    def get_table_style(self):
        """Common stylesheet used for all tables in the UI"""
        return """
            QTableWidget { 
                background-color: #1D2B3A; 
                color: #FFF; 
                font-size: 14px; 
                font-family: 'Times New Roman'; 
                font-weight: bold; 
                border: 1px solid #3A4F63; 
                border-radius: 10px; 
            }
            QTableWidget::item {
                text-align: center; 
                font-weight: bold; 
                font-size: 14px; 
                font-family: 'Times New Roman';
            }
            QHeaderView::section { 
                background-color: white; 
                color: #1D2B3A; 
                font-size: 14px; 
                font-weight: bold; 
                text-align: center; 
                border: 1px solid #3A4F63; 
            }
            QLineEdit {
                color: white;
                background-color: #2D3B4A;
                selection-background-color: #72B6DC;
                selection-color: white;
                text-align: center; 
                font-size: 14px; 
                font-weight: bold; 
                font-family: 'Times New Roman';
                border: 1px solid #3A4F63;
                padding: 2px;
            }
            QScrollBar:vertical {
                background: #3A4F63;
                width: 12px;
                margin: 0px;
                border: 1px solid #FFFFFF;
                border-radius: 4px;
            }
            QScrollBar::handle:vertical {
                background: #FFFFFF;
                min-height: 20px;
                border-radius: 4px;
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                background: none;
                height: 0px;
            }
            QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
                background: none;
            }
            QScrollBar:horizontal {
                background: #3A4F63;
                height: 12px;
                margin: 0px;
                border: 1px solid #2D3B4A;
                border-radius: 4px;
            }
            QScrollBar::handle:horizontal {
                background: #72B6DC;
                min-width: 20px;
                border-radius: 4px;
            }
            QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
                background: none;
                width: 0px;
            }
            QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
                background: none;
            }
        """

    def update_heat_controls_visibility(self):
        """Update visibility of heat controls based on Enable_Bricklets flag."""

        mvp_version = self.shared_state.get_MVP_Device_Version().rsplit("_", 1)[-1]        
        is_visible = self.shared_state.Enable_Bricklets 
        
        if hasattr(self, 'temp_label'):
            self.temp_label.setVisible(is_visible)
        if hasattr(self, 'temp_table'):
            self.temp_table.setVisible(is_visible)
        self.sample_heat_check.setVisible(is_visible)
        self.coil_heat_check.setVisible(is_visible)
        self.s1v_heat_check.setVisible(is_visible)
        self.s2v_heat_check.setVisible(is_visible)
        self.n2h_heat_check.setVisible(is_visible)
        self.aircube_heat_check.setVisible(is_visible)
        if hasattr(self, 'aircube_table'):
            self.aircube_table.setVisible(is_visible)
        
        if mvp_version == "1.0":
            is_visible = False  # Hide backflush controls for MVP 1.0   
            
        if hasattr(self, 'backflush_block'):
            self.backflush_block.setVisible(is_visible)

    def reset_checkboxes(self,Flag):
        self.sample_heat_check.setChecked(Flag)
        self.coil_heat_check.setChecked(Flag)
        self.s1v_heat_check.setChecked(Flag)
        self.s2v_heat_check.setChecked(Flag)
        self.n2h_heat_check.setChecked(Flag)
        self.aircube_heat_check.setChecked(Flag)

    def on_set_button_clicked(self):
        try: 
            # Update pre-sweep delay from user input
            try:
                pre_sweep_delay = int(self.pre_sweep_edit.text().strip())
                self.shared_state.Pre_sweep_delay = pre_sweep_delay
                print(f"Pre-Sweep Delay updated to: {self.shared_state.Pre_sweep_delay}")
            except ValueError:
                self.status_label.setText("Invalid Pre-Sweep Delay value")
                return

            selected_test_code = self.test_code_edit.text().strip()
            valid_test_codes = [f"T{i}" for i in range(1, 12)]
            if selected_test_code not in valid_test_codes:
                self.status_label.setText("Invalid Test Code")
                return

            pulse_value = next((row[1] for row in self.test_code_data if row[0] == selected_test_code), None)
            if not pulse_value:
                self.status_label.setText("No pulse value \n found for test code")
                return

            argon_row, voc_row = None, None

            if '+' in pulse_value:
                argon_pulse_code, voc_pulse_code = map(str.strip, pulse_value.split('+'))

                # Fetch Nitrogen data
                for row in range(self.argon_table.rowCount()):
                    if self.argon_table.item(row, 0) and self.argon_table.item(row, 0).text() == argon_pulse_code:
                        try:
                            argon_t0 = float(self.argon_table.item(row, 2).text()) if self.argon_table.item(row, 2) else 0
                            argon_t3 = float(self.argon_table.item(row, 3).text()) if self.argon_table.item(row, 3) else 0

                            # Conditional defaults and table update
                            if argon_t0 != 0:
                                argon_t0 = 0
                                self.show_info_message("T0 should be 0. Default set to 0.")                                
                                item_t0 = QTableWidgetItem(str(argon_t0))
                                item_t0.setTextAlignment(Qt.AlignCenter)
                                self.argon_table.setItem(row, 2, item_t0)

                            if argon_t3 < 5:
                                argon_t3 = 5
                                self.show_info_message("T3 should be minimum 5 seconds. Default set to 5.")
                                item_t3 = QTableWidgetItem(str(argon_t3))
                                item_t3.setTextAlignment(Qt.AlignCenter)
                                self.argon_table.setItem(row, 3, item_t3)

                            argon_row = (argon_pulse_code, self.argon_table.item(row, 1).text(), argon_t0, argon_t3)

                        except (ValueError, AttributeError):
                            self.status_label.setText("Invalid numeric values \n in Nitrogen table")
                            return
                        break
                else:
                    self.status_label.setText("Missing data for Nitrogen \n pulse code")
                    return

                # Fetch VOC data
                for row in range(self.voc_table.rowCount()):
                    if self.voc_table.item(row, 0) and self.voc_table.item(row, 0).text() == voc_pulse_code:
                        try:
                            voc_t1 = float(self.voc_table.item(row, 2).text()) if self.voc_table.item(row, 2) else 0
                            voc_t2 = float(self.voc_table.item(row, 3).text()) if self.voc_table.item(row, 3) else 0

                            # Conditional defaults and table update
                            cleansedelay = int(integrate.get_cleanse_delay())
                            if voc_t1 < cleansedelay:
                                voc_t1 = cleansedelay
                                item_t1 = QTableWidgetItem(str(voc_t1))
                                item_t1.setTextAlignment(Qt.AlignCenter)
                                self.show_info_message(f"T1 should be minimum {cleansedelay} seconds. Default set to {cleansedelay}.")
                                self.voc_table.setItem(row, 2, item_t1)

                            if voc_t2 < 0:
                                voc_t2 = 0
                                item_t2 = QTableWidgetItem(str(voc_t2))
                                item_t2.setTextAlignment(Qt.AlignCenter)
                                self.show_info_message("T2 should be minimum 0 second. Default set to 0.")
                                self.voc_table.setItem(row, 3, item_t2)

                            voc_row = (voc_pulse_code, self.voc_table.item(row, 1).text(), voc_t1, voc_t2)
                            print("voc_row ----", voc_row)

                        except (ValueError, AttributeError):
                            voc_t1 = cleansedelay
                            voc_t2 = 0
                            voc_row = (voc_pulse_code, self.voc_table.item(row, 1).text(), voc_t1, voc_t2)
                            self.voc_table.setItem(row, 2, QTableWidgetItem(str(voc_t1)))
                            self.voc_table.setItem(row, 3, QTableWidgetItem(str(voc_t2)))
                            print("voc_row with defaults ----", voc_row)
                        break
                else:
                    self.status_label.setText("Missing data for VOC \n pulse code")
                    return

                # Save to shared_state
                self.shared_state.set_current_test_code(selected_test_code)
                self.shared_state.set_current_pulse_value(pulse_value)
                self.shared_state.voc_data = [voc_row]
                self.shared_state.set_argon_data([argon_row])

                # Set integrate values
                try:
                    Argon_flow_rate = int(float(argon_row[1]))
                    integrate.set_Argon_flow_rate(Argon_flow_rate)
                    VOC_argon_flow_rate = int(float(voc_row[1]))
                    integrate.set_VOC_argon_flow_rate(VOC_argon_flow_rate) 
                except AttributeError as e:
                    print(f"Error accessing integrate module attributes: {e}")
                    self.status_label.setText("Internal module error")
                    return

                self.shared_state.set_T0(int(argon_t0))
                self.shared_state.set_T1(int(voc_t1))
                self.shared_state.set_T2(int(voc_t2))
                self.shared_state.set_T3(int(argon_t3))
              

                try:
                    integrate.set_t0(self.shared_state.get_T0())
                    integrate.set_t1(self.shared_state.get_T1())
                    integrate.set_t2(self.shared_state.get_T2())
                    integrate.set_t3(self.shared_state.get_T3())

                except AttributeError as e:
                    print(f"Error setting integrate module times: {e}")
                    self.status_label.setText("Internal module error")
                    return

                integrate.set_Count(0)
                # Calculate T-Total
                Ttotal = int(argon_t0 + voc_t1 + voc_t2 + argon_t3)
                self.t_total_label.setText(f"T-Total: {Ttotal}")

                # Sample and Coil Heat
                for row in range(self.test_code_table.rowCount()):
                    sample_val = self.test_code_table.item(row, 2).text() if self.test_code_table.item(row, 2) else "0"
                    coil_val = self.test_code_table.item(row, 3).text() if self.test_code_table.item(row, 3) else "0"
                    S1V_val = self.test_code_table.item(row, 4).text() if self.test_code_table.item(row, 4) else "0"
                    S2V_val = self.test_code_table.item(row, 5).text() if self.test_code_table.item(row, 5) else "0"
                    N2H_val = self.test_code_table.item(row, 6).text() if self.test_code_table.item(row, 6) else "0"

                    # Validate Sample Heat
                    try:
                        sample_val_float = float(sample_val)
                    except ValueError:
                        sample_val_float = 0
                    if sample_val_float < 0 or sample_val_float > 100:
                        sample_val_float = 0
                        item_sample = QTableWidgetItem(str(sample_val_float))
                        item_sample.setTextAlignment(Qt.AlignCenter)
                        self.test_code_table.setItem(row, 2, item_sample)
                        self.show_info_message("Sample temperature should be 0–100°C. Default set to 0.")

                    # Validate Coil Heat
                    try:
                        coil_val_float = float(coil_val)
                    except ValueError:
                        coil_val_float = 0
                    if coil_val_float < 0 or coil_val_float > 100:
                        coil_val_float = 0
                        item_coil = QTableWidgetItem(str(coil_val_float))
                        item_coil.setTextAlignment(Qt.AlignCenter)
                        self.test_code_table.setItem(row, 3, item_coil)
                        self.show_info_message("Coil temperature should be 0–100°C. Default set to 0.")
                        
                    # Validate S1V 
                    try:
                        S1V_val_float = float(S1V_val)
                    except ValueError:
                        S1V_val_float = 0
                    if S1V_val_float < 0 or S1V_val_float > 100:
                        S1V_val_float = 0
                        item_S1V = QTableWidgetItem(str(S1V_val_float))
                        item_S1V.setTextAlignment(Qt.AlignCenter)
                        self.test_code_table.setItem(row, 4, item_S1V)
                        self.show_info_message("S1V temperature should be 0–100°C. Default set to 0.")   
                        
                    # Validate S2V 
                    try:
                        S2V_val_float = float(S2V_val)
                    except ValueError:
                        S2V_val_float = 0
                    if S2V_val_float < 0 or S2V_val_float > 100:
                        S2V_val_float = 0
                        item_S2V = QTableWidgetItem(str(S2V_val_float))
                        item_S2V.setTextAlignment(Qt.AlignCenter)
                        self.test_code_table.setItem(row, 5, item_S2V)
                        self.show_info_message("S2V temperature should be 0–100°C. Default set to 0.")    
                        
                    # Validate N2H 
                    try:
                        N2H_val_float = float(N2H_val)
                    except ValueError:
                        N2H_val_float = 0
                    if N2H_val_float < 0 or N2H_val_float > 100:
                        N2H_val_float = 0
                        item_N2H = QTableWidgetItem(str(N2H_val_float))
                        item_N2H.setTextAlignment(Qt.AlignCenter)
                        self.test_code_table.setItem(row, 6, item_N2H)
                        self.show_info_message("N2H temperature should be 0–100°C. Default set to 0.")                                                                   
                        

                # Save selected test code heat values
                index = int(selected_test_code[1:]) - 1
                self.shared_state.Sample_Heat = float(self.test_code_table.item(index, 2).text())
                self.shared_state.Coil_Heat = float(self.test_code_table.item(index, 3).text())
                self.shared_state.S1V_Heat = float(self.test_code_table.item(index, 4).text())
                self.shared_state.S2V_Heat = float(self.test_code_table.item(index, 5).text())
                self.shared_state.N2H_Heat = float(self.test_code_table.item(index, 6).text())
                
                print("self.shared_state.Sample_Heat",self.shared_state.Sample_Heat)
                integrate.set_heater_temperature(self.shared_state.Sample_Heat)
                integrate.set_tubing_coil_temp(self.shared_state.Coil_Heat)
                integrate.set_S1V_temp(self.shared_state.S1V_Heat)
                integrate.set_S2V_temp(self.shared_state.S2V_Heat)
                integrate.set_N2H_temp(self.shared_state.N2H_Heat)
                
                print("heater_temperature ", integrate.get_heater_temperature())
                print("tubing_coil_temp ", integrate.get_tubing_coil_temp())
                print("S1V_temp ", integrate.get_S1V_temp())
                print("S2V_temp ", integrate.get_S2V_temp())
                print("N2H_temp ", integrate.get_N2H_temp)

                integrate.set_heater_calibration(True)
                integrate.set_temperature_time_difference()

                self.status_label.setText(f"Value of {selected_test_code} is Updated")

        except AttributeError as e:
            print(f"Error Set Button click: {e}")          
            self.status_label.setText("Internal error occurred")

    def show_info_message(self, text):
        if self.main_window.screens["Settings"].isVisible():
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Information)
            msg.setText(text)
            msg.setWindowTitle("Info")
            msg.setStandardButtons(QMessageBox.Ok)
            msg.exec_()


    def UI_Visiblity(self,bool):
        self.logout_button.setEnabled(bool)
        self.toggle_sidebar_signal.emit(bool)
        self.backflush_time_edit.setEnabled(bool)
        self.set_n2_btn.setEnabled(bool)
        self.on_btn.setEnabled(bool)
        self.off_btn.setEnabled(bool)
        self.vial_up_btn.setEnabled(bool)
        self.one_third_down_btn.setEnabled(bool)
        self.full_down_btn.setEnabled(bool)
        self.auto_checkbox.setEnabled(bool)
        self.BackFlush_btn.setEnabled(bool)
        self.advanced_button.setEnabled(bool)
        self.set_button.setEnabled(bool)
        self.sample_heat_check.setEnabled(bool)
        self.coil_heat_check.setEnabled(bool)
        self.s1v_heat_check.setEnabled(bool)
        self.s2v_heat_check.setEnabled(bool)
        self.n2h_heat_check.setEnabled(bool)
        self.argon_table.setEnabled(bool)
        self.voc_table.setEnabled(bool)
        self.test_code_table.setEnabled(bool)
        self.temp_table.setEnabled(bool)
        self.aircube_table.setEnabled(bool)

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


