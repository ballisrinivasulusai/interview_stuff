import sys
import psutil
import os
from pathlib import Path
from PyQt5.QtCore import QTimer
from PyQt5.QtWidgets import (
    QApplication,
    QDialog,
    QVBoxLayout,
    QRadioButton,
    QDialogButtonBox,
    QLabel,
)
from SharedState import SharedState, APP_VERSION
from datetime import datetime
import atexit

# ---------------- App Info ----------------

APP_NAME = "CubexApp"
print("Application starting...")

# ---------------- Qt Application ----------------

app = QApplication(sys.argv)

# ---------------- MVP Selection Logic ----------------

versions = ["MVP_1.0", "MVP_1.5"]
selected_mvp = None
# Check command line argument
if len(sys.argv) > 1:
    arg_version = sys.argv[1].strip()
    if arg_version == "1.0" or arg_version == "1.5":
        selected_mvp = f"MVP_{arg_version}"
        print(f"Selected via CLI argument: {selected_mvp}")
    else:
        print(f"Invalid argument '{arg_version}'. Falling back to dialog.")

# If not passed or invalid → show dialog
if selected_mvp is None:
    dialog = QDialog()
    dialog.setWindowTitle("Select MVP Device")
    dialog.setModal(True)

    layout = QVBoxLayout()

    label = QLabel("Please select the MVP device version:")
    layout.addWidget(label)

    radio_buttons = []
    for version in versions:
        radio = QRadioButton(version)
        layout.addWidget(radio)
        radio_buttons.append(radio)

    # Default selection
    radio_buttons[0].setChecked(True)

    button_box = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
    button_box.accepted.connect(dialog.accept)
    button_box.rejected.connect(dialog.reject)
    layout.addWidget(button_box)

    dialog.setLayout(layout)

    result = dialog.exec_()

    # If dialog cancelled → exit
    if result != QDialog.Accepted:
        print("MVP selection dialog cancelled. Exiting application.")
        sys.exit(0)

    # Get selected value
    for i, radio in enumerate(radio_buttons):
        if radio.isChecked():
            selected_mvp = versions[i]
            break

    print(f"User selected: {selected_mvp}")

# ---------------- Final Version ----------------

selected_version = f"{APP_NAME}_{APP_VERSION}_{selected_mvp}"
print("Selected MVP Device Version:", selected_version)

# ---------------- Logging Setup ----------------

SCRIPT_DIR = Path(__file__).parent.absolute()
BASE_LOG_DIR = SCRIPT_DIR / "Logfiles"

TODAY_DATE = datetime.now().strftime("%d_%m_%Y")
TODAY_FOLDER = os.path.join(BASE_LOG_DIR, TODAY_DATE)
os.makedirs(TODAY_FOLDER, exist_ok=True)

LOG_START_TIME = datetime.now()
LOG_TIME_STR = LOG_START_TIME.strftime("%Y%m%d_%H%M%S")

TMP_LOG_PATH = os.path.join(TODAY_FOLDER, f"tmp_Log_{LOG_TIME_STR}.txt")
FINAL_LOG_PATH = os.path.join(TODAY_FOLDER, f"Log_{LOG_TIME_STR}.txt")

log_file = open(TMP_LOG_PATH, "a", buffering=1)
sys.stdout = log_file
sys.stderr = log_file

current_log_start_time = LOG_START_TIME
current_tmp_log = TMP_LOG_PATH
current_final_log = FINAL_LOG_PATH

def close_logfile():
    try:
        log_file.flush()
        log_file.close()
        if os.path.exists(current_tmp_log):
            os.rename(current_tmp_log, current_final_log)
    except Exception:
        pass

atexit.register(close_logfile)

print("Logging started at:", LOG_START_TIME)

# ---------------- App Start ----------------

APP_START_TIME = datetime.now()
print("Application started at:", APP_START_TIME)

# ---------------- Shared State ----------------

shared_state = SharedState()
shared_state.set_MVP_Device_Version(selected_version)

app.setProperty("App_Version", selected_version)

# ---------------- Load Brick Config ----------------

from TinkerForge.Bricklet_ID_Configuration import Bricklet_ID_Config
brick_config = Bricklet_ID_Config()

# ---------------- Main Window ----------------

from MainWindow import MainWindow
main_window = MainWindow()
main_window.showMaximized()
main_window.setWindowTitle(selected_version)
main_window.show()

# ---------------- Memory Monitor ----------------

timer = QTimer()

def memory_usage():
    global log_file, current_log_start_time
    global current_tmp_log, current_final_log

    process = psutil.Process(os.getpid())
    mem = process.memory_info().rss / (1024 * 1024)
    print(f"Memory usage: {mem:.2f} MB")

    elapsed = (datetime.now() - current_log_start_time).total_seconds()

    # Rotate log every 30 minutes
    if elapsed >= 1800:
        try:
            log_file.close()
            if os.path.exists(current_tmp_log):
                os.rename(current_tmp_log, current_final_log)

            new_start = datetime.now()
            new_time = new_start.strftime("%Y%m%d_%H%M%S")

            current_tmp_log = os.path.join(TODAY_FOLDER, f"tmp_Log_{new_time}.txt")
            current_final_log = os.path.join(TODAY_FOLDER, f"Log_{new_time}.txt")

            log_file = open(current_tmp_log, "a", buffering=1)
            sys.stdout = log_file
            sys.stderr = log_file

            current_log_start_time = new_start
            print("New log started at:", new_start)

        except Exception as e:
            print("Log rotation failed:", e)

timer.timeout.connect(memory_usage)
timer.start(5000)

# ---------------- Run App ----------------

sys.exit(app.exec_())