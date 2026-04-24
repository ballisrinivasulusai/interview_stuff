"""

Global Variable Decalarations.

"""
from cubex_lib.cube_interface import Cube
#from intergrate import create_connection
#import integrate
import os
import threading
import subprocess


BOARD_PORT = 22
BOARD_USER = "root"
BOARD_PASSWORD = "voc@123"
APP_VERSION = "V1.8.4"

class SharedState:
 
    """
    Thread-safe Singleton SharedState
    """

    _instance = None
    _instance_lock = threading.Lock()

    def __new__(cls):
        if cls._instance is None:
            with cls._instance_lock:
                if cls._instance is None:
                    cls._instance = super().__new__(cls)
                    cls._instance._initialized = False
        return cls._instance   
    
    
    _lock = threading.Lock()
    def __init__(self):            
        # ===============================
        # PREVENT RE-INITIALIZATION
        # ===============================
        if self._initialized:
            return

        self._initialized = True
        self._lock = threading.Lock()
        
        self.cube = Cube()
        self.connection_status = {"Device 1": False}
        self.device1_ip = None  
        self.t_dwell = None
        self.d_step = None
        self.g_step = None
        self.file_transferred = True
        self.continue_measurement = False
        self.Enable_CUBEXAPI = True
        self.Enable_Bricklets = True
        self.BrickletThreadStarted = False
        self.PowerupBricklets_onetime_initialization  = False
        self.heatertemperature = 0

        self.EnableBricklets_Checkbox = False
        self.EnablePresweep_Checkbox = False
        

        # Calibrations Screen
        self.calib_time = 0.0
        
        # Measurement settings
        self.current_test_code = None
        self.current_pulse_value = "A1 + V1"  
        self.voc_data = [("V1", 0, 0, 0)]  
        self.argon_data = [("A1", 0, 0, 0)] 
        self.T0 = 0 
        self.T1 = 0 
        self.T2 = 0 
        self.T3 = 0
        self.Pre_sweep_delay  = 5  # seconds
        self.Sample_Heat = 0
        self.Coil_Heat = 0
        self.S1V_Heat = 0
        self.S2V_Heat = 0
        self.N2H_Heat = 0
        self.lbl_Sample_Heat_chkbox = 0
        self.lbl_Coil_Heat_chkbox = 0
        self.lbl_Aircube_Heat_chkbox = 0
        self.Flag_Sample_Heat_chkbox = False
        self.Flag_Coil_Heat_chkbox = False
        self.Flag_S1V_Heat_chkbox = False
        self.Flag_S2V_Heat_chkbox = False
        self.Flag_N2H_Heat_chkbox = False       
        self.Flag_Air_quality_chkbox = False
        
        self.should_stop = False 
        # Advance Settings
       
        # Set Sensors
        self.var_Sensor_SN1 = "00000000"
        self.var_Sensor_SN2 = "00000000"
        self.var_Sample_ID1 = "Sample1"
        self.var_Sample_ID2 = "Sample2"

        # Set SWeep
        self.var_vg_min = -1.5
        self.var_vg_max = 2.0
        self.var_vsd = 0.01       
        
        # SWeep
        self.var_t_dwell = 50.0
        self.var_d_step = 0.02
        self.var_g_step = 0.02
        
        # Measure
        self.var_vg_measure = -1.5
        self.var_vsd_measure= 0.01
        self.var_time = 0.0
        
        # Display dI/I
        self.var_name = None
        self.var_y_axis = None

        self.timestamp = 0

        self.current_screen = ""
        self.last_rawtext_file_path = ""
        
        self.Disconnect_Cubedevice = False
        self.device_ip_address = ""
        self.failed_status = ""
        self.measurement_failed = False
        self.Cancel_Event = False
        self.measurement_granted = False
        self.Bricklet_T3Stage_Complete = False
        
        # Sensor Calibration Measurement going on
        self.Sensor_calibration_measurment_goingon = False
        self.Sensor_calibration_measurment_completed = False
        self.Sensor_calibration_Exit_Flag = False
        
        self.shutdown_Flag = False
        
        self.MVP_Device_Version = "0.0.0"
        self.Raspberrypi_Serial_Number = "00000000"

        # BackFlush
        self.BackFlush_Auto = True
        self.BackFlush_ON = False
        self.BackFlush_OFF = False
        self.BackFlush_N2time = 0
        self.BackFlush_Raise_value = 0
        self.BackFlush_lower_value = 0

        self.Vg_Vsd_calibration_mode = True
    
    # getters and setters

    def get_Vg_Vsd_calibration_mode(self):
        with self._lock:
            return self.Vg_Vsd_calibration_mode
    
    def set_Vg_Vsd_calibration_mode(self, val: bool):
        with self._lock:
            self.Vg_Vsd_calibration_mode = val

    def set_var_Sensor_SN1(self, val: str):
        with self._lock:
            self.var_Sensor_SN1 = val     
    
    def get_var_Sensor_SN1(self):
        with self._lock:
            return self.var_Sensor_SN1  

    def set_var_Sensor_SN2(self, val: str):
        with self._lock:
            self.var_Sensor_SN2 = val     
    
    def get_var_Sensor_SN2(self):
        with self._lock:
            return self.var_Sensor_SN2  

    def set_var_Sample_ID1(self, val: str):
        with self._lock:
            self.var_Sample_ID1 = val     
    
    def get_var_Sample_ID1(self):
        with self._lock:
            return self.var_Sample_ID1  

    def set_var_Sample_ID2(self, val: str):
        with self._lock:
            self.var_Sample_ID2 = val     
    
    def get_var_Sample_ID2(self):
        with self._lock:
            return self.var_Sample_ID2  

    def get_var_Sensor_SN2(self):
        with self._lock:
            return self.var_Sensor_SN2  


    def get_BackFlush_Auto(self):
        with self._lock:
            return self.BackFlush_Auto
    
    def set_BackFlush_Auto(self, val: bool):
        with self._lock:
            self.BackFlush_Auto = val
    
    def get_BackFlush_ON(self):
        with self._lock:
            return self.BackFlush_ON
        
    def set_BackFlush_ON(self, val: bool):
        with self._lock:
            self.BackFlush_ON = val        
    
    def get_BackFlush_OFF(self):
        with self._lock:
            return self.BackFlush_OFF
    
    def set_BackFlush_OFF(self, val: bool):
        with self._lock:
            self.BackFlush_OFF = val  

    def get_BackFlush_N2time(self):
        with self._lock:
            return self.BackFlush_N2time
    
    def set_BackFlush_N2time(self, val: int):
        with self._lock:
            self.BackFlush_N2time = val     
    
    def get_BackFlush_Raise_value(self):
        with self._lock:
            return self.BackFlush_Raise_value                      
    
    def set_BackFlush_Raise_value(self, val: float):
        with self._lock:
            self.BackFlush_Raise_value = val
    
    def get_BackFlush_lower_value(self):
        with self._lock:
            return self.BackFlush_lower_value
    
    def set_BackFlush_lower_value(self, val: float):
        with self._lock:
            self.BackFlush_lower_value = val              
    
    def get_Raspberrypi_Serial_Number(self):
         with self._lock:
            return self.Raspberrypi_Serial_Number         

    def set_Raspberrypi_Serial_Number(self, val : str):
         with self._lock:
            self.Raspberrypi_Serial_Number = val
    
    def get_MVP_Device_Version(self):
        with self._lock:
            return self.MVP_Device_Version         

    def set_MVP_Device_Version(self, val : str):
        with self._lock:
            self.MVP_Device_Version = val

    def set_shutdown_Flag(self, val: bool):
        with self._lock:
            self.shutdown_Flag = val

    def get_shutdown_Flag(self):
        with self._lock:
            return self.shutdown_Flag

    def set_PowerupBricklets_onetime_initialization(self, val: bool):
        with self._lock:
            self.PowerupBricklets_onetime_initialization = val

    def get_PowerupBricklets_onetime_initialization(self):
        with self._lock:
            return self.PowerupBricklets_onetime_initialization

    def set_Bricklet_T3Stage_Complete(self,val1: bool):
        with self._lock:
            self.Bricklet_T3Stage_Complete = val1

    def get_Bricklet_T3Stage_Complete(self):
        with self._lock:
            return self.Bricklet_T3Stage_Complete

    def set_measurement_granted(self,val1: bool):
        with self._lock:
            self.measurement_granted = val1

    def get_measurement_granted(self):
        with self._lock:
            return self.measurement_granted  

    def set_Flag_Sample_Heat_chkbox(self,val1: bool):
        with self._lock:
            self.Flag_Sample_Heat_chkbox = val1

    def get_Flag_Sample_Heat_chkbox(self):
        with self._lock:
            return self.Flag_Sample_Heat_chkbox     

    def set_Flag_Coil_Heat_chkbox(self,val1: bool):
        with self._lock:
            self.Flag_Coil_Heat_chkbox = val1

    def get_Flag_Coil_Heat_chkbox(self):
        with self._lock:
            return self.Flag_Coil_Heat_chkbox     

    def set_Flag_Air_quality_chkbox(self,val1: bool):
        with self._lock:
            self.Flag_Air_quality_chkbox = val1

    def get_Flag_Air_quality_chkbox(self):
        with self._lock:
            return self.Flag_Air_quality_chkbox     

    def set_Cancel_event(self,val1: bool):
        with self._lock:
            self.Cancel_Event = val1

    def get_Cancel_event(self):
        with self._lock:
            return self.Cancel_Event         

    def set_failed_status(self, msg : str):
        with self._lock:
            self.failed_status = msg

    def get_failed_status(self):
        with self._lock:
            return self.failed_status
            
    def set_measurement_failed(self, flag: bool):
        with self._lock:
            self.measurement_failed = flag
    
    def get_measurement_failed(self):
        with self._lock:  
            return self.measurement_failed

    def get_device_connection_state(self):
        with self._lock:
            return self.connection_status.get("Device1")

    def set_device_connection_state(self, flag: bool):
        with self._lock:
            self.connection_status["Device1"] = flag

    def set_device_ip_address(self,ipaddr: str):
        with self._lock:
            self.device_ip_address = ipaddr

    def get_device_ip_address(self):
        with self._lock:
            return self.device_ip_address


    def set_Disconnect_Cubedevice(self,val1: int):
        with self._lock:
            self.Disconnect_Cubedevice = val1

    def get_Disconnect_Cubedevice(self):
        with self._lock:
            return self.Disconnect_Cubedevice     
    

    def get_current_screen(self):
        return self.current_screen
    
    def set_current_screen(self, screen_name):
        self.current_screen = screen_name 

    def set_T0(self,val1: int):
        with self._lock:
            self.T0 = val1

    def get_T0(self):
        with self._lock:
            return self.T0 

    def set_T1(self,val: int):
        with self._lock:
            self.T1 = val

    def get_T1(self):
        with self._lock:
            return self.T1     
        
    def set_T2(self,val: int):
        with self._lock:
            self.T2 = val

    def get_T2(self):
        with self._lock:
            return self.T2

    def set_T3(self,val: int):
        with self._lock:
            self.T3 = val

    def get_T3(self):
        with self._lock:
            return self.T3    
        
    def set_timestamp(self,val: str):
        with self._lock:
            self.timestamp = val
            
    def get_timestamp(self):
        with self._lock:
            return self.timestamp   
        
    def set_t_dwell(self,val: float):
        with self._lock:
            self.t_dwell = val

    def get_t_dwell(self):
        with self._lock:
            return self.t_dwell  
        
    def set_var_t_dwell(self, val: float):
        with self._lock:
            self.var_t_dwell = val

    def get_var_t_dwell(self):
        with self._lock:
            return self.var_t_dwell      
        
    def set_d_step(self,val: float):
        with self._lock:
            self.d_step = val
            
    def get_d_step(self):
        with self._lock:
            return self.d_step     
        
    def set_var_d_step(self, val: float):
        with self._lock:
            self.var_d_step = val

    def get_var_d_step(self): 
        with self._lock:
            return self.var_d_step    

    def set_g_step(self,val: float):
        with self._lock:
            self.g_step = val         

    def get_g_step(self):
        with self._lock:
            return self.g_step   
        
    def set_var_g_step(self, val: float):
        with self._lock:
            self.var_g_step = val

    def get_var_g_step(self):
        with self._lock:
            return self.var_g_step       
            
    def set_continue_measurement(self, val: bool):
        with self._lock:
            self.continue_measurement = val

    def get_continue_measurement(self):
        with self._lock:
            return self.continue_measurement    
            
    def set_BrickletThreadStarted(self, val: bool):
        with self._lock:
            self.BrickletThreadStarted = val

    def get_BrickletThreadStarted(self):
        with self._lock:
            return self.BrickletThreadStarted    
        
    def set_file_transferred(self, val: bool):
        with self._lock:
            self.file_transferred = val

    def get_file_transferred(self):
        with self._lock:
            return self.file_transferred    
        
        
    def set_EnableBricklets_Checkbox(self, val: bool):
        with self._lock:
            self.EnableBricklets_Checkbox = val

    def get_EnableBricklets_Checkbox(self):
        with self._lock:        
            return self.EnableBricklets_Checkbox
        
    def set_current_test_code(self, val: str):
        with self._lock:
            self.current_test_code = val

    def get_current_test_code(self):
        with self._lock:
            return self.current_test_code   
        
    def set_current_pulse_value(self, val: str):
        with self._lock:
            self.current_pulse_value = val

    def get_current_pulse_value(self):
        with self._lock:
            return self.current_pulse_value        
             
    def set_voc_data(self, val: list):
        with self._lock:
            self.voc_data = val

    def get_voc_data(self):
        with self._lock:
            return self.voc_data

    def set_argon_data(self, val: list):
        with self._lock:
            self.argon_data = val

    def get_argon_data(self):
        with self._lock:
            return self.argon_data      
        
    # Sensor Calibration Screen
    def set_Sensor_calibration_measurment_goingon(self, val: bool):
        with self._lock:
            self.Sensor_calibration_measurment_goingon = val

    def get_Sensor_calibration_measurment_goingon(self):
        with self._lock:        
            return self.Sensor_calibration_measurment_goingon       
        
    def set_Sensor_calibration_measurment_completed(self, val: bool):
        with self._lock:
            self.Sensor_calibration_measurment_completed = val

    def get_Sensor_calibration_measurment_completed(self):
        with self._lock:        
            return self.Sensor_calibration_measurment_completed   

    def set_Sensor_calibration_Exit_Flag(self, val: bool):
        with self._lock:
            self.Sensor_calibration_Exit_Flag = val

    def get_Sensor_calibration_Exit_Flag(self):
        with self._lock:        
            return self.Sensor_calibration_Exit_Flag                           
          
   
    def updateTvariables_Bricklets(self):
        import integrate
        print("updateTvariables_Bricklets called")
        if integrate.get_Sweep_Measurement() or integrate.get_Normal_Measurement():
            print("get_Sweep_Measurement", integrate.get_Sweep_Measurement())
            print("get_Normal_Measurement", integrate.get_Normal_Measurement())
            # integrate.t1 = 5
            # integrate.t3 = 5
            integrate.set_t1(5)
            integrate.set_t3(5)
        else:
            # integrate.t1 = self.T1
            # integrate.t3 = self.T3
            
            integrate.set_t1(self.get_T1())
            integrate.set_t3(self.get_T3())
            
        if integrate.get_Sweep_Measurement():
            #integrate.t2 = int((0.001 * self.cube.d_time) * ((self.var_vg_max - self.var_vg_min) / self.var_g_step) + 2)
            integrate.set_t2(int((0.001 * self.cube.d_time) * ((self.var_vg_max - self.var_vg_min) / self.var_g_step) + 2))
        elif integrate.get_Normal_Measurement():
            #integrate.t2 = int(float(self.var_time)) 
            integrate.set_t2(int(float(self.var_time))) 
        else:
            #integrate.t2 = self.T2  
            integrate.set_t2(self.get_T2())  

    def is_eth_up(self, interface="eth0"):
        """
        Check if an Ethernet interface is UP using /sys/class/net.

        Returns:
            True  -> Interface is UP
            False -> Interface is DOWN or not available
        """
        operstate_path = f"/sys/class/net/{interface}/operstate"

        try:
            # Check if interface exists
            if not os.path.exists(operstate_path):
                print(f"Interface '{interface}' does not exist.")
                return False

            # Read the current state (e.g., "up", "down", "unknown")
            with open(operstate_path, "r") as f:
                state = f.read().strip()

            return state == "up"

        except Exception as e:
            print(f"Error checking interface state: {e}")
            return False

    """
    Zaid NewREq
    """
    def get_rpi_serial(self):
        try:
            output = subprocess.check_output(
                "awk '/Serial/ {print $3}' /proc/cpuinfo",
                shell=True
            )
            return output.decode().strip()
        except Exception:
            return "Unknown"

    def read_soc_uid(self, file_path="soc_uid.txt"):
        """
        Read soc_uid from local file.

        Args:
            file_path (str): Path to soc_uid file (default: soc_uid.txt)

        Returns:
            str: UID value or "Unknown"
        """

        try:
            with open(file_path, "r", encoding="utf-8") as f:
                uid = f.read().strip()

            # Handle empty file
            if not uid:
                return "Unknown"

            return uid

        except FileNotFoundError:
            print(f"[WARNING] File not found: {file_path}")
            return "Unknown"

        except Exception as e:
            print(f"[ERROR] Failed to read soc_uid: {e}")
            return "Unknown"
