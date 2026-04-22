"""
Device Test Script
===================
 
Power Up Sequence to Device READY:
 1.Set sensor 1,2 of Nitrogen MFC to 0% 
 2.Set sensor 1,2 of VOC MFC to 0% 
 3.Energize exhaust Solenoid Valve = on/Open 2f3R SW0
 4.Set sensor 1,2 of Nitrogen MFC to 30% for 5sec then 0%
 5.Set sensor 1,2 of Argon MFC to 30% for 5sec then 0%
 6.Exhaust Solenoid Valve = Closed 2f3R SW0


- Modularized with separate functions for each device.
- Functions can be modified independently for future updates.

Test Sequence:


"""
import os
import threading
import time
from datetime import datetime, timedelta
from TinkerForge.ThermoCouple_1 import tc1_initialize_thermocouple
from TinkerForge.ThermoCouple_2 import tc2_initialize_thermocouple
from TinkerForge.Bricklet_ID_Configuration import mfc1_serial_port, mfc2_serial_port, mfc3_serial_port, mfc4_serial_port
from TinkerForge.temp_config import CONTROL_LOOP_INTERVAL_SEC,REQUIRED_TEMPERATURE_C,TEMP_HISTORY_LENGTH 
from TinkerForge.heater_control import HeaterController,HEATER_ON,HEATER_OFF
#from TinkerForge.MFC_def import Sfc6
from TinkerForge.SFC5xxx import mfc_sfc5
from SharedState import SharedState
from TinkerForge.SolenoidValve_Qrelay import sv1_initialize_relay, sv1_turn_on_relay, sv1_backflush_relay, sv1_turn_off_relay,sv1_get_relay_status,sv1_backflush_off, sv1_backflush_on
from TinkerForge.dual_relay import dual_initialize_relay,dual_turn_on_relay, dual_turn_off_relay,dual_get_relay_status
from TinkerForge.AirQuality import aq_initialize
from TinkerForge.Vial_Base import vial_start_home, vial_base_off,vial_base_initialize_stepper, vial_base_position, vial_base_home_stepper, vial_base_raise, vial_base_is_home_position, vial_base_lower, vial_base_status, VIAL_UPPER_LIMIT,VIAL_1_3RD_POSITION
from PyQt5.QtCore import QObject, pyqtSignal

class Update_steps(QObject):
    Update_steps_count_signal = pyqtSignal(int)
    
update_steps = Update_steps()

NITROGEN_GAS_4 = 4
TARGET_TEMP = 50

thermocouple1 = None 
thermocouple2 = None
thermocouple3 = None
thermocouple4 = None
thermocouple5 = None
sv1_relay = None 
mfc_sfc6 = None
controller    = None   
dual_relay = None
vial_base = None
S1V_temp = 0.0
S2V_temp = 0.0
N2H_temp = 0.0
sample_temperature = 0.0
tubing_temperature = 0.0
air_quality_humidity = 0.0
air_quality_temperature = 0.0
air_quality_pressure = 0.0
vortex_gen_1_temperature = 0.0
vortex_gen_2_temperature = 0.0
n2_gas_temperature = 0.0
meas_count = 0
meas_max_count = 0
ttotal = 0
value =2
sampling_time = 0

update_status_callback = None
#Shared Variables
t0 = 0
t1 = 0
t2 = 0
t3 =  0
current_tag_id1 = None
current_tag_id2 = None
current_tag_id3 = None
current_tag_id4 = None
Sweep_Measurement = False
Normal_Measurement = False
Calibrate_Measurement = False
mainmeasurement = False
Argon_flow_rate = None
VOC_argon_flow_rate = None
heater_temperature = 0.0
tubing_coil_temp = 0.0
Start_Bricketlet_T0_flow = False    
#start_bricklet = False
view_graph = False
view_graph1 = False
view_graph2 = False
startmeasurement1 = False
startmeasurement1_T0_flow = False
startmeasurement2 = False
measurement1Completed = False
measurement2Completed = False
single_device_connected = False
enable_bricklet = False
enable_print = True 
current_argon_flow = None
start_timer = False
Count = 0
Count1 = 0
stopmeasurement1 = False
stopmeasurement2 = False
exit_measurement_thread1 = False
exit_measurement_thread2 = False
transfer_rawtext_file = False
stop_measurement_data_acq = False
temperature_monitor = False
Appclose = False
check_temperature = False
Cancel_Measurement = False

serial_ports = [
        mfc1_serial_port, mfc2_serial_port, mfc3_serial_port, mfc4_serial_port
]

nirogen_serial_ports = [
        mfc1_serial_port, mfc3_serial_port
]

voc_serial_ports = [
        mfc2_serial_port, mfc4_serial_port
]

cleanse_delay = 5

# Lock for synchronizing access
_shared_state_lock = threading.Lock()
_disconnect_lock = threading.Lock()
start_bricklet_event = threading.Event()
Start_timer_event = threading.Event()
startmeasurement1_event = threading.Event()
startmeasurement1_T0_flow_event = threading.Event()
startmeasurement2_event = threading.Event()
Start_data_acquisition_event = threading.Event()	
Start_dac_configuration_event = threading.Event()	

temperature_monitor_active = False
bricklet_initialization_completed = False

temperature_log_active = False
temperature_log_filepath = None

ConnectionThread_Closed = False
BrickletThread_Closed = False
TimerThread_Closed = False
TemperaturThread_Closed = False
MeasurmentThread_Closed = False
Vial_Thread_Closed = False

stop_vial_Base  = False

# Screen switching flag
_screen_switching = False
N2_Period = 10
steps_count = 0 
temperature_time_diff = 0.0
heater_calibration = False
def set_heater_calibration(val1: bool):
    global heater_calibration
    with _shared_state_lock:
        heater_calibration = val1

def get_heater_calibration():
    global heater_calibration
    with _shared_state_lock:
        return heater_calibration

def set_temperature_time_difference():
    sample_temp = get_sample_temperature()
    dual_target_temperature = get_heater_temperature()
    if (sample_temp - dual_target_temperature) < 10:
        set_temperature_time_diff(1.5)

    elif (sample_temp - dual_target_temperature) < 20:
        set_temperature_time_diff(3.0)
        

    elif (sample_temp - dual_target_temperature) < 30:
        set_temperature_time_diff(4.0)
    
    else:
        set_temperature_time_diff(5.0)

def set_temperature_time_diff(val: float):
    global temperature_time_diff
    with _shared_state_lock:
        temperature_time_diff = val

def get_temperature_time_diff():
    global temperature_time_diff
    with _shared_state_lock:
        return temperature_time_diff

def set_steps_count(Count: int):    
    global steps_count
    with _shared_state_lock:
        steps_count = Count
        
def get_steps_count():
    global steps_count
    with _shared_state_lock:
        return steps_count

def set_N2_Period(period: int):    
    global N2_Period
    with _shared_state_lock:
        N2_Period = period
        
def get_N2_Period():
    global N2_Period
    with _shared_state_lock:
        return N2_Period

def get_screen_switching():
    global _screen_switching
    return _screen_switching

def set_screen_switching(value: bool):
    global _screen_switching
    _screen_switching = value

def set_dual_relay(device):
    global dual_relay
    with _shared_state_lock:
        dual_relay = device

def get_dual_relay():
    global dual_relay
    with _shared_state_lock:
        return dual_relay
    
def set_sv1_relay(device):
    global sv1_relay
    with _shared_state_lock:
        sv1_relay = device

def get_sv1_relay():
    global sv1_relay
    with _shared_state_lock:
        return sv1_relay

def set_vial_base(device):
    global vial_base
    with _shared_state_lock:
        vial_base = device

def get_vial_base():
    global vial_base
    with _shared_state_lock:
        return vial_base

def set_ConnectionThread_Closed(val1: bool):
    global ConnectionThread_Closed
    with _shared_state_lock:
        ConnectionThread_Closed = val1

def get_ConnectionThread_Closed():
    global ConnectionThread_Closed
    with _shared_state_lock:
        return ConnectionThread_Closed

def set_BrickletThread_Closed(val1: bool):
    global BrickletThread_Closed
    with _shared_state_lock:
        BrickletThread_Closed = val1

def get_BrickletThread_Closed():
    global BrickletThread_Closed
    with _shared_state_lock:
        return BrickletThread_Closed
    
def set_TimerThread_Closed(val1: bool):
    global TimerThread_Closed
    with _shared_state_lock:
        TimerThread_Closed = val1

def get_TimerThread_Closed():
    global TimerThread_Closed
    with _shared_state_lock:
        return TimerThread_Closed
    
def set_TemperaturThread_Closed(val1: bool):
    global TemperaturThread_Closed
    with _shared_state_lock:
        TemperaturThread_Closed = val1

def get_TemperaturThread_Closed():
    global TemperaturThread_Closed
    with _shared_state_lock:
        return TemperaturThread_Closed    
    
def set_MeasurmentThread_Closed(val1: bool):
    global MeasurmentThread_Closed
    with _shared_state_lock:
        MeasurmentThread_Closed = val1

def get_MeasurmentThread_Closed():
    global MeasurmentThread_Closed
    with _shared_state_lock:
        return MeasurmentThread_Closed  


def set_Vial_Thread_Closed(val1: bool):
    global Vial_Thread_Closed
    with _shared_state_lock:
        Vial_Thread_Closed = val1

def get_Vial_Thread_Closed():
    global Vial_Thread_Closed
    with _shared_state_lock:
        return Vial_Thread_Closed  
    
def set_stop_vial_Base(val1: bool):
    global stop_vial_Base
    with _shared_state_lock:
        stop_vial_Base = val1

def get_stop_vial_Base():
    global stop_vial_Base
    with _shared_state_lock:
        return stop_vial_Base    

def set_check_temperature(val1: bool):
    global check_temperature
    with _shared_state_lock:
        check_temperature = val1

def get_check_temperature():
    global check_temperature
    with _shared_state_lock:
        return check_temperature 
        
def set_Cancel_Measurement(val1: bool):
    global Cancel_Measurement
    with _shared_state_lock:
        Cancel_Measurement = val1

def get_Cancel_Measurement():
    global Cancel_Measurement
    with _shared_state_lock:
        return Cancel_Measurement            
    
def set_temperature_log_active(val1: bool):
    global temperature_log_active
    with _shared_state_lock:
        temperature_log_active = val1

def get_temperature_log_active():
    global temperature_log_active
    with _shared_state_lock:
        return temperature_log_active

def set_temperature_log_filepath(filepath: str):    
    global temperature_log_filepath
    with _shared_state_lock:
        temperature_log_filepath = filepath
        
def get_temperature_log_filepath():
    global temperature_log_filepath
    with _shared_state_lock:
        return temperature_log_filepath 


def log_temperature_data(measurement_screen, filepath, sample_temp, tubing_temp,vortex_gen_1_temp,vortex_gen_2_temp,n2_gas_temp, air_temp, air_humidity,air_pressure):
    try:
        parent_dir = os.path.dirname(filepath)
        if not os.path.exists(parent_dir):
            print(f"[DEBUG] Parent directory {parent_dir} does not exist, creating...")
            os.makedirs(parent_dir, exist_ok=True)

        file_exists = os.path.exists(filepath)

        # If file doesn't exist, create it and add the header
        if not file_exists:
            with open(filepath, 'w') as f:
                f.write("Time, Sample Temperature, Coil Temperature, AIRQ Temperature, AIRQ Humidity, AIRQ Pressure\n")

        current_time = datetime.now().strftime("%H:%M:%S")

        # --- Sample Heater ---
        if measurement_screen.shared_state.Flag_Sample_Heat_chkbox == 1:
            sample_val = sample_temp
        else:
            sample_val = "Not Enabled"

        # --- Coil Heater ---
        if measurement_screen.shared_state.Flag_Coil_Heat_chkbox == 1:
            tubing_val = tubing_temp
        else:
            tubing_val = "Not Enabled"
            
        if measurement_screen.shared_state.Flag_S1V_Heat_chkbox == 1:
            vortex_gen_1_temp_val = vortex_gen_1_temp
        else:
            vortex_gen_1_temp_val = "Not Enabled"     
            
        if measurement_screen.shared_state.Flag_S2V_Heat_chkbox == 1:
            vortex_gen_2_temp_val = vortex_gen_2_temp
        else:
            vortex_gen_2_temp_val = "Not Enabled"  
            
        if measurement_screen.shared_state.Flag_N2H_Heat_chkbox == 1:
            n2_gas_temp_val = n2_gas_temp
        else:
            n2_gas_temp_val = "Not Enabled"                               
            

        # --- Air Quality ---
        if measurement_screen.shared_state.Flag_Air_quality_chkbox == 1:
            air_temp_val = air_temp
            air_humidity_val = air_humidity
            air_pressure_val = air_pressure
        else:
            air_temp_val = "Not Enabled"
            air_humidity_val = "Not Enabled"
            air_pressure_val = "Not Enabled"

        # Create a single line entry (CSV style)
        log_entry = f"{current_time}, {sample_val}, {tubing_val},{vortex_gen_1_temp_val}, {vortex_gen_2_temp_val}, {n2_gas_temp_val}, {air_temp_val}, {air_humidity_val}, {air_pressure_val}\n"
        print(f"[DEBUG] Writing to log: {log_entry.strip()}")
        # Append the data to file
        with open(filepath, 'a') as f:
            f.write(log_entry)

    except Exception as e:
        print(f"[ERROR] Failed to write temperature data to {filepath}: {e}")
        raise


def set_temperature_monitor_active(val1: bool): 
    global temperature_monitor_active
    with _shared_state_lock:
        temperature_monitor_active = val1

def get_temperature_monitor_active():
    global temperature_monitor_active
    with _shared_state_lock:
        return temperature_monitor_active

def set_bricklet_initialization_completed(val1: bool): 
    global bricklet_initialization_completed
    with _shared_state_lock:
        bricklet_initialization_completed = val1

def get_bricklet_initialization_completed(): 
    global bricklet_initialization_completed
    with _shared_state_lock:
        return bricklet_initialization_completed

def set_Appclose(val1: bool):
    global Appclose
    with _shared_state_lock:
        Appclose = val1

def get_Appclose():
    global Appclose
    with _shared_state_lock:
        return Appclose

def set_Sweep_Measurement(val1: bool):
    global Sweep_Measurement
    with _shared_state_lock:
        Sweep_Measurement = val1

def get_Sweep_Measurement():
    global Sweep_Measurement
    with _shared_state_lock:
        return Sweep_Measurement


def set_Normal_Measurement(val1: bool):
    global Normal_Measurement
    with _shared_state_lock:
        Normal_Measurement = val1

def get_Normal_Measurement():
    global Normal_Measurement
    with _shared_state_lock:
        return Normal_Measurement


def set_Calibrate_Measurement(val1: bool):
    global Calibrate_Measurement
    with _shared_state_lock:
        Calibrate_Measurement = val1

def get_Calibrate_Measurement():
    global Calibrate_Measurement
    with _shared_state_lock:
        return Calibrate_Measurement


def set_mainmeasurement(val1: bool):
    global mainmeasurement
    with _shared_state_lock:
        mainmeasurement = val1

def get_mainmeasurement():
    global mainmeasurement
    with _shared_state_lock:
        return mainmeasurement

def set_sampling_time(val1: int):
    global sampling_time
    with _shared_state_lock:
        sampling_time = val1	

def get_sampling_time():
    global sampling_time    
    with _shared_state_lock:
        return sampling_time

def set_Count(val1: int):
    global Count
    with _shared_state_lock:
        Count = val1	

def get_Count():
    global Count    
    with _shared_state_lock:
        return Count    
     
def set_Argon_flow_rate(val1: int):
    global Argon_flow_rate
    with _shared_state_lock:
        Argon_flow_rate = val1
        print(f"FV Argon flow rate set to: {Argon_flow_rate}")

def get_Argon_flow_rate():
    global Argon_flow_rate
    with _shared_state_lock:
        print(f"FV Getting Argon flow rate: {Argon_flow_rate}")        
        return Argon_flow_rate


def set_VOC_argon_flow_rate(val1: int):
    global VOC_argon_flow_rate
    with _shared_state_lock:
        VOC_argon_flow_rate = val1

def get_VOC_argon_flow_rate():
    global VOC_argon_flow_rate
    with _shared_state_lock:
        return VOC_argon_flow_rate

def set_heater_temperature(val1: float):
    global heater_temperature
    with _shared_state_lock:
        heater_temperature = val1

def get_heater_temperature():
    global heater_temperature
    with _shared_state_lock:
        return heater_temperature
    
def set_tubing_coil_temp(val1: float):
    global tubing_coil_temp
    with _shared_state_lock:
        tubing_coil_temp = val1

def get_tubing_coil_temp():
    global tubing_coil_temp
    with _shared_state_lock:
        return tubing_coil_temp   
    
def set_S1V_temp(val1: float):
    global S1V_temp
    with _shared_state_lock:
        S1V_temp = val1

def get_S1V_temp():
    global S1V_temp
    with _shared_state_lock:
        return S1V_temp    
    
def set_S2V_temp(val1: float):
    global S2V_temp
    with _shared_state_lock:
        S2V_temp = val1

def get_S2V_temp():
    global S2V_temp
    with _shared_state_lock:
        return S2V_temp     
    
def set_N2H_temp(val1: float):
    global N2H_temp
    with _shared_state_lock:
        N2H_temp = val1

def get_N2H_temp():
    global N2H_temp
    with _shared_state_lock:
        return N2H_temp     

def set_cleanse_delay(val1: float):
    global cleanse_delay
    with _shared_state_lock:
        cleanse_delay = val1

def get_cleanse_delay():
    global cleanse_delay
    with _shared_state_lock:
        return cleanse_delay        
     
    
def set_Start_Bricketlet_T0_flow(val1: bool):
    global Start_Bricketlet_T0_flow
    with _shared_state_lock:
        Start_Bricketlet_T0_flow = val1

def get_Start_Bricketlet_T0_flow():
    global Start_Bricketlet_T0_flow
    with _shared_state_lock:
        return Start_Bricketlet_T0_flow


def set_view_graph(val1: bool):
    global view_graph
    with _shared_state_lock:
        view_graph = val1

def get_view_graph():
    global view_graph
    with _shared_state_lock:
        return view_graph

def set_view_graph1(val1: bool):
    global view_graph1
    with _shared_state_lock:
        view_graph1 = val1

def get_view_graph1():
    global view_graph1
    with _shared_state_lock:
        return view_graph1

def set_measurement1Completed(val1: bool):
    global measurement1Completed
    with _shared_state_lock:
        measurement1Completed = val1

def get_measurement1Completed():
    global measurement1Completed
    with _shared_state_lock:
        return measurement1Completed

def set_enable_bricklet(val1: bool):
    global enable_bricklet
    with _shared_state_lock:
        enable_bricklet = val1

def get_enable_bricklet():
    global enable_bricklet
    with _shared_state_lock:
        return enable_bricklet

def set_current_argon_flow(val1: float):
    global current_argon_flow
    with _shared_state_lock:
        current_argon_flow = val1

def get_current_argon_flow():
    global current_argon_flow
    with _shared_state_lock:
        return current_argon_flow

def set_stopmeasurement1(val1: bool):
    global stopmeasurement1
    with _shared_state_lock:
        stopmeasurement1 = val1
		
def get_stopmeasurement1():
    global stopmeasurement1
    with _shared_state_lock:
        return stopmeasurement1

def set_exit_measurement_thread1(val1: bool):
    global exit_measurement_thread1
    with _shared_state_lock:
        exit_measurement_thread1 = val1
		
def get_exit_measurement_thread1():
    global exit_measurement_thread1
    with _shared_state_lock:
        return exit_measurement_thread1

def set_transfer_rawtext_file(val1: bool):
    global transfer_rawtext_file
    with _shared_state_lock:
        transfer_rawtext_file = val1
		
def get_transfer_rawtext_file():
    global transfer_rawtext_file
    with _shared_state_lock:
        return transfer_rawtext_file

def set_stop_measurement_data_acq(val1: bool):
    global stop_measurement_data_acq
    with _shared_state_lock:
        stop_measurement_data_acq = val1
		
def get_stop_measurement_data_acq():
    global stop_measurement_data_acq
    with _shared_state_lock:
        return stop_measurement_data_acq

def set_t0(val1: int):
    global t0
    with _shared_state_lock:
        t0 = val1

def get_t0():
    with _shared_state_lock:
        return t0

def set_t1(val1: int):
    global t1
    with _shared_state_lock:
        t1 = val1

def get_t1():
    with _shared_state_lock:
        return t1


def set_t2(val1: int):
    global t2
    with _shared_state_lock:
        t2 = val1

def get_t2():
    print("get_t2")
    global t2
    with _shared_state_lock:
        return t2


def set_t3(val1: int):
    global t3
    with _shared_state_lock:
        t3 = val1

def get_t3():
    with _shared_state_lock:
        return t3

def set_sample_temperature(val1: float):
    global sample_temperature
    with _shared_state_lock:
        sample_temperature = val1
	
def get_sample_temperature():
	global sample_temperature
	with _shared_state_lock:
		return sample_temperature

def set_tubing_temperature(val1: float):
	global tubing_temperature
	with _shared_state_lock:
		tubing_temperature = val1
	
def get_tubing_temperature():
	global tubing_temperature
	with _shared_state_lock:
		return tubing_temperature

def set_air_quality_humidity(val1: float):  
    global air_quality_humidity
    with _shared_state_lock:
        air_quality_humidity = val1 

def get_air_quality_humidity(): 
    global air_quality_humidity
    with _shared_state_lock:
        return air_quality_humidity

def set_air_quality_pressure(val1: float):
    global air_quality_pressure
    with _shared_state_lock:
        air_quality_pressure = val1

def get_air_quality_pressure():
    global air_quality_pressure
    with _shared_state_lock:
        return air_quality_pressure

def set_vortex_gen_1_temperature(val1: float):
    global vortex_gen_1_temperature
    with _shared_state_lock:
        vortex_gen_1_temperature = val1

def get_vortex_gen_1_temperature():
    global vortex_gen_1_temperature
    with _shared_state_lock:
        return vortex_gen_1_temperature
 
def set_vortex_gen_2_temperature(val1: float):
    global vortex_gen_2_temperature
    with _shared_state_lock:
        vortex_gen_2_temperature = val1

def get_vortex_gen_2_temperature():
    global vortex_gen_2_temperature
    with _shared_state_lock:
        return vortex_gen_2_temperature

def set_n2_gas_temperature(val1: float):
    global n2_gas_temperature
    with _shared_state_lock:
        n2_gas_temperature = val1

def get_n2_gas_temperature():
    global n2_gas_temperature
    with _shared_state_lock:
        return n2_gas_temperature

def set_air_quality_temperature(val1: float):
    global air_quality_temperature
    with _shared_state_lock:
        air_quality_temperature = val1
        
def get_air_quality_temperature():
    global air_quality_temperature
    with _shared_state_lock:
        return air_quality_temperature

# def set_mfc_zero(mfc_sfc6, serial_port):
#     mfc_sfc6.setpoint_percentage(serial_port, 0)
#     print(f"{serial_port} Flow Rate Set to SCCM 0")
#     #print(f'flow = {mfc_sfc6.read_average_value(serial_port, 5)}')
    
def MFC_open_nitrogen_flow(mfc_sfc6,serial_port, vserial_port, Nitrogen_flow_rate, measurement_screen = None):

    def emit_status(status):
        if measurement_screen:
            measurement_screen.bricklet_status_signal.emit(status)

    #print(f'Value to be Set Nr-MFC <Not VOC>  <{serial_port}> flow : {Nitrogen_flow_rate}')
    #emit_status(f'Value to be Set Nr-MFC <Not VOC>  <{serial_port}> flow : {Nitrogen_flow_rate}')

    if get_enable_bricklet():
        #print(f'Setting Nitrogen flow Before setting Nr-MFC  <{serial_port}> flow : {mfc_sfc6.read_average_value(serial_port, 5)}')
        #print(f'Setting Nitrogen Flow Before setting VOC-MFC <{vserial_port}> flow : {mfc_sfc6.read_average_value(vserial_port, 5)}')

        #emit_status(f'Setting Nitrogen Flow Before setting Nr-MFC  <{serial_port}> flow : {mfc_sfc6.read_average_value(serial_port, 5)}')
        #emit_status(f'Setting Nitrogen Flow Before setting VOC-MFC <{vserial_port}> flow : {mfc_sfc6.read_average_value(vserial_port, 5)}')
        # Set MFC-28Du & 28CM to Argon flow rate set in measurement table
        #mfc_sfc6.set_calibration(serial_port, NITROGEN_GAS_4)
        if mfc_sfc6.setpoint_percentage(serial_port, Nitrogen_flow_rate) == 0:
            print(f" Failed to set MFC <{serial_port}> to {Nitrogen_flow_rate}%")
            emit_status(f" Failed to set MFC <{serial_port}> to {Nitrogen_flow_rate}%")
            print(f" DisConnecting to the MFC <{serial_port}>")
            emit_status(f" DisConnecting to the MFC <{serial_port}>")
            mfc_sfc6.disconnect(serial_port)
            time.sleep(1)
            print(f" Connecting to the MFC <{serial_port}>")
            emit_status(f" Connecting the MFC <{serial_port}>")
            ret = mfc_sfc6.connect(serial_port, baudrate=115200,additional_response_time=2.0, slave_address=0)
            if ret == "Connect failed" :
                print(f" Connecting Faile to the MFC <{serial_port}>")
                emit_status(f" Connecting Failed the MFC <{serial_port}>")
                return False

        #time.sleep(1)
        #print(f'Setting Nitrogen Flow After setting Nr-MFC  <{serial_port}> flow : {mfc_sfc6.read_average_value(serial_port, 5)}')
        #print(f'Setting Nitrogen Flow After setting VOC-MFC <{vserial_port}> flow : {mfc_sfc6.read_average_value(vserial_port, 5)}')
        #emit_status(f'Setting Nitrogen Flow After setting Nr-MFC  <{serial_port}> flow : {mfc_sfc6.read_average_value(serial_port, 5)}')
        #emit_status(f'Setting Nitrogen Flow After setting VOC-MFC <{vserial_port}> flow : {mfc_sfc6.read_average_value(vserial_port, 5)}')

def MFC_open_VOC_flow(mfc_sfc6,serial_port,vserial_port, VOC_flow_rate,Argon_flow_rate, measurement_screen = None):
    global enable_print
    def emit_status(status):
        if measurement_screen:
            measurement_screen.bricklet_status_signal.emit(status)

    Nitrogen_mix_percentage = 0.01 * VOC_flow_rate
    current_argon_val = 0.5 * (Argon_flow_rate/100)
    Nitrogen_mix = current_argon_val * Nitrogen_mix_percentage
    VOC_mix= current_argon_val - Nitrogen_mix
    VOC_mix_per = int((VOC_mix * 100) / 0.5)
    VOC_flow_rate = VOC_mix_per	
	
    print(f'Value to be Set VOC-MFC <Not Nr>  <{vserial_port}> flow : {VOC_flow_rate}')
    emit_status(f'Value to be Set VOC-MFC <Not Nr>  <{vserial_port}> flow : {VOC_flow_rate}')

    if get_enable_bricklet():
        """commented by R
        if enable_print:
            print(f'Setting VOC Flow Before setting Nr-MFC  <{serial_port}> flow : {mfc_sfc6.read_average_value(serial_port, 5)}')
            print(f'Setting VOC Flow Before setting VOC-MFC <{vserial_port}> flow : {mfc_sfc6.read_average_value(vserial_port, 5)}')
            emit_status(f'Setting VOC Flow Before setting Nr-MFC  <{serial_port}> flow : {mfc_sfc6.read_average_value(serial_port, 5)}')
            emit_status(f'Setting VOC Flow Before setting VOC-MFC <{vserial_port}> flow : {mfc_sfc6.read_average_value(vserial_port, 5)}')
        """
        # Set MFC-28Do & 28Dw to Argon flow rate set in measurement table
        #mfc_sfc6.set_calibration(vserial_port, NITROGEN_GAS_4)
        if mfc_sfc6.setpoint_percentage(vserial_port, VOC_flow_rate) == 0:
            print(f" Failed to set MFC <{vserial_port}> to {VOC_flow_rate}%")
            emit_status(f" Failed to set MFC <{vserial_port}> to {VOC_flow_rate}%")
            print(f" Disconnecting to  MFC <{vserial_port}>")
            emit_status(f" Disconnecting to  MFC <{vserial_port}> ")
            mfc_sfc6.disconnect(vserial_port)
            time.sleep(1)
            print(f" Connecting to the MFC <{vserial_port}>")
            ret = mfc_sfc6.connect(vserial_port, baudrate=115200,additional_response_time=2.0, slave_address=0)
            if ret == "Connect failed" :
                print(f" Connecting Failed to the MFC <{vserial_port}>")
                emit_status(f" Connecting Failed to the MFC <{vserial_port}>")
                return False 
        #time.sleep(1)
        """commented by R
        if enable_print:
            print(f'Setting VOC Flow After setting Nr-MFC  <{serial_port}> flow : {mfc_sfc6.read_average_value(serial_port, 5)}')
            print(f'Setting VOC Flow After setting VOC-MFC <{vserial_port}> flow : {mfc_sfc6.read_average_value(vserial_port, 5)}')
            emit_status(f'Setting VOC Flow After setting Nr-MFC  <{serial_port}> flow : {mfc_sfc6.read_average_value(serial_port, 5)}')
            emit_status(f'Setting VOC Flow After setting VOC-MFC <{vserial_port}> flow : {mfc_sfc6.read_average_value(vserial_port, 5)}')
        """
def MFC_VOC_argon_flow_mvp(mfc_sfc6,serial_port,vserial_port, VOC_flow_rate,Nitrogen_flow_rate, measurement_screen = None):
    def emit_status(status):
        if measurement_screen:
            measurement_screen.bricklet_status_signal.emit(status)
        
    #print('Setting Mixing Constant ***************')
    #emit_status('Setting Mixing Constant ***************')
    #Mixing VOC and Argon based on Mixing constant from table
    Nitrogen_mix_percentage = 0.01 * VOC_flow_rate

    #if enable_bricklet:
    #    print(f'Before setting Nr-MFC  <{serial_port}> flow : {mfc_sfc6.read_average_value(serial_port, 5)}')
    #    print(f'Before setting VOC-MFC <{vserial_port}> flow : {mfc_sfc6.read_average_value(vserial_port, 5)}')
    #    emit_status(f'Before setting Nr-MFC  <{serial_port}> flow : {mfc_sfc6.read_average_value(serial_port, 5)}')
    #    emit_status(f'Before setting VOC-MFC <{vserial_port}> flow : {mfc_sfc6.read_average_value(vserial_port, 5)}')
        
    #current_argon_val = mfc_sfc6.read_average_value(serial_port,5)
    current_argon_val = 0.5 * (Nitrogen_flow_rate/100)
    
    #print(f'Current_argon_val  = {current_argon_val}')

    Nitrogen_mix = current_argon_val * Nitrogen_mix_percentage
    #print(f'Argon_mix  = {Argon_mix} Nitrogen_mix_percentage {Nitrogen_mix_percentage}')

    Nitrogen_mix_per = int((Nitrogen_mix * 100) / 0.5)
    #print(f'Nr-FlowRate = {Nitrogen_flow_rate}  Nitrogen_mix  = {Nitrogen_mix} Nitrogen_mix_per {Nitrogen_mix_per}')
    VOC_mix= current_argon_val - Nitrogen_mix
    #print(f'VOC_mix  = {VOC_mix} current_argon_val {current_argon_val}')
    VOC_mix_per = int((VOC_mix * 100) / 0.5)
    #print(f'Mixing Constant = {VOC_flow_rate} VOC_mix  = {VOC_mix} VOC_mix_per {VOC_mix_per}')
    
    #emit_status(f'Nitrogen-Mix <{serial_port}> flow : {Nitrogen_mix_per}')
    #emit_status(f'VOC-Mix <{serial_port}> flow : {VOC_mix_per}')

    if get_enable_bricklet():
        #mfc_sfc6.setpoint_percentage(serial_port, Nitrogen_mix_per)
        if mfc_sfc6.setpoint_percentage(serial_port, Nitrogen_mix_per) == 0:
            print(f" Failed to set MFC <{serial_port}> to {Nitrogen_mix_per}%")
            emit_status(f" Failed to set MFC <{serial_port}> to {Nitrogen_mix_per}%")
            print(f" Disconnecting to  MFC <{serial_port}>")
            emit_status(f" Disconnecting to  MFC <{serial_port}> ")
            mfc_sfc6.disconnect(serial_port)
            time.sleep(1)
            print(f" Connecting to the MFC <{serial_port}>")
            ret = mfc_sfc6.connect(serial_port, baudrate=115200,additional_response_time=2.0, slave_address=0)
            if ret == "Connect failed" :
                print(f" Connecting Failed to the MFC <{serial_port}>")
                emit_status(f" Connecting Failed to the MFC <{serial_port}>")
                
        #mfc_sfc6.setpoint_percentage(vserial_port, VOC_mix_per)
        if mfc_sfc6.setpoint_percentage(vserial_port, VOC_mix_per) == 0:
            print(f" Failed to set MFC <{vserial_port}> to {VOC_mix_per}%")
            emit_status(f" Failed to set MFC <{vserial_port}> to {VOC_mix_per}%")
            print(f" Disconnecting to  MFC <{vserial_port}>")
            emit_status(f" Disconnecting to  MFC <{vserial_port}> ")
            mfc_sfc6.disconnect(vserial_port)
            time.sleep(1)
            print(f" Connecting to the MFC <{vserial_port}>")
            ret = mfc_sfc6.connect(vserial_port, baudrate=115200,additional_response_time=2.0, slave_address=0)
            if ret == "Connect failed" :
                print(f" Connecting Failed to the MFC <{serial_port}>")
                emit_status(f" Connecting Failed to the MFC <{serial_port}>") 

        #print(f'After setting Nr-MFC  <{serial_port}> flow : {mfc_sfc6.read_average_value(serial_port, 5)}')
        #print(f'After setting VOC-MFC <{vserial_port}> flow : {mfc_sfc6.read_average_value(vserial_port, 5)}')
        #emit_status(f'After setting Nr-MFC  <{serial_port}> flow : {mfc_sfc6.read_average_value(serial_port, 5)}')
        #emit_status(f'After setting VOC-MFC <{vserial_port}> flow : {mfc_sfc6.read_average_value(vserial_port, 5)}')

def set_mfc_zero(mfc_sfc6, serial_port):
    if not mfc_sfc6.setpoint_percentage(serial_port, 0):
        print(f" Failed to set MFC <{serial_port}> to 0%")
        print(f" DisConnecting to the MFC <{serial_port}>")
        mfc_sfc6.disconnect(serial_port)
        time.sleep(1)
        ret = mfc_sfc6.connect(serial_port, baudrate=115200,additional_response_time=2.0, slave_address=0)
        if ret == "Connect failed" :
            print(f" Connecting Failed to the MFC <{serial_port}>")
            return False        
    print(f"{serial_port} Flow Rate Set to SCCM 0")
    return True
    #print(f'flow = {mfc_sfc6.read_average_value(serial_port, 5)}')

def set_initial_mfc_zero(mfc_sfc6, serial_port):
    if not mfc_sfc6.setpoint_percentage(serial_port, 0):
        print(f" Failed to set MFC <{serial_port}> to 0%")
        return False        
    print(f"{serial_port} Initial Flow Rate Set to SCCM 0")
    return True

def open_mfc_rate(mfc_sfc6,serial_port,flow_rate):
    #Open MFC11 to 30% for 5 Seconds, then 0%
    # Set MFC to ARGON
    #mfc_sfc6.set_calibration(serial_port, NITROGEN_GAS_4)
    mfc_sfc6.setpoint_percentage(serial_port, flow_rate)
    if mfc_sfc6.setpoint_percentage(serial_port, flow_rate) == 0:
        print(f" Failed to set MFC <{serial_port}> to {flow_rate}%")
        print(f" Disconnecting to  MFC <{serial_port}>")
        mfc_sfc6.disconnect(serial_port)
        time.sleep(1)
        print(f" Connecting to the MFC <{serial_port}>")
        ret = mfc_sfc6.connect(serial_port, baudrate=115200,additional_response_time=2.0, slave_address=0)
        if ret == "Connect failed" :
            print(f" Connecting Failed to the MFC <{serial_port}>")
            return False
    print(f"Flow Rate Set to SCCM ({flow_rate}%)")
    print(f'Raw Flow = {mfc_sfc6.measure_raw_flow(serial_port)}')
    #print(f'flow = {mfc_sfc6.read_average_value(serial_port, 5)}')
    return True

def Exhaust_Solenoid_Valve_on(sv1_relay):
	#Exhaust solonoid ON
	return sv1_turn_on_relay(sv1_relay, 0)
 
	
def Exhaust_Solenoid_Valve_off(sv1_relay):
	#Exhaust solonoid OFF
	return sv1_turn_off_relay(sv1_relay, 0)

def VOC_Solenoid_Valve_on(sv1_relay):    
    ret = True
	#Sensor 1,2 VOC Solenoid Valve ON/OPEN
    if not sv1_turn_on_relay(sv1_relay, 1):
        ret = False
    if not sv1_turn_on_relay(sv1_relay, 2):
        ret = False    
    return ret

def VOC_Solenoid_Valve_off(sv1_relay):    
    ret = True
	#Sensor 1,2 VOC Solenoid Valve OFF/CLOSED
    if not sv1_turn_off_relay(sv1_relay, 1):
        ret = False        
    if not sv1_turn_off_relay(sv1_relay, 2):
        ret = False    
    return ret  

def check_for_sample_temp(controller):
    

    sample_temp = get_sample_temperature() 
    dual_target_temperature = get_heater_temperature()
    
    """
     Call the sample heater algorithm for enabling the heater on / offss
    """
    heater_state = controller.update(
        current_temp=sample_temp,
        required_temp=dual_target_temperature,
    )
    
    #ual_relay = get_dual_relay()
    if heater_state == HEATER_ON:
        dual_turn_on_relay(dual_relay,0)
        print(f"Turning ON dual relay for sample heater  (Current Tempearture :{sample_temp})")
    else:
        dual_turn_off_relay(dual_relay,0)
        print(f"Turning OFF dual relay for sample heater  (Current Tempearture :{sample_temp})")
    return sample_temp

   
def check_for_tubing_temp(controller):
    tubing_temp = get_tubing_temperature() 
    tubing_heater_temperature = get_tubing_coil_temp()

    """
    Call the coil heater algorithm for enabling the heater on / offss
    """
    heater_state = controller.update(
        current_temp=tubing_temp,
        required_temp=tubing_heater_temperature,
    )
    
    #dual_relay = get_dual_relay()
    if heater_state == HEATER_ON:
        dual_turn_on_relay(dual_relay,1)
        print(f"Turning ON dual relay for tubing heater  (Current Tempearture :{tubing_temp})")
    else:
        dual_turn_off_relay(dual_relay,1)
        print(f"Turning OFF dual relay for tubing heater  (Current Tempearture :{tubing_temp})")

    return tubing_temp


def check_for_air_temp():
    air_temp = get_air_quality_temperature()
    return air_temp
	
def check_for_air_humidity():
	air_humidity = get_air_quality_humidity() 
	return air_humidity
	
def check_for_air_pressure():
	air_pressure = get_air_quality_pressure()
	return air_pressure

   
def Meas_timer_counter_thread(measurement_screen=None):
    try:
        while True:
            try:
                event_set = Start_timer_event.wait(timeout=1)
                if event_set:
                    print("Timer Counter Thread Event is Set to True")
                    Start_timer_event.clear()
                    cctime = int(time.time())

                    if get_t2() > 0 and get_enable_bricklet():
                        max_count = measurement_screen.get_meas_max_count() + 8 
                    else:
                        max_count = measurement_screen.get_meas_max_count()
                    
                    
                    while (int(time.time()) <= (cctime + max_count)):
                        if get_Cancel_Measurement():
                            print("User Pressed the Cancel button During the Measurment, exiting Measurement timer counter Thread")
                            set_TimerThread_Closed(True)                  
                            break

                        update_timer = int(time.time() - cctime)
                        # Due to Failed Data Acquisition
                        if not measurement_screen.shared_state.get_continue_measurement():
                            if get_t2() ==  0: 
                                 if update_timer >= (max_count - 8): 
                                     print("Control is disabled")
                                     measurement_screen.control_update_signal.emit("disable_controls")
                            break 
                        #update_timer = int(time.time() - cctime)
                        print(f"Timer Counter = {update_timer}")
                        # Emit signal and log
                        if measurement_screen:
                            measurement_screen.Timer_status_signal.emit(f"{update_timer} Sec")
                            print(f"[DEBUG] Emitted Timer_status_signal with value: {update_timer} Sec")
                        else:
                            print("[ERROR] measurement_screen is None, cannot emit Timer_status_signal")
                        time.sleep(1)

                    measurement_screen.set_meas_max_count(0)

                else:
                    # Reset timer display if event not set
                    if get_Appclose():
                        print("Application closing, exiting Measurement timer counter Thread")
                        set_TimerThread_Closed(True)
                        return  
                    
                if get_Cancel_Measurement():
                    print("User Pressed the Cancel button During the Measurment, exiting Measurement timer counter Thread")
                    set_TimerThread_Closed(True)                  
                                    
            except AttributeError as e:
                print(f"AttributeError in inner loop: {e}")
                time.sleep(1)
    except Exception as e:
        print(f"Unexpected error in Meas_timer_counter_thread: {e}")
        time.sleep(1)
	       
def close_Temperature():
    dual_relay = get_dual_relay() 
    dual_turn_off_relay(dual_relay, 0)
    dual_turn_off_relay(dual_relay, 1)

def Temperature_Monitor_Worker_thread(measurement_screen=None):

    sample_temp_value = 0.0
    air_temp_value = 0.0
    air_humidity_value = 0.0
    air_pressure_value = 0.0
    tubing_temp_value = 0.0
    vortex_gen1_temp_value = 0.0
    vortex_gen2_temp_value = 0.0
    n2_gas_temp_value = 0.0
    last_temperature_log_time = 0
    # relay_sample_flag = 0
    # relay_tubing_flag = 0
    log_interval = 1  # Log every 1 second
    relay_turned_off = False
    air_temp_rounded = 0.0
    air_humidity_rounded = 0.0
    air_pressure_rounded = 0.0

    """
        Initialize the Heater Algorithm 
    """

    Sample_heater_controller = HeaterController(
        name="SAMPLE_HEATER",
        history_length=5,
        k_predict_sec=90.0,
        hysteresis=1.0,
        min_early_margin=0.3,
        max_early_cut=5.0,
    )

    Coil_heater_controller = HeaterController(
        name="COIL_HEATER",
        history_length=5,
        k_predict_sec=55.0,
        hysteresis=0.8,
        min_early_margin=0.2,
        max_early_cut=3.0,
    )


    while True:
        #tnow = datetime.now()
        #print(f"<TT> {tnow.strftime('%Y-%m-%d %H:%M:%S')} : Temperature Monitor Thread Running...")

        try:
            # If bricklet is disable, do not access the bricklet functions
            if (not get_enable_bricklet()) or (not get_bricklet_initialization_completed()):
                #print(f"<TT> Temperature : Bricklet Disabled {get_bricklet_initialization_completed()}")
                if get_Appclose():
                    print("<TT> Temperature : Appication closed")
                    set_TemperaturThread_Closed(True)
                    return

                if get_Cancel_Measurement():
                    print("<TT> User Pressed the Cancel button During the Measurment, exiting Temperature Monitor Thread ")
                    set_TemperaturThread_Closed(True)

                time.sleep(1)
                continue

            current_time = time.time()
            if not get_temperature_monitor_active():
                print("<TT> get temperature monitor is false...")
                if not relay_turned_off:
                    print("iturn off the relay...")
                    close_Temperature()
                    relay_turned_off = True
                time.sleep(0.5)
                continue

            relay_turned_off = False
            if get_Appclose():
                print("<TT> Application closing, exiting Temperature Monitor Thread")
                close_Temperature()
                set_TemperaturThread_Closed(True)
                return

            if get_Cancel_Measurement():
                print("<TT> User Pressed the Cancel button During the Measurment, exiting Temperature Monitor Thread ")
                set_TemperaturThread_Closed(True)

            #if get_enable_bricklet() and measurement_screen.shared_state.connection_status.get("Device 1", True):
            #print("Current Screen : ", measurement_screen.shared_state.get_current_screen())
            if get_enable_bricklet() and measurement_screen.shared_state.get_device_connection_state():
                if measurement_screen.shared_state.get_Flag_Sample_Heat_chkbox():
                    sample_temp_value = check_for_sample_temp(Sample_heater_controller)
                    if measurement_screen and measurement_screen.shared_state.get_current_screen() == "Settings":
                        measurement_screen.sample_temperature_update_signal.emit(sample_temp_value)
                    #relay_sample_flag = 1
                else:
                    #if relay_sample_flag == 1:
                    dual_relay = get_dual_relay() 
                    dual_turn_off_relay(dual_relay, 0)
                    #relay_sample_flag = 0

                if measurement_screen.shared_state.get_Flag_Coil_Heat_chkbox():
                    tubing_temp_value = check_for_tubing_temp(Coil_heater_controller)
                    #print("Tubing Temperature : ", tubing_temp_value, measurement_screen.shared_state.get_Flag_Coil_Heat_chkbox())
                    if measurement_screen and measurement_screen.shared_state.get_current_screen() == "Settings":
                        measurement_screen.coil_temperature_update_signal.emit(tubing_temp_value)
                    #relay_tubing_flag = 1
                else:
                    
                    #if relay_tubing_flag == 1:
                    dual_relay = get_dual_relay() 
                    dual_turn_off_relay(dual_relay, 1)
                    #relay_tubing_flag = 0

                if measurement_screen.shared_state.get_Flag_Air_quality_chkbox()  and measurement_screen.shared_state.get_current_screen() == "Settings":
                    # Air Temperature
                    air_temp_value = check_for_air_temp()
                    air_temp_rounded = round(air_temp_value, 3)
                    #air_temp_str = f"{air_temp_rounded:.3f}"
                    if measurement_screen:
                        measurement_screen.aircube_temperature_update_signal.emit(air_temp_rounded)


                    # Air Humidity
                    air_humidity_value = check_for_air_humidity()
                    air_humidity_rounded = round(air_humidity_value, 3)
                    #air_humidity_str = f"{air_humidity_rounded:.3f}"
                    if measurement_screen:
                        measurement_screen.aircube_humidity_update_signal.emit(air_humidity_rounded)


                    # Air Pressure
                    air_pressure_value = check_for_air_pressure()
                    air_pressure_rounded = round(air_pressure_value, 3)
                    #air_pressure_str = f"{air_pressure_rounded:.3f}"
                    if measurement_screen:
                        measurement_screen.aircube_pressure_update_signal.emit(air_pressure_rounded)


                set_sample_temperature(sample_temp_value)
                set_tubing_temperature(tubing_temp_value)
                set_vortex_gen_1_temperature(vortex_gen1_temp_value)
                set_vortex_gen_2_temperature(vortex_gen2_temp_value)
                set_n2_gas_temperature(n2_gas_temp_value)
                set_air_quality_temperature(air_temp_rounded)
                set_air_quality_humidity(air_humidity_rounded)
                set_air_quality_pressure(air_pressure_rounded)


                # Log temperature data
                if get_temperature_log_active() and (current_time - last_temperature_log_time) > log_interval:
                    log_temperature_data(
                        measurement_screen,
                        get_temperature_log_filepath(),
                        get_sample_temperature(),
                        get_tubing_temperature(),
                        get_vortex_gen_1_temperature(),
                        get_vortex_gen_2_temperature(),
                        get_n2_gas_temperature(),
                        get_air_quality_temperature(),
                        get_air_quality_humidity(),
                        get_air_quality_pressure()
                    )
                    last_temperature_log_time = time.time()
        except AttributeError as e:
            print(f"AttributeError in inner loop: {e}")
            break  # Break on AttributeError to prevent infinite loop on invalid access

        time.sleep(0.5)


def wait_for_required_temp_to_set(measurement_screen = None):

    def emit_status(status):
        if measurement_screen:
            measurement_screen.bricklet_status_signal.emit(status)

    is_sample_temperature = False
    sample_temp = 0.0
    tubing_temp = 0.0

    """
    Wait until the Sample and Tubing Temp is set
    as per the setting table.
    """
    dual_target_temperature = get_heater_temperature()
    tubing_heater_temperature = get_tubing_coil_temp()

    tnow = datetime.now()
    rc = True
    while True:
        # user pressed the cancel button.
        if get_Cancel_Measurement():
            return False

        if datetime.now() > tnow + timedelta(seconds=10):
            print("<BT> Temperature not Reached == Timeout")
            rc = False
            break
        """
        Sample Temp is increasing fastly. So made it as -1
        """
        sample_temp = get_sample_temperature() 
        sample_temp = sample_temp + 0.5
        """
        TUBING Temp is increasing Slowly. So made it as +1
        """
        tubing_temp = get_tubing_temperature() 
        tubing_temp = tubing_temp + 0.5
        """
        Check the Sample and Tubing temp teaches the configured value
        """
        print("get_heater_calibration() : ", get_heater_calibration())
        # if sample_temp >= dual_target_temperature and tubing_temp >= tubing_heater_temperature:
        if (sample_temp >= (dual_target_temperature - 2) and not get_heater_calibration()):
            is_sample_temperature = True
        
        elif (sample_temp >= (dual_target_temperature - 2 )):
            is_sample_temperature = True

        if is_sample_temperature and tubing_temp >= (tubing_heater_temperature - 2 ):
            print("<BT> Temperature Reached == Breaking")
            rc = True
            break

        print(f"Required Sample Temperature is {dual_target_temperature} : {sample_temp} ")
        emit_status(f"Required Sample Temperature is {dual_target_temperature}: {sample_temp} ")
        print(f"Required Tubing Temperature is {tubing_heater_temperature} : {tubing_temp}")
        emit_status(f"Required Tubing Temperature is {tubing_heater_temperature} : {tubing_temp}")

        time.sleep(0.5)

    if rc:
        print(f"Required Sample Temperature Reached {dual_target_temperature} : {sample_temp} ")
        emit_status(f"Required Sample Temperature  Reached {dual_target_temperature}: {sample_temp} ")
        print(f"Required Tubing Temperature  Reached {tubing_heater_temperature} : {tubing_temp}")
        emit_status(f"Required Tubing Temperature  Reached {tubing_heater_temperature} : {tubing_temp}")
    return rc
#////////////////////////////////********************************///////////////////////////////////
def start_nitrogen_flow(measurement_screen = None):
    global  mfc_sfc6, serial_ports, sampling_time, mainmeasurement, enable_print
    sv1_relay = None

    def get_current_time():
        timestamp = time.time()
        readable_time = datetime.fromtimestamp(timestamp)
        only_time = readable_time.strftime("%H:%M:%S")
        return only_time

    def emit_status(status):
        if measurement_screen:
            measurement_screen.bricklet_status_signal.emit(status)
        #print(status)

    #serial ports for MFCs 1s & 2s
    voc_serial_ports1 = [mfc2_serial_port, mfc4_serial_port]
    argon_serial_ports2 = [mfc1_serial_port, mfc3_serial_port]

    # T0 START Measurement Process:
    # Set MFC-12,22,32,42  to Argon flow rate set in measurement table

    main_flag =get_mainmeasurement()
    print(f"<BT> main_measurement_flag = {main_flag}")

    if get_mainmeasurement():
        print("<BT> Printing T1 Starts Bricklet Status")
        emit_status("<BT> Printing T1 Starts Bricklet Status")

    if enable_print:
        print_bricklet_device_status(measurement_screen)

    #Exhast Open
    if get_enable_bricklet():
        sv1_relay = get_sv1_relay()
        if not Exhaust_Solenoid_Valve_on(sv1_relay) :
            print("<BT> Failed to set Exhaust Solenoid Valve to ON/Open 2f3R SW0")
            emit_status("<BT> Failed to set Exhaust Solenoid Valve to ON/Open 2f3R SW0 <Bricklet Failure>")
            measurement_screen.shared_state.set_failed_status("<BT> Failed to set Exhaust Solenoid Valve to ON/Open 2f3R SW0")
            measurement_screen.shared_state.set_measurement_failed(True) 
            return False


    # Max measure count is required only the Main measurement
    if get_mainmeasurement():
        if get_enable_bricklet():
            print("nitrogen only")
            measurement_screen.set_meas_max_count(get_t1() + get_t2() + get_t3() + 8)
        else:
            measurement_screen.set_meas_max_count(get_t1() + get_t2() + get_t3())


    print("Data Acq started Bricklet started for the MFC")
    print(f"<BL Timing : {get_current_time()}> T0 time started")
    emit_status(f"<Timing : {get_current_time()}> T0 time started ")
    print(f"dwell : {measurement_screen.shared_state.cube.d_time},max : {measurement_screen.shared_state.var_vg_max},min : {measurement_screen.shared_state.var_vg_min},g_step : {measurement_screen.shared_state.var_g_step} ")


    # T1 :
    # Set Sensor 1,2 Nitrogen MFC & Sensor 1,2 VOC MFC Open
    if get_enable_bricklet():
        for serial_port, vserial_port in zip(argon_serial_ports2, voc_serial_ports1):
            MFC_open_nitrogen_flow(mfc_sfc6, serial_port, vserial_port, get_Argon_flow_rate(), measurement_screen)            

    set_current_argon_flow(get_Argon_flow_rate())
    current_nitrogen_flow = get_current_argon_flow()
    prev_nitrogen_flow = current_nitrogen_flow

    # Max measure count is required only the Main measurement
    if get_mainmeasurement():
        Start_timer_event.set()
        measurement_screen.control_update_signal.emit("enable_controls")
        print("<BT> Timer Enterd")

    print(f"<BT> Counter started at {get_current_time()} ")
    #now starting data acquisition
    Start_data_acquisition_event.set()

    # Max measure count is required only the Main measurement
    if get_mainmeasurement():
        print(f"<BT> S Timing : {get_current_time()}> T1 time started")
        emit_status(f"<BT> S Timing : {get_current_time()}> T1 time started ")
    else:
        print(f"<BT> S Timing : {get_current_time()}> Only one timer wait for the Adv Sweep and Adv Measure")

    if get_mainmeasurement() and get_enable_bricklet():
        for serial_port, vserial_port in zip(argon_serial_ports2, voc_serial_ports1):
            val1 = mfc_sfc6.read_average_value(serial_port, 5)
            val2 = mfc_sfc6.read_average_value(vserial_port, 5)
            
            if val1 is None:
                print(f'<BT> unable to set MFC Flow{serial_port}')
                emit_status(f'<BT> unable to set MFC Flow{serial_port}')
                measurement_screen.shared_state.set_failed_status(f"<BT> unable to set MFC Flow{serial_port} <Bricklet Failure>")
                measurement_screen.shared_state.set_measurement_failed(True)   
                return False

            if val2 is None:
                print(f'<BT> unable to set MFC Flow{serial_port}')
                emit_status(f'<BT> unable to set MFC Flow{serial_port}')
                measurement_screen.shared_state.set_failed_status(f"<BT> unable to set MFC Flow{serial_port} <Bricklet Failure>")
                measurement_screen.shared_state.set_measurement_failed(True)   
                return False

            print(f'<BT> T1 Nr-MFC  <{serial_port}> flow : {val1}')
            print(f'<BT> T1 VOC-MFC <{vserial_port}> flow : {val2}')
            emit_status(f'<BT> T1 Nr-MFC  <{serial_port}> flow : {val1}')
            emit_status(f'<BT> T1 VOC-MFC <{vserial_port}> flow : {val2}')

    if get_mainmeasurement():
        sleep_time = get_t1()
        print(f"current Nitrogen value from table is {current_nitrogen_flow}")
        emit_status("now change Nitrogen flow value ")


        tnow = datetime.now()
        while True:
            current_nitrogen_flow = get_current_argon_flow()
            print(f"current Nitrogen flow is : {current_nitrogen_flow}")
            print(f"previous Nitrogen flow is : {prev_nitrogen_flow}")
            # Change Nitrogen flow during runtime, if t2 is zero

            if current_nitrogen_flow != prev_nitrogen_flow :
                print(f"current Nitrogen flow is : {current_nitrogen_flow}")
                emit_status(f"current Nitrogen flow is : {current_nitrogen_flow}")
                for serial_port in argon_serial_ports2:
                    if get_enable_bricklet():
                        MFC_open_nitrogen_flow(mfc_sfc6, serial_port, vserial_port, current_nitrogen_flow, measurement_screen)
                if get_enable_bricklet():
                    for serial_port, vserial_port in zip(argon_serial_ports2, voc_serial_ports1):
                        
                        val1 = mfc_sfc6.read_average_value(serial_port, 5)
                        val2 = mfc_sfc6.read_average_value(vserial_port, 5)
            
                        if val1 is None:
                            print(f'<BT> unable to set MFC Flow{serial_port}')
                            emit_status(f'<BT> unable to set MFC Flow{serial_port}')
                            measurement_screen.shared_state.set_failed_status(f"<BT> unable to set MFC Flow{serial_port} <Bricklet Failure>")
                            measurement_screen.shared_state.set_measurement_failed(True)   
                            return False

                        if val2 is None:
                            print(f'<BT> unable to set MFC Flow{serial_port}')
                            emit_status(f'<BT> unable to set MFC Flow{serial_port}')
                            measurement_screen.shared_state.set_failed_status(f"<BT> unable to set MFC Flow{serial_port} <Bricklet Failure>")
                            measurement_screen.shared_state.set_measurement_failed(True)   
                            return False

                        print(f'<BT> T1 T2=0 Nr-MFC  <{serial_port}> flow : {val1}')
                        print(f'<BT> T1 T2=0 VOC-MFC <{vserial_port}> flow : {val2}')
                        emit_status(f'<BT> T1 T2=0 Nr-MFC  <{serial_port}> flow : {val1}')
                        emit_status(f'<BT> T1 T2=0 VOC-MFC <{vserial_port}> flow : {val2}')

                prev_nitrogen_flow = current_nitrogen_flow

            # user pressed the cancel button.
            if get_Cancel_Measurement():
                print('<BT>  Stopped the Measurement due to user cancel request.')
                emit_status('<BT> Stopped the Measurement due to user cancel request.')
                measurement_screen.control_update_signal.emit("disable_controls")
                return False

            # Due to Failed start data acquisition
            if not measurement_screen.shared_state.get_continue_measurement():
                print('<BT>  Stopped the Measurement ')
                emit_status('<BT> Stopped the Measurement.')
                return False

            #if datetime.now() > tnow + sleep_time:
            if datetime.now() > tnow + timedelta(seconds=sleep_time):
                break

            time.sleep(1)
            #sleep_time = sleep_time - 1
        print(f"<BL E Timing : {get_current_time()} Completed {get_t1()} seconds - t1 phase")
        emit_status(f"BL E <Timing : {get_current_time()} Completed {get_t1()} seconds - t1 phase")
    else:
        sleep_time = round(get_sampling_time() + 1)
        print(f"Adv Setting Sweep/Messure sampling_time_sleep = {sleep_time}")
        tnow = datetime.now()
        while True:
            # user pressed the cancel button.
            if get_Cancel_Measurement():
                print('<BT>  Stopped the Measurement due to user cancel request.')
                emit_status('<BT> Stopped the Measurement due to user cancel request.')
                return False

            # Due to Failed start data acquisition
            if not measurement_screen.shared_state.get_continue_measurement():
                print('<BT>  Stopped the Measurement ')
                emit_status('<BT> Stopped the Measurement.')
                return False

            time.sleep(1)

            #if datetime.now() > tnow + sleep_time:
            if datetime.now() > tnow + timedelta(seconds=sleep_time):
                break


        print(f"<BT> E Timing : {get_current_time()}> Completed the Adv Sweep and Adv Measure")


    set_transfer_rawtext_file(True)
    if get_mainmeasurement() and get_enable_bricklet():
        for serial_port, vserial_port in zip(argon_serial_ports2, voc_serial_ports1):
            val1 = mfc_sfc6.read_average_value(serial_port, 5)
            val2 = mfc_sfc6.read_average_value(vserial_port, 5)
            
            if val1 is None:
                print(f'<BT> unable to set MFC Flow{serial_port}')
                emit_status(f'<BT> unable to set MFC Flow{serial_port}')
                measurement_screen.shared_state.set_failed_status(f"<BT> unable to set MFC Flow{serial_port} <Bricklet Failure>")
                measurement_screen.shared_state.set_measurement_failed(True)   
                return False

            if val2 is None:
                print(f'<BT> unable to set MFC Flow{serial_port}')
                emit_status(f'<BT> unable to set MFC Flow{serial_port}')
                measurement_screen.shared_state.set_failed_status(f"<BT> unable to set MFC Flow{serial_port} <Bricklet Failure>")
                measurement_screen.shared_state.set_measurement_failed(True)   
                return False

            print(f'<BT> End Nr-MFC  <{serial_port}> flow : {val1}')
            print(f'<BT> End VOC-MFC <{vserial_port}> flow : {val2}')
            emit_status(f'<BT> Nr-MFC  <{serial_port}> flow : {val1}')
            emit_status(f'<BT> VOC-MFC <{vserial_port}> flow : {val2}')

    if get_mainmeasurement():
        print(f"<BL S Timing : {get_current_time()} wait for {get_t3()} seconds for completing t3 phase)")
        emit_status(f"<BL S Timing : {get_current_time()} wait for {get_t3()} seconds for completing t3 phase)")
        #time.sleep(get_t3() - 1)
        sleep_time = get_t3()
        tnow = datetime.now()
        while True:
            # Change Nitrogen flow during runtime, if t2 is zero
            current_nitrogen_flow = get_current_argon_flow()
            if current_nitrogen_flow != prev_nitrogen_flow :
                print(f"current Nitrogen flow is : {current_nitrogen_flow}")
                emit_status(f"current Nitrogen flow is : {current_nitrogen_flow}")
                for serial_port in argon_serial_ports2:
                    if get_enable_bricklet():
                        MFC_open_nitrogen_flow(mfc_sfc6, serial_port, vserial_port, current_nitrogen_flow, measurement_screen)

                if get_enable_bricklet():
                    for serial_port, vserial_port in zip(argon_serial_ports2, voc_serial_ports1):
                        val1 = mfc_sfc6.read_average_value(serial_port, 5)
                        val2 = mfc_sfc6.read_average_value(vserial_port, 5)
                    
                        if val1 is None:
                            print(f'<BT> unable to set MFC Flow{serial_port}')
                            emit_status(f'<BT> unable to set MFC Flow{serial_port}')
                            measurement_screen.shared_state.set_failed_status(f"<BT> unable to set MFC Flow{serial_port} <Bricklet Failure>")
                            measurement_screen.shared_state.set_measurement_failed(True)   
                            return False

                        if val2 is None:
                            print(f'<BT> unable to set MFC Flow{serial_port}')
                            emit_status(f'<BT> unable to set MFC Flow{serial_port}')
                            measurement_screen.shared_state.set_failed_status(f"<BT> unable to set MFC Flow{serial_port} <Bricklet Failure>")
                            measurement_screen.shared_state.set_measurement_failed(True)   
                            return False

                        print(f'<BT> T3C Nr-MFC  <{serial_port}> flow : {val1}')
                        print(f'<BT> T3C VOC-MFC <{vserial_port}> flow : {val2}')
                        emit_status(f'<BT> Nr-MFC  <{serial_port}> flow : {val1}')
                        emit_status(f'<BT> VOC-MFC <{vserial_port}> flow : {val2}')
                    
                prev_nitrogen_flow = current_nitrogen_flow

            # user preassed the cancel button.
            if get_Cancel_Measurement():
                return

            #if datetime.now() > tnow + sleep_time:
            if datetime.now() > tnow + timedelta(seconds=sleep_time):
                break

            time.sleep(1)

        print(f"Counter ended at {get_current_time()}")
        measurement_screen.control_update_signal.emit("disable_controls")
        print(f"<BL E Timing : {get_current_time()} Completed t3 seconds")
        emit_status(f"<BL E Timing : {get_current_time()} Completed {get_t3()} t3 seconds")
        emit_status("Configuring Bricklet to Initial Value")
    else:
        print(f"T2 value is {get_t2()} Not Sleeping for T2 time")
        print("<BL E Timing : (T3) Not Sleeping for T3 time")
        print("Configuring Bricklet to Initial Value")

    # T3:
    # Stop VOC & Nitrogen to 0%:
    set_stop_measurement_data_acq(True)

    for serial_port in serial_ports:
        print(f"Serial Port = {serial_port}")
        if get_enable_bricklet():
            if not set_mfc_zero(mfc_sfc6, serial_port):
                print(f"<BT> Failed to set MFC <{serial_port}> to 0%")
                emit_status(f"<BT> Failed to set MFC <{serial_port}> to 0%")
                measurement_screen.shared_state.set_failed_status(f"<BT> Failed to set MFC <{serial_port}> to 0% <Bricklet Failure>")
                measurement_screen.shared_state.set_measurement_failed(True)   
                return False

    emit_status("""\n<BL> Set MFC-28Du to 0%

    Set MFC-28Do to 0%
    Set MFC-28CM to 0%
    Set MFC-28Dw to 0%""")

    #Set Exhaust Solenoid Valve = Closed
    if get_enable_bricklet():
        sv1_relay = get_sv1_relay()
        if not Exhaust_Solenoid_Valve_off(sv1_relay):
            print("<BL> Failed to set Exhaust Solenoid Valve to OFF/Closed 2f3R SW0")
            emit_status("<BL> Failed to set Exhaust Solenoid Valve to OFF/Closed 2f3R SW0")
            measurement_screen.shared_state.set_failed_status("<BT> Failed to set Exhaust Solenoid Valve to OFF/Closed 2f3R SW0 <Bricklet Failure>")
            measurement_screen.shared_state.set_measurement_failed(False)   
            return False

            
    if get_mainmeasurement():
        print("<BL> Printing T3 End Bricklet Status")
        emit_status("<BL> Printing T3 End Bricklet Status")
    else:
        print("<BL> Ending Bricklet configuration")

    if enable_print:
        print_bricklet_device_status(measurement_screen)

    measurement_screen.shared_state.set_Bricklet_T3Stage_Complete(True)

    while not get_measurement1Completed():
        print("Wait for One device (Device 1) to complete the measurement")
        # user preassed the cancel button.
        if get_Cancel_Measurement():
            print('<BT>  Stopped the Measurement due to user cancel request.')
            emit_status('<BT> Stopped the Measurement due to user cancel request.')
            break
        
        if datetime.now() > tnow + timedelta(seconds=10):
            print("<BT> Timeout waiting for Device 1 to complete the measurement")
            emit_status('<BT> Timeout waiting for Device 1 to complete the measurement')
            break
        time.sleep(1)

    set_measurement1Completed(False)

    if get_mainmeasurement():
        print("Measurement has been completed")
        emit_status("Measurement has been completed")
    else:
        print("Measurement has been completed")
        
    return True

def print_bricklet_device_status(measurement_screen = None):
    dual_relay = get_dual_relay()
    sv1_relay = get_sv1_relay()
    #serial ports for MFCs 1s & 2s
    voc_serial_ports1 = [mfc2_serial_port, mfc4_serial_port]
    argon_serial_ports2 = [mfc1_serial_port, mfc3_serial_port]

    def emit_status(status):
        if measurement_screen:
            measurement_screen.bricklet_status_signal.emit(status)

    print("<BT> PT Current Status of the Bricklet")
    emit_status("<BT> PT Current Status of the Bricklet")

    if get_enable_bricklet():
        sample = get_sample_temperature()
        coil_tube = get_tubing_temperature() 
        relay_status = sv1_get_relay_status(sv1_relay)
        dual_relay_status = dual_get_relay_status(dual_relay)
        mvp_version = measurement_screen.shared_state.get_MVP_Device_Version().rsplit("_", 1)[-1]
        if mvp_version == "1.5":
            vial_status = vial_base_status()
            emit_status(f"<BT> PT Vial Base status {vial_status}")
        print(f"<BT> PF sample temperature : {sample}")
        print(f"<BT> PF coil_tube temperature : {coil_tube}")
        print(f"<BT> PT Exhaust_Solenoid_Valve and VOC_valve status {relay_status}")
        print(f"<BT> PT Dual Relay status {dual_relay_status}")
        emit_status(f"<BT> PT sample temperature : {sample}")
        emit_status(f"<BT> PT coil_tube temperature : {coil_tube}")
        emit_status(f"<BT> PT Exhaust_Solenoid_Valve and VOC_valve status {relay_status}")
        emit_status(f"<BT> PT Dual Relay status {dual_relay_status}")        
        for serial_port, vserial_port in zip(argon_serial_ports2, voc_serial_ports1):
            print(f'<BT> PT Nr-MFC  <{serial_port}> flow : {mfc_sfc6.read_average_value(serial_port, 5)}')
            print(f'<BT> PT VOC-MFC <{vserial_port}> flow : {mfc_sfc6.read_average_value(vserial_port, 5)}')
            emit_status(f'<BT> PT Nr-MFC  <{serial_port}> flow : {mfc_sfc6.read_average_value(serial_port, 5)}')
            emit_status(f'<BT> PT VOC-MFC <{vserial_port}> flow : {mfc_sfc6.read_average_value(vserial_port, 5)}')


#////////////////////////////////********************************///////////////////////////////////
def start_nitrogen_and_voc_flow(measurement_screen = None):
    global  mfc_sfc6, serial_ports, enable_print

    sv1_relay = None

    def get_current_time():
        timestamp = time.time()
        readable_time = datetime.fromtimestamp(timestamp)
        only_time = readable_time.strftime("%H:%M:%S")
        return only_time

    def emit_status(status):
        if measurement_screen:
            measurement_screen.bricklet_status_signal.emit(status)
        #print(status)

    #serial ports for MFCs 1s & 2s
    voc_serial_ports1 = [mfc2_serial_port, mfc4_serial_port]
    argon_serial_ports2 = [mfc1_serial_port, mfc3_serial_port]

    # T0 START Measurement Process:
    # Set MFC-12,22,32,42  to Argon flow rate set in measurement table

    set_Start_Bricketlet_T0_flow(False)

    """
    Print the current status of the bricklet
    """

    if enable_print:
        print_bricklet_device_status(measurement_screen)

    if get_enable_bricklet():
        sv1_relay = get_sv1_relay()
        relay_states = sv1_backflush_relay(sv1_relay)
        print(f"relay backflush state {relay_states}")
        if relay_states:
            sv1_backflush_off(sv1_relay)
            measurement_screen.shared_state.set_BackFlush_ON(False)

    mvp_version = measurement_screen.shared_state.get_MVP_Device_Version().rsplit("_", 1)[-1]
    if mvp_version == "1.5":
        if get_enable_bricklet():
            if not vial_base_position() == VIAL_UPPER_LIMIT:
                resullt = vial_base_raise(VIAL_UPPER_LIMIT)
                print("stepper moved from home to maximum level")
                if not resullt:
                    print("<BT> Failed to move vial base to upper limit")
                    emit_status("<BT> Failed to move vial base to upper limit")
                    measurement_screen.shared_state.set_failed_status("<BT> Failed to move vial base to upper limit <Bricklet Failure>")
                    measurement_screen.shared_state.set_measurement_failed(True)   
                    return False


    #Exhast Open
    print("<BT> Set exhaust Solenoid Valve = on/Open 2f3R SW0")
    emit_status("<BT> Set exhaust Solenoid Valve = on/Open 2f3R SW0")
    
    if get_enable_bricklet():
        sv1_relay = get_sv1_relay()
        if (not Exhaust_Solenoid_Valve_on(sv1_relay)):
            print("<BT> Failed to set Exhaust Solenoid Valve to ON/Open 2f3R SW0")
            emit_status("<BT> Failed to set Exhaust Solenoid Valve to ON/Open 2f3R SW0")
            measurement_screen.shared_state.set_failed_status("<BT> Failed to set Exhaust Solenoid Valve to ON/Open 2f3R SW0 <Bricklet Failure>")
            measurement_screen.shared_state.set_measurement_failed(True)  
            return False 

    # T0 ==> T1 Starts :
    # Set Sensor 1,2 Nitrogen MFC & Sensor 1,2 VOC MFC Open
    for serial_port, vserial_port in zip(argon_serial_ports2, voc_serial_ports1):
        MFC_open_nitrogen_flow(mfc_sfc6, serial_port, vserial_port, get_Argon_flow_rate(), measurement_screen)

    for vserial_port, aserial_port in zip(voc_serial_ports1, argon_serial_ports2):
        MFC_open_VOC_flow(mfc_sfc6, aserial_port, vserial_port, get_VOC_argon_flow_rate(), get_Argon_flow_rate(), measurement_screen)


    if get_enable_bricklet():
        for serial_port, vserial_port in zip(argon_serial_ports2, voc_serial_ports1):
            val1 = mfc_sfc6.read_average_value(serial_port, 5)
            val2 = mfc_sfc6.read_average_value(vserial_port, 5)
            
            if val1 is None:
                print(f'<BT> unable to set MFC Flow{serial_port}')
                emit_status(f'<BT> unable to set MFC Flow{serial_port}')
                measurement_screen.shared_state.set_failed_status(f"<BT> unable to set MFC Flow{serial_port} <Bricklet Failure>")
                measurement_screen.shared_state.set_measurement_failed(True)   
                return False

            if val2 is None:
                print(f'<BT> unable to set MFC Flow{serial_port}')
                emit_status(f'<BT> unable to set MFC Flow{serial_port}')
                measurement_screen.shared_state.set_failed_status(f"<BT> unable to set MFC Flow{serial_port} <Bricklet Failure>")
                measurement_screen.shared_state.set_measurement_failed(True)   
                return False

            print(f'<BT> T0 Nr-MFC  <{serial_port}> flow : {val1}')
            print(f'<BT> T0 VOC-MFC <{vserial_port}> flow : {val2}')
            emit_status(f'<BT> T0 Nr-MFC  <{serial_port}> flow : {val1}')
            emit_status(f'<BT> T0 VOC-MFC <{vserial_port}> flow : {val2}')

        #VOC Solenoid OFF
        if (not VOC_Solenoid_Valve_off(sv1_relay)):
            print("<BT> Failed to set VOC Solenoid Valve to Close")
            emit_status("<BT> Failed to set VOC Solenoid Valve to Close")
            measurement_screen.shared_state.set_failed_status("<BT> Failed to set VOC Solenoid Valve to Close <Bricklet Failure>")
            measurement_screen.shared_state.set_measurement_failed(True)    
            return False


    print(f"<BT> Timing : {get_current_time()}> T1 time started")
    emit_status(f"<BT> Timing : {get_current_time()}> T1 time started ")
    print(f"<BT> dwell : {measurement_screen.shared_state.cube.d_time},max : {measurement_screen.shared_state.var_vg_max},min : {measurement_screen.shared_state.var_vg_min},g_step : {measurement_screen.shared_state.var_g_step} ")
    measurement_screen.set_meas_max_count(get_t1() + get_t2() + get_t3())
    
    print("<BT> Setting the Start dataacq event")
    Start_timer_event.set()
    #now starting data acquisition
    Start_data_acquisition_event.set()

    print(f"<BT> S Timing : {get_current_time()} T0 ==> T1 Starts")
    emit_status(f"<BT> <Timing : {get_current_time()} T0 ==> T1 Starts")

    # sleep_time = get_t1()
    # while True:
    #     # user preassed the cancel button.
    #     if get_Cancel_Measurement():
    #         print("<BT> <Press Cancel :Coming out from T1 phase>")
    #         return False

    #     # Due to Failed start data acquisition
    #     if not measurement_screen.shared_state.get_continue_measurement():
    #         print("<BT> Measurement stopped :Coming out from T1 phase>")
    #         return False

    #     if sleep_time == 0:
    #         break
    #     time.sleep(1)
    #     sleep_time = sleep_time - 1
    
    tnow = datetime.now()
    print_status = True
    while True:
        if datetime.now() > tnow + timedelta(seconds=get_t1()):
            break               
        if get_enable_bricklet() and  print_status:
            print_status = False
            for vserial_port, aserial_port in zip(voc_serial_ports1, argon_serial_ports2):
                val1 = mfc_sfc6.read_average_value(aserial_port, 5)
                val2 = mfc_sfc6.read_average_value(vserial_port, 5)
                
                if val1 is None:
                    print(f'<BT> unable to set MFC Flow{aserial_port}')
                    emit_status(f'<BT> unable to set MFC Flow{aserial_port}')
                    measurement_screen.shared_state.set_failed_status(f"<BT> unable to set MFC Flow{aserial_port} <Bricklet Failure>")
                    measurement_screen.shared_state.set_measurement_failed(True)   
                    return False

                if val2 is None:
                    print(f'<BT> unable to set MFC Flow{vserial_port}')
                    emit_status(f'<BT> unable to set MFC Flow{vserial_port}')
                    measurement_screen.shared_state.set_failed_status(f"<BT> unable to set MFC Flow{vserial_port} <Bricklet Failure>")
                    measurement_screen.shared_state.set_measurement_failed(True)   
                    return False
                
                print(f'<BT> T2 Nr-MFC  <{aserial_port}> flow :  {val1}')
                print(f'<BT> T2 VOC-MFC <{vserial_port}> flow : {val2}')
                emit_status(f'<BT> T2 Nr-MFC  <{aserial_port}> flow : {val1}')
                emit_status(f'<BT> T2 VOC-MFC <{vserial_port}> flow : {val2}')
                
        print(f"<BT> Timing : {get_current_time()} print_time")
        print(f"<BT> Set the flowrate to {get_VOC_argon_flow_rate()}")
        time.sleep(1)  
        # user preassed the cancel button.
        if get_Cancel_Measurement() == True:
            print(f"<Press Cancel :Coming out from T1 phase>")
            return
        # Due to Failed start data acquisition
        if measurement_screen.shared_state.get_continue_measurement() == False:
            return
    
    print(f"<BT> S Timing : {get_current_time()} Completed {get_t1()} seconds - T1 phase End === T2 Starts")
    emit_status(f"<BT> Timing : {get_current_time()} Completed {get_t1()} seconds - T1 phase End === T2 Starts")

    # # T1 Turn on VOC solenoid valves
    # if get_enable_bricklet():
    #     if (not VOC_Solenoid_Valve_on(sv1_relay)):
    #         print("<BT> Failed to set VOC Solenoid Valve to Open")
    #         emit_status("<BT> Failed to set VOC Solenoid Valve to Open")
    #         measurement_screen.shared_state.set_failed_status("<BT> Failed to set VOC Solenoid Valve to Open <Bricklet Failure>")
    #         measurement_screen.shared_state.set_measurement_failed(True)    
    #         return False
        
    # print("<BT> Turn on VOC solenoid valve")
    # emit_status("<BT> Turn on VOC solenoid valve")

    # if get_enable_bricklet():
    #     print(f"<BT> {serial_port} Flow Rate Set to SCCM (Argon_flow_rate%)")

    # print(f"<BT> Starting VOC Flow Normal_Measurement {get_Normal_Measurement()} Sweep_Measurement {get_Sweep_Measurement()}")

    # Voc enabling condition.
    # T2 Start flowing VOCs + Nitrogen

    tnow = datetime.now()
    if (get_t2() > 0 ):
        #for vserial_port, aserial_port in zip(voc_serial_ports1, argon_serial_ports2):
        #    MFC_VOC_argon_flow_mvp(mfc_sfc6, aserial_port, vserial_port, get_VOC_argon_flow_rate(), get_Argon_flow_rate(), measurement_screen)
        # print(f"<BL S Timing : {get_current_time()}> print_time")
        # if get_enable_bricklet():
        #     for vserial_port, aserial_port in zip(voc_serial_ports1, argon_serial_ports2):
        #         val1 = mfc_sfc6.read_average_value(aserial_port, 5)
        #         val2 = mfc_sfc6.read_average_value(vserial_port, 5)
                
        #         if val1 is None:
        #             print(f'<BT> unable to set MFC Flow{aserial_port}')
        #             emit_status(f'<BT> unable to set MFC Flow{aserial_port}')
        #             measurement_screen.shared_state.set_failed_status(f"<BT> unable to set MFC Flow{aserial_port} <Bricklet Failure>")
        #             measurement_screen.shared_state.set_measurement_failed(True)   
        #             return False

        #         if val2 is None:
        #             print(f'<BT> unable to set MFC Flow{vserial_port}')
        #             emit_status(f'<BT> unable to set MFC Flow{vserial_port}')
        #             measurement_screen.shared_state.set_failed_status(f"<BT> unable to set MFC Flow{vserial_port} <Bricklet Failure>")
        #             measurement_screen.shared_state.set_measurement_failed(True)   
        #             return False
                
        #         print(f'<BT> T2 Nr-MFC  <{aserial_port}> flow :  {val1}')
        #         print(f'<BT> T2 VOC-MFC <{vserial_port}> flow : {val2}')
        #         emit_status(f'<BT> T2 Nr-MFC  <{aserial_port}> flow : {val1}')
        #         emit_status(f'<BT> T2 VOC-MFC <{vserial_port}> flow : {val2}')
        # print(f"<BT> Timing : {get_current_time()} print_time")
        # print(f"<BT> Set the flowrate to {get_VOC_argon_flow_rate()}")
        # time.sleep(0.1)

        if get_enable_bricklet():
            if not VOC_Solenoid_Valve_on(sv1_relay):
                print("<BT> Failed to set VOC Solenoid Valve to Open")
                emit_status("<BT> Failed to set VOC Solenoid Valve to Open")
                measurement_screen.shared_state.set_failed_status("<BT> Failed to set VOC Solenoid Valve to Open <Bricklet Failure>")
                measurement_screen.shared_state.set_measurement_failed(True)   
                return False
  
            for vserial_port, aserial_port in zip(voc_serial_ports1, argon_serial_ports2):
                MFC_VOC_argon_flow_mvp(mfc_sfc6, aserial_port, vserial_port, get_VOC_argon_flow_rate(), get_Argon_flow_rate(), measurement_screen)
        
        print("<BT> Turn on VOC solenoid valve")
        emit_status("<BT> Turn on VOC solenoid valve")

        print(f"<BT> S Timing : {get_current_time()} wait for {get_t2()} seconds for completing T2 phase")
        emit_status(f"<BT> S Timing <Start VOC Flow>  : {get_current_time()} wait for {get_t2()} seconds for completing T2 phase)")

        # sleep_time = get_t2()
        # while True:
        #     if sleep_time == 0:
        #         break
        #     time.sleep(1)
        #     sleep_time = sleep_time - 1
        #     # user preassed the cancel button.
        #     if get_Cancel_Measurement():
        #         print("<Press Cancel :Coming out from T2 phase>")
        #         return False

        #tnow = datetime.now()
        print_status = True
        while True:
            if datetime.now() > tnow + timedelta(seconds=get_t2()):
                break               
            print(f"<BL S Timing : {get_current_time()}> print_time")
            if get_enable_bricklet()  and print_status:
                print_status = False
                for serial_port, vserial_port in zip(argon_serial_ports2, voc_serial_ports1):
                    val1 = mfc_sfc6.read_average_value(serial_port, 5)
                    val2 = mfc_sfc6.read_average_value(vserial_port, 5)
                    
                    if val1 is None:
                        print(f'<BT> unable to set MFC Flow{serial_port}')
                        emit_status(f'<BT> unable to set MFC Flow{serial_port}')
                        measurement_screen.shared_state.set_failed_status(f"<BT> unable to set MFC Flow{serial_port} <Bricklet Failure>")
                        measurement_screen.shared_state.set_measurement_failed(True)   
                        return False

                    if val2 is None:
                        print(f'<BT> unable to set MFC Flow{vserial_port}')
                        emit_status(f'<BT> unable to set MFC Flow{vserial_port}')
                        measurement_screen.shared_state.set_failed_status(f"<BT> unable to set MFC Flow{vserial_port} <Bricklet Failure>")
                        measurement_screen.shared_state.set_measurement_failed(True)   
                        return False
                    
                    print(f'<BT> T3 Nr-MFC  <{serial_port}> flow :  {val1}')
                    print(f'<BT> T3 VOC-MFC <{vserial_port}> flow : {val2}')
                    emit_status(f'<BT> T3 Nr-MFC  <{serial_port}> flow : {val1}')
                    emit_status(f'<BT> T3 VOC-MFC <{vserial_port}> flow : {val2}')
                print(f"<BT> E Timing : {get_current_time()}> print_time")

            time.sleep(1)  
            # user preassed the cancel button.
            if get_Cancel_Measurement() == True:
                print(f"<Press Cancel :Coming out from T2 phase>")
                return

        #time.sleep(get_t2() - 1)
        print(f"<BT> E Timing : {get_current_time()} Completed T2 seconds === T3 Starts")
        emit_status(f"<BT> E Timing <End VOC Flow> : {get_current_time()} Completed {get_t2()} T2 seconds === T3 Starts")

        # T3:
        #Set Sensor 1,2 dual_relay_Valve = off/closed
        if get_enable_bricklet() == True:
            if VOC_Solenoid_Valve_off(sv1_relay) == False:
                print(f" Failed to set VOC Solenoid Valve to Close")
                emit_status(f" Failed to set VOC Solenoid Valve to Close")

        print(f" Turn off VOC solenoid valve")
        emit_status(f" Turn off VOC solenoid valve")

        #Set Sensor 1,2 dual_relay_Valve = off/closed
        for serial_port, vserial_port in zip(argon_serial_ports2, voc_serial_ports1):
            MFC_open_nitrogen_flow(mfc_sfc6, serial_port, vserial_port, get_Argon_flow_rate(), measurement_screen)
            #MFC_open_VOC_flow(mfc_sfc6, serial_port, vserial_port, get_VOC_argon_flow_rate(), get_Argon_flow_rate(), measurement_screen)

        # print(f"<BL S Timing : {get_current_time()}> print_time")
        # if get_enable_bricklet():
        #     for serial_port, vserial_port in zip(argon_serial_ports2, voc_serial_ports1):
        #         val1 = mfc_sfc6.read_average_value(serial_port, 5)
        #         val2 = mfc_sfc6.read_average_value(vserial_port, 5)
                
        #         if val1 is None:
        #             print(f'<BT> unable to set MFC Flow{serial_port}')
        #             emit_status(f'<BT> unable to set MFC Flow{serial_port}')
        #             measurement_screen.shared_state.set_failed_status(f"<BT> unable to set MFC Flow{serial_port} <Bricklet Failure>")
        #             measurement_screen.shared_state.set_measurement_failed(True)   
        #             return False

        #         if val2 is None:
        #             print(f'<BT> unable to set MFC Flow{vserial_port}')
        #             emit_status(f'<BT> unable to set MFC Flow{vserial_port}')
        #             measurement_screen.shared_state.set_failed_status(f"<BT> unable to set MFC Flow{vserial_port} <Bricklet Failure>")
        #             measurement_screen.shared_state.set_measurement_failed(True)   
        #             return False
                
        #         print(f'<BT> T3 Nr-MFC  <{serial_port}> flow :  {val1}')
        #         print(f'<BT> T3 VOC-MFC <{vserial_port}> flow : {val2}')
        #         emit_status(f'<BT> T3 Nr-MFC  <{serial_port}> flow : {val1}')
        #         emit_status(f'<BT> T3 VOC-MFC <{vserial_port}> flow : {val2}')
        # print(f"<BT> E Timing : {get_current_time()}> print_time")


    set_transfer_rawtext_file(True)


    #emit_status("<BT> Taking the Measurement")


    # print(f"<BT> S Timing : {get_current_time()} wait for {get_t3()} seconds for completing T3 phase)")
    # emit_status(f"<BT> S Timing : {get_current_time()} wait for {get_t3()} seconds for completing T3 phase)")

    # if get_enable_bricklet():
    #     sleep_time = get_t3()
    #     while True:
    #         # user preassed the cancel button.
    #         if get_Cancel_Measurement():
    #             print("<BT> <Press Cancel : 1. Coming out from T3 phase>")
    #             emit_status("<BT> <Press Cancel : 1. Coming out from T3 phase>")
    #             return False

    #         if sleep_time == 0:
    #             break
    #         time.sleep(1)
    #         sleep_time = sleep_time - 1
    
    print(f"<BL S Timing : {get_current_time()} wait for {get_t3()} seconds for completing T3 phase)")
    emit_status(f"<BL S Timing : {get_current_time()} wait for {get_t3()} seconds for completing T3 phase)")

    if get_enable_bricklet() == True:
        tnow = datetime.now()
        print_status = True
        while True:
            if datetime.now() > tnow + timedelta(seconds=get_t3()):
                break               
            if get_enable_bricklet() == True and print_status:
                print_status = False
                for serial_port, vserial_port in zip(argon_serial_ports2, voc_serial_ports1):
                    val1 = mfc_sfc6.read_average_value(serial_port, 5)
                    val2 = mfc_sfc6.read_average_value(vserial_port, 5)
                    
                    if val1 is None:
                        print(f'<BT> unable to set MFC Flow{serial_port}')
                        emit_status(f'<BT> unable to set MFC Flow{serial_port}')
                        measurement_screen.shared_state.set_failed_status(f"<BT> unable to set MFC Flow{serial_port} <Bricklet Failure>")
                        measurement_screen.shared_state.set_measurement_failed(True)   
                        return False

                    if val2 is None:
                        print(f'<BT> unable to set MFC Flow{vserial_port}')
                        emit_status(f'<BT> unable to set MFC Flow{vserial_port}')
                        measurement_screen.shared_state.set_failed_status(f"<BT> unable to set MFC Flow{vserial_port} <Bricklet Failure>")
                        measurement_screen.shared_state.set_measurement_failed(True)   
                        return False
                    
                    print(f'<BT> T3 Nr-MFC  <{serial_port}> flow :  {val1}')
                    print(f'<BT> T3 VOC-MFC <{vserial_port}> flow : {val2}')
                    emit_status(f'<BT> T3 Nr-MFC  <{serial_port}> flow : {val1}')
                    emit_status(f'<BT> T3 VOC-MFC <{vserial_port}> flow : {val2}')
                print(f"<BT> E Timing : {get_current_time()}> print_time")

            time.sleep(1)  
            # user preassed the cancel button.
            if get_Cancel_Measurement() == True:
                print(f"<Press Cancel :Coming out from T3 phase>")
                return
    else:
        sleep_time = get_t3()
        while True:
            # user preassed the cancel button.
            if get_Cancel_Measurement():
                print("<BT> <Press Cancel : 2. Coming out from T3 phase>")
                emit_status("<BT> <Press Cancel : 2. Coming out from T3 phase>")
                return False
            if sleep_time == 0:
                break
            time.sleep(1)
            sleep_time = sleep_time - 1

    set_stop_measurement_data_acq(True)
    print(f"<BT> E Timing : {get_current_time()} Completed T3 seconds")
    emit_status(f"<BT> E Timing : {get_current_time()} Completed {get_t3()} T3 seconds")

    # Stop VOC & Nitrogen to 0%:
    for serial_port in serial_ports:
        print(f"Serial Port = {serial_port}")
        if get_enable_bricklet():
            if not set_mfc_zero(mfc_sfc6, serial_port):
                print(f"<BT> Failed to set MFC <{serial_port}> to 0%")
                emit_status(f"<BT> Failed to set MFC <{serial_port}> to 0%")
                measurement_screen.shared_state.set_failed_status(f"<BT> Failed to set MFC <{serial_port}> to 0% <Bricklet Failure>")
                measurement_screen.shared_state.set_measurement_failed(True)  
                return False                
                    
    emit_status("""\nSet MFC-28Du to 0%
    Set MFC-28Do to 0%
    Set MFC-28CM to 0%
    Set MFC-28Dw to 0%""")

    # T3 AFTER:
    #Set Exhaust Solenoid Valve = Closed
    if get_enable_bricklet():
        sv1_relay = get_sv1_relay()
        if not Exhaust_Solenoid_Valve_off(sv1_relay):
            print("<BT> Failed to set Exhaust Solenoid Valve to Close")
            emit_status("<BT> Failed to set Exhaust Solenoid Valve to Close")
            measurement_screen.shared_state.set_failed_status("<BT> Failed to set Exhaust Solenoid Valve to OFF/Closed 2f3R SW0 <Bricklet Failure>")
            measurement_screen.shared_state.set_measurement_failed(True)  
            return False                

    print("<BT> Turn off Exhaust Solenoid valve")
    emit_status(" Turn off Exhaust Solenoid valve")


    print("<BT> Printing T3 End Bricklet Status")
    emit_status(" <BT> Printing T3 End Bricklet Status")

    """
    Print the current status of the bricklet
    """
    if enable_print:
        print_bricklet_device_status(measurement_screen)

    measurement_screen.shared_state.set_Bricklet_T3Stage_Complete(True)

    tnow = datetime.now()
    while not get_measurement1Completed():
        print("<BT> Wait for One device (Device 1) to complete the measurement")
        if datetime.now() > tnow + timedelta(seconds=10):
            print("<BT> Error : Unable to come out from the Final measurement stage")
            break
            
        if get_Cancel_Measurement():
            print("<BT> <Network Error>")
            emit_status("<BT> Network Error")
            return False
            
        time.sleep(1)

    #Turn OFF Exhaust solenoid
    if get_enable_bricklet():
        Exhaust_Solenoid_Valve_off(sv1_relay)

    set_measurement1Completed(False)

    emit_status("<BT> Measurement has been completed")


    mvp_version = measurement_screen.shared_state.get_MVP_Device_Version().rsplit("_", 1)[-1]
    if mvp_version == "1.5":
        auto = measurement_screen.shared_state.get_BackFlush_Auto()
        if get_enable_bricklet() == True and auto == True:  

            emit_status("<BT> Started Backflush vials move down to -1000000 steps")
            # 1. vials move down to -1000000 steps
            result = vial_base_raise(VIAL_1_3RD_POSITION)
            if not result:
                print("<BT> Failed to move vial base to 1/3rd position")
                emit_status("<BT> Failed to move vial base to 1/3rd position")
                measurement_screen.shared_state.set_failed_status("<BT> Failed to move vial base to 1/3rd position <Bricklet Failure>")
                measurement_screen.shared_state.set_measurement_failed(True)   
                return False

            emit_status("<BT> Turn on backflush solenoid valve")
            #2. back flush solenoid valves open
            sv1_backflush_on(sv1_relay)
            
            #3. back flush N2 time runs
            N2_Period = get_N2_Period()
            emit_status(f"<BT> Backflush N2 time running for {N2_Period} seconds")
            print(f"N2 time : int {N2_Period}")
            time.sleep(N2_Period)

            #4. back flush solenoids close
            emit_status("<BT> Turn off backflush solenoid valve")
            sv1_backflush_off(sv1_relay)

            emit_status("<BT> Moving vial base to upper limit")
            # 5. Raise vial base to upper limit for backflush               
            result = vial_base_raise(VIAL_UPPER_LIMIT)
            if not result:
                print("<BT> Failed to move vial base to upper limit")
                emit_status("<BT> Failed to move vial base to upper limit")
                measurement_screen.shared_state.set_failed_status("<BT> Failed to move vial base to upper limit <Bricklet Failure>")
                measurement_screen.shared_state.set_measurement_failed(True)   
                return False
            
            emit_status("<BT> Completed Backflush process")

    return True


def Power_UP_Bricklet(ip_con,update_status_callback=None):
    global thermocouple2, mfc_sfc6, serial_ports, thermocouple1

    sv1_relay = None

    def get_current_time():
        timestamp = time.time()
        readable_time = datetime.fromtimestamp(timestamp)
        only_time = readable_time.strftime("%H:%M:%S")
        return only_time

    get_tubing_coil_temp()

    print(f"<BL S Timing : {get_current_time()} MVP Bricklet starting time")
    print("Power Up Sequence to Measurement READY")
    if update_status_callback:
        update_status_callback("Power Up Sequence to Measurement READY")

    # 2️⃣ Initialize Devices
    # Initialize the Thermcouple Bricklets
    thermocouple1 = tc1_initialize_thermocouple(ip_con)
    print("Initialized the Sample Heater <2f1H> : Thermcouple")
    if update_status_callback:
        update_status_callback("Initialized the Sample Heater <2f1H> : Thermcouple")

    thermocouple2 = tc2_initialize_thermocouple(ip_con)
    print("Initialized the Tubing Heater <28H8> : Thermcouple")
    if update_status_callback:
        update_status_callback("Initialized the Tubing Heater <28H8> : Thermcouple")

    # If the MFCs are already opened, disconnect all the MFCs
    if  mfc_sfc6 is not None:
        print("Disconnecting all MFCs")
        if update_status_callback:
            update_status_callback("Disconnecting all MFCs")
        mfc_sfc6.disconnect(mfc1_serial_port)
        mfc_sfc6.disconnect(mfc2_serial_port)
        mfc_sfc6.disconnect(mfc3_serial_port)
        mfc_sfc6.disconnect(mfc4_serial_port)
        print("Closing all the MFCs*****")

    # Connect to the all MFCs
    mfc_sfc6 = mfc_sfc5
    print("Connecting MFC1*****")
    ret = mfc_sfc6.connect(mfc1_serial_port, baudrate=115200,additional_response_time=2.0, slave_address=0)
    if ret == "Connect failed" :    
        print(f"Error in connecting MFC1: {ret}")
        if update_status_callback:
            update_status_callback(f"Error in connecting MFC1: {mfc1_serial_port} {ret}")
        return False
    
    print("Connecting MFC2*****")   
    ret = mfc_sfc6.connect(mfc2_serial_port, baudrate=115200,additional_response_time=2.0, slave_address=0)
    if  ret == "Connect failed" :    
        print(f"Error in connecting MFC2: {ret}")
        if update_status_callback:
            update_status_callback(f"Error in connecting MFC2: {mfc2_serial_port} {ret}")
        return False

    print("Connecting MFC3*****")
    ret = mfc_sfc6.connect(mfc3_serial_port, baudrate=115200,additional_response_time=2.0, slave_address=0)
    if  ret == "Connect failed" :    
        print(f"Error in connecting MFC3: {ret}")
        if update_status_callback:
            update_status_callback(f"Error in connecting MFC3: {mfc3_serial_port} {ret}")
        return False
    
    print("Connecting MFC4*****")
    mfc_sfc6.connect(mfc4_serial_port, baudrate=115200,additional_response_time=2.0, slave_address=0)
    if  ret == "Connect failed" :    
        print(f"Error in connecting MFC4: {ret}")
        if update_status_callback:
            update_status_callback(f"Error in connecting MFC4: {mfc4_serial_port} {ret}")
        return False

    print("Connected to the all MFCs")
    if update_status_callback:
        update_status_callback("Connected to the all MFCs")


    #  Set all MFC to 0%, i.e. Closed
    for serial_port in serial_ports:
        print(f"Serial Port = {serial_port}")
        ptype = mfc_sfc6.product_type(serial_port)
        print(f"Product type = {ptype}")
        time.sleep(1)
        if set_initial_mfc_zero(mfc_sfc6, serial_port):
            print(f" Failed to set MFC <{serial_port}> to 0%")
            if (not set_initial_mfc_zero(mfc_sfc6, serial_port)):
                print(f" Failed to set MFC again <{serial_port}> to 0%")
                if update_status_callback:
                    update_status_callback(f"Error in Setting MFC {serial_port} to Zero {ret}")
                return False

    print("Set all MFC = 0%, Closed")
    if update_status_callback:
        update_status_callback("Set all MFC = 0%, Closed")
        time.sleep(1)

    #Solenoid Valve all Open
    sv1_initialize_relay(ip_con)
    sv1_relay = get_sv1_relay() 
    if (not Exhaust_Solenoid_Valve_on(sv1_relay)):
        print(" Failed to Open Exhaust Solenoid Valve ")
        if update_status_callback:
            update_status_callback(" Failed to Open Exhaust Solenoid Valve ")
        return False

    #Open All Nitrogen MFCs 30% for 5 Seconds, then 0%
    for serial_port in serial_ports:
        print(f"opening Serial Port = {serial_port}")
        if (not open_mfc_rate(mfc_sfc6, serial_port, 30)):
            print(f" Failed to open MFC <{serial_port}> to 30%")
            if update_status_callback:
                update_status_callback(f" Failed to open MFC <{serial_port}> to 30%")
            return False

    print("Opened all the Nitrogen/VOC MFCs - to 30% for 5 Seconds, then 0%s")

    time.sleep(5)
    for serial_port in serial_ports:
        if (not open_mfc_rate(mfc_sfc6,serial_port, 0)):
            print(f" Failed to close MFC <{serial_port}> to 0%")
            if update_status_callback:
                update_status_callback(f" Failed to close MFC <{serial_port}> to 0%")
            return False
        
    print("Opened all the Nitrogen/VOC MFCs - to 30% for 5 Seconds, then 0%")
    if update_status_callback:
        update_status_callback("Opened all the Nitrogen/VOC MFCs - to 30% for 5 Seconds, then 0%")

    #Solenoid Valve all Closed
    sv1_relay = get_sv1_relay()
    if (not Exhaust_Solenoid_Valve_off(sv1_relay)):
        print(" Failed to Close Exhaust the Solenoid Valve ")
        if update_status_callback:
            update_status_callback(" Failed to Close Exhaust Solenoid Valve ")
        return False
    
    print("Closed Exhaust the Solenoid Valve ")
    sv1_backflush_off(sv1_relay)
    print("Backflush Turned off")
    if update_status_callback:
        update_status_callback("Exhaust Solenoid Valve = Closed")

    print("Initialize the Dual relay <Sample/tubing>")

    shared_state = SharedState()
    mvp_version = shared_state.get_MVP_Device_Version().rsplit("_", 1)[-1]
    if mvp_version == "1.5":
        #Vial_Base initialization
        vial_base_initialize_stepper(ip_con)
        vial_base = get_vial_base()
        vial_start_home()
        if update_status_callback:
            update_status_callback("Initialize the Vial Base brick")

    #Dual Ac Relay initialization
    dual_initialize_relay(ip_con)
    if update_status_callback:
        update_status_callback("Initialize the Dual relay <Sample/tubing>")

    #Air quality brick initialization
    try:
        aq_initialize(ip_con)
        print("Initialize the air_quality brick")
        if update_status_callback:
            update_status_callback("Initialize the air_quality brick")
    except Exception as e:
        print(f"Error initializing air quality brick: {e}")
        return "AirQ Failed"

    print(f"<BL E Timing : {get_current_time()} Bricklet stop time")

    print("Initialization Completed.")
    if update_status_callback:
        update_status_callback("Full Test Completed. Connection Closed.")
        set_temperature_monitor_active(True)

    set_bricklet_initialization_completed(True)
    return True

def clear_bricklets():
    global mfc_sfc6, serial_ports, sv1_relay,vial_base

    if get_enable_bricklet():
        # Off the Solenoid Valve
        try:
            Exhaust_Solenoid_Valve_off(sv1_relay)
        except Exception as e:
            print(f"<BT> Error in clear_bricklets Exhaust_Solenoid_Valve_off : {e}")	
                    
        try:
            set_initial_mfc_zero(mfc_sfc6, serial_ports[0])
        except Exception as e:
            print(f"<BT> Error in clear_bricklets set MFC1 {serial_ports[0]}: {e}")	
            
        try:
            set_initial_mfc_zero(mfc_sfc6, serial_ports[1])
        except Exception as e:
            print(f"<BT> Error in clear_bricklets set MFC2 {serial_ports[1]}: {e}")	
            
        try:
            set_initial_mfc_zero(mfc_sfc6, serial_ports[2])
        except Exception as e:
            print(f"<BT> Error in clear_bricklets set MFC3 {serial_ports[2]}: {e}")	
            
        try:
            set_initial_mfc_zero(mfc_sfc6, serial_ports[3])
        except Exception as e:
            print(f"<BT> Error in clear_bricklets set MFC4 {serial_ports[3]}: {e}")	   

        try:
            sv1_backflush_off(sv1_relay)
        except Exception as e:
            print(f"<BT> Error in clear_bricklets sv1_backflush_off : {e}")

        shared_state = SharedState()
        mvp_version = shared_state.get_MVP_Device_Version().rsplit("_", 1)[-1]
        if mvp_version == "1.5":
            try:
                vial_base_off()
            except Exception as e:
                print(f"<BT> Error in clear_bricklets vial_base.set_enabled(False) : {e}")


def BrickletThread(measurement_screen=None):
    global mfc_sfc6, serial_ports 

    def emit_status(status):
        try:
            if measurement_screen:
                measurement_screen.bricklet_status_signal.emit(status)
        except Exception as e:
            print(f"<BT> Error in emit_status: {e}")

    try:
        start_bricklet_event.clear()
        while True:
            print("<BT> Wait for receiving the Start Bricklet event")
            while True:
                rc = False
                # Wait for receiving the event
                event_set = start_bricklet_event.wait(timeout=1)
                if event_set:
                    break

                # During the measurment close.
                if get_Appclose():
                    print("<BT> App is closing, exiting BrickletThread")
                    set_BrickletThread_Closed(True)
                    return
                
                if get_Cancel_Measurement():
                    print("<BT> User Pressed the Cancel button During the Measurment, exiting BrickletThread")
                    set_BrickletThread_Closed(True)
                
                time.sleep(1)

            # Received the event, Starting the Bricklet configuration
            print("<BT> Received the Start Bricklet event")
            emit_status("<BT> Received the Start Bricklet event")
            start_bricklet_event.clear()
            emit_status("<BT> Processing Start Bricklet event...")
            print("<BT> Processing Start Bricklet event...")
            dual_target_temperature = get_heater_temperature()
            tubing_heater_temperature = get_tubing_coil_temp()
            print(f"<BT> sample temperature Table Value : {dual_target_temperature}")
            print(f"<BT> tubing temperature Table Value : {tubing_heater_temperature}")

            # Check the Temperature of the Sample and Tubing
            # Wait until the configured Sample and Tubing Temperature
            if get_enable_bricklet():
                if measurement_screen.shared_state.get_Flag_Sample_Heat_chkbox() and measurement_screen.shared_state.get_Flag_Coil_Heat_chkbox():
                    if not wait_for_required_temp_to_set():
                        if not get_Cancel_Measurement():
                            print("<BT> Temperature not Reached == Returning")
                            set_check_temperature(False)
                            measurement_screen.measurement_complete_signal.emit(False)
                        continue

            tnow = datetime.now()
            print(f"<BT> {tnow.strftime('%Y-%m-%d %H:%M:%S')} : Started ")

            try:
                print(f"<BT> Normal_Measurement : {get_Normal_Measurement()} Sweep_Measurement {get_Sweep_Measurement()}")
                # if t2 is Zero, do not flow VOC
                if get_t2() > 0 and not get_Normal_Measurement() and not get_Sweep_Measurement():
                    print("<BT> Started Main Measurement with VCC")
                    # Enable Argon and VOC only for the main measurement
                    rc = start_nitrogen_and_voc_flow(measurement_screen)
                else:
                    print("<BT> Started AdvanceSettings Sweep / Measurement with Bricklet ")
                    # # Enable only Argon
                    rc = start_nitrogen_flow(measurement_screen)
                    
                if not rc:
                    print("<BT> Error in starting the Bricklet Flow Configuration")
                    emit_status("<BT> Error in starting the Bricklet Flow Configuration")
                    measurement_screen.shared_state.set_PowerupBricklets_onetime_initialization(False)
                    # Enabling all the flags to continue the measurement
                    clear_bricklets()
                    print("<BT> Cleared the Bricklet Settings after Failed to start the Bricklet Flow Configuration")
                    
                    if measurement_screen.shared_state.is_eth_up() and not get_Cancel_Measurement():      
                        measurement_screen.shared_state.set_failed_status("<BT> Error in starting the Bricklet Flow Configuration, Cancelling the Measurement")
                        measurement_screen.shared_state.set_measurement_failed(True)                     
                    continue
                          
            except Exception as e:
                print(f"<BT> Error in starting the Bricklet Flow Configuration: {e} ")
                emit_status("<BT> Error in starting the Bricklet Flow Configuration")
                # Enabling all the flags to continue the measurement
                measurement_screen.shared_state.set_PowerupBricklets_onetime_initialization(False)
                #Start_data_acquisition_event.set()
                #set_stop_measurement_data_acq(True)
                clear_bricklets()
                print("<BT> Cleared the Bricklet Settings after Exception")
                if measurement_screen.shared_state.is_eth_up() and not get_Cancel_Measurement():
                    measurement_screen.shared_state.set_failed_status("<BT> Exception in starting the Bricklet Flow Configuration, Cancelling the Measurement")
                    measurement_screen.shared_state.set_measurement_failed(True)                 
                continue


            try:
                # Clear all the bricklet settings after the measurement
                clear_bricklets()
                if (measurement_screen.shared_state.get_EnableBricklets_Checkbox()) and (get_Normal_Measurement() or get_Sweep_Measurement()):
                    print("View graph is enabled")
                    set_view_graph1(True)
                else:
                    set_view_graph(True)
            except Exception as e:
                print(f"Error in graph view logic: {e}")

    except Exception as e:
        print(f"Critical error in BrickletThread: {e}")

