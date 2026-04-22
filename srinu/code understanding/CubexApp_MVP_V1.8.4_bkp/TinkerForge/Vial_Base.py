"""
Vial Base Unit test application.
These APIs can be reused for testing the Vial Base which
can be called from some other file.

UID : 2akw
"""

import time
import integrate
from tinkerforge.bricklet_silent_stepper_v2 import BrickletSilentStepperV2
from TinkerForge.tforge_con import create_connection
from TinkerForge.Bricklet_ID_Configuration import (
    Vial_Base_UID,
    VIAL_MOTOR_CONFIG,
    VIAL_UPPER_LIMIT,
    VIAL_LOWER_LIMIT,
    VIAL_1_3RD_POSITION,
    VIAL_STEP_RESOLUTION
)


# ====================================================
# INITIALIZATION
# ====================================================

stepper = None
def vial_base_initialize_stepper(ipcon):
    global stepper
    if stepper is not None:
        return stepper    
    stepper = BrickletSilentStepperV2(Vial_Base_UID, ipcon)
    vial_base_configure_stepper()
    integrate.set_vial_base(stepper)
    print("Vial Base Stepper Initialized")
    return stepper


def vial_base_configure_stepper():
    global stepper
    #stepper.set_step_configuration(BrickletSilentStepperV2.STEP_RESOLUTION_128, 0)
    try:
        resolution = getattr(BrickletSilentStepperV2,f"STEP_RESOLUTION_{VIAL_STEP_RESOLUTION}")
        print(f"Configuring step resolution: {VIAL_STEP_RESOLUTION} ({resolution})")
    except AttributeError:
        raise ValueError(f"Invalid step resolution: {VIAL_STEP_RESOLUTION}")
    stepper.set_step_configuration(resolution, 0)
    stepper.set_motor_current(VIAL_MOTOR_CONFIG["run_current"])
    stepper.set_max_velocity(VIAL_MOTOR_CONFIG["max_velocity"])
    stepper.set_speed_ramping(VIAL_MOTOR_CONFIG["acceleration"],VIAL_MOTOR_CONFIG["deceleration"])
    print("Stepper motor configured.")


# ====================================================
# VIAL STATUS
# ====================================================
def vial_base_status():
    global stepper
    status = stepper.get_enabled()
    return status

# ====================================================
# VIAL OFF
# ====================================================
def vial_base_off():
    global stepper
    stepper.stop()
    stepper.set_enabled(False)
    print("vial base is turned off")

# ====================================================
# APP START STEPPER TO ZERO
# ====================================================
def vial_start_home():
    global stepper
    stepper.set_enabled(True)
    home_step = abs(VIAL_UPPER_LIMIT)
    stepper.set_steps(home_step)
    while True:
        gpio0_state, _ = stepper.get_gpio_state()
        if not gpio0_state:
            #stepper.stop()
            #while stepper.get_remaining_steps() != 0:
            #    time.sleep(0.01) 
            print(f"Gpio 0-state {gpio0_state}")
            stepper.set_enabled(False)
            stepper.set_current_position(0)
            print("vial position set to zero")
            break

    time.sleep(0.2)
    stepper.set_enabled(False)

# ====================================================
# POSITION CHECK
# ====================================================

def vial_base_is_home_position():
    global stepper
    gpio0_state, _ = stepper.get_gpio_state()

    if gpio0_state is True:
        print("ERROR: GPIO state is None! Check connection.")
        return False

    return not gpio0_state

# ====================================================
# POSITION state
# ====================================================
def vial_base_position():
    global stepper
    position = stepper.get_current_position() 
    return position

# ====================================================
# BASIC MOVEMENT
# ====================================================

def vial_base_move_stepper(target_position):
    global stepper
    if target_position > VIAL_UPPER_LIMIT or target_position < VIAL_LOWER_LIMIT:
        print(
            f"ERROR: Position out of range "
            f"({VIAL_LOWER_LIMIT} to {VIAL_UPPER_LIMIT})!"
        )
        return

    stepper.set_enabled(True)
    stepper.set_target_position(target_position)

    while stepper.get_current_position() != stepper.get_target_position():
        time.sleep(0.1)

    stepper.set_enabled(False)

    print(
        f"Movement complete. "
        f"Current Position: {stepper.get_current_position()} steps."
    )


def vial_base_home_stepper():
    global stepper
    print("Starting Homing Process...")

    if vial_base_is_home_position(stepper):
        stepper.set_current_position(0)
        print("Already Reached Home Position.")
        return

    stepper.set_enabled(True)
    stepper.set_target_position(0)

    while stepper.get_current_position() != stepper.get_target_position():
        if vial_base_is_home_position(stepper):
            stepper.set_current_position(0)
            break
        time.sleep(0.1)

    stepper.set_enabled(False)

    if vial_base_is_home_position(stepper):
        stepper.set_current_position(0)
        print("Reached Home Position.")
    else:
        print("Not in home position.")


# ====================================================
# NEW FUNCTION 1
# ====================================================

def vial_base_measurement(timeout=10):
    """
    Moves vial base to upper limit for measurement
    with timeout protection.
    """
    global stepper
    print("Starting Vial Base Measurement...")


    current_position = stepper.get_current_position()
    print(f"Current Position: {current_position}")

    if not vial_base_is_home_position(stepper):
        if current_position > VIAL_UPPER_LIMIT:
            print("ERROR: Current position beyond MAX limit.")
            return False

    stepper.set_enabled(True)
    stepper.set_target_position(VIAL_UPPER_LIMIT)

    start_time = time.time()

    while stepper.get_current_position() < VIAL_UPPER_LIMIT:

        if (time.time() - start_time) > timeout:
            print("ERROR: Measurement move timeout.")
            stepper.set_enabled(False)
            return False

        time.sleep(0.05)

    stepper.set_enabled(False)
    print("Measurement position reached successfully.")
    return True



def vial_base_raise(MAX_limit):
    """
    Raises vial base to given MAX_limit safely.
    """
    global stepper
    print(f"Raising vial base to {MAX_limit} steps.")
    print(f" Resolution : {VIAL_STEP_RESOLUTION}")

    
    stepper.set_enabled(True)
    stepper.set_target_position(MAX_limit)

    start_time = time.time()

    while stepper.get_current_position() != stepper.get_target_position():
        print("Current Position", stepper.get_current_position())
        print("Target Position", stepper.get_target_position())        
        
        if integrate.get_Appclose():
            print("<TT> Temperature : Application closed")
            integrate.set_Vial_Thread_Closed(True)
            break
        
        stop = integrate.get_stop_vial_Base()
        print(f"vial stop or not in raise: {stop}")
        if stop:
            vial_base_off()
            print("Vial Base stopped while raising")
            break

        time.sleep(0.5)
        current_pos = stepper.get_current_position()
        print("Current Position", current_pos)
        integrate.set_steps_count(current_pos)
        print("get Steps Count", integrate.get_steps_count())
        #integrate.update_steps.Update_steps_count_signal.emit(integrate.get_steps_count())
        #temperature, short_a, short_b, open_a, open_b = stepper.get_driver_status()
        #print(f"temp = {temperature}, short_a = {short_a}, short_b = {short_b}, open_a = {open_a}, open_b = {open_b}")
        
        if current_pos < VIAL_UPPER_LIMIT:
            print("ERROR: Upper limit exceeded!")
            stepper.stop()
            stepper.set_enabled(False)
            return False

        # timeout check
        if time.time() - start_time >= 90:
            print("90 seconds timeout reached")
            return False

    stepper.stop()
    stepper.set_enabled(False)
    print("Vial base raised successfully.")
    return True


def vial_base_lower(MIN_limit=VIAL_LOWER_LIMIT, timeout=10):
    """
    Lowers vial base safely to given MIN_limit.
    Includes lower bound check and timeout protection.
    """
    global stepper
    print(f"Lowering vial base to {MIN_limit} steps.")


    current_position = stepper.get_current_position()

    # Safety: Check user given limit
    if MIN_limit < VIAL_LOWER_LIMIT:
        print(f"ERROR: MIN_limit below allowed lower limit ({VIAL_LOWER_LIMIT})")
        return False

    stepper.set_enabled(True)
    stepper.set_target_position(MIN_limit)

    start_time = time.time()

    while stepper.get_current_position() != stepper.get_target_position():

        if integrate.get_Appclose():
            print("<TT> Temperature : Application closed")
            integrate.set_Vial_Thread_Closed(True)
            break        
        
        stop = integrate.get_stop_vial_Base()
        print(f"vial stop or not in lower: {stop}")      
        if stop:
            vial_base_off()
            print("Vial Base stopped while lowering")

            break

        time.sleep(0.5)
        current_pos = stepper.get_current_position()
        print("Current Position", current_pos)
        integrate.set_steps_count(current_pos)
        print("get Steps Count", integrate.get_steps_count())
        #integrate.update_steps.Update_steps_count_signal.emit(integrate.get_steps_count())
        gpio_0, gpio_1 = stepper.get_gpio_state()
        print(f"Vial GPIO_0 = {gpio_0}, GPIO_1 ={gpio_1}")
        if gpio_0 == False:
            print("GPIO_0 Reached Low")
            if current_pos > VIAL_LOWER_LIMIT:
                print("ERROR: Lower limit exceeded!")    
                break        
            stepper.set_enabled(False)
            time.sleep(0.2)
            stepper.set_current_position(0)
            print("current position set to Zero")
            current_pos = stepper.get_current_position()
            print(f"set position after gpio is {current_pos}")
            break

        #temperature, short_a, short_b, open_a, open_b = stepper.get_driver_status()
        #print(f"temp = {temperature}, short_a = {short_a}, short_b = {short_b}, open_a = {open_a}, open_b = {open_b}")


        if current_pos > VIAL_LOWER_LIMIT:
            print("ERROR: Lower limit exceeded!")
            stepper.stop()
            stepper.set_current_position(0)
            stepper.set_enabled(False)
            return False

        # timeout check
        if time.time() - start_time >= 90:
            print("90 seconds timeout reached")
            return False

        time.sleep(0.1)

    stepper.stop()
    stepper.set_enabled(False)

    print("Vial base lowered successfully.")
    return True


# ====================================================
# USER MENU
# ====================================================

def vial_base_user_menu():
    global stepper
    while True:
        print("\nVial Base Movement Options:")
        print("1. Move to Position")
        print("2. Home")
        print("3. Raise (Custom MAX)")
        print("4. Measurement (Auto MAX)")
        print("5. Exit")

        choice = input("Enter your choice (1/2/3/4/5): ")
        print(f"Current Position: {stepper.get_current_position()}")

        if choice == "1":
            pos = int(input("Enter target position: "))
            vial_base_move_stepper(stepper, pos)

        elif choice == "2":
            vial_base_home_stepper(stepper)

        elif choice == "3":
            max_limit = int(input("Enter MAX limit: "))
            vial_base_raise(stepper, max_limit)

        elif choice == "4":
            vial_base_measurement(stepper)

        elif choice == "5":
            print("Exiting program.")
            break

        else:
            print("Invalid choice!")


# ====================================================
# MAIN
# ====================================================

def vial_base_main():
    ip_con = create_connection()
    stepper = vial_base_initialize_stepper(ip_con)
    vial_base_user_menu(stepper)
    ip_con.disconnect()
    print("Vial Base Control Completed.")


if __name__ == "__main__":
    vial_base_main()

