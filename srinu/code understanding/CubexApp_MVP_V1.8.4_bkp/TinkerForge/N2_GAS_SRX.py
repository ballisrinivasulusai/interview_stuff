from tinkerforge.bricklet_performance_dc import BrickletPerformanceDC
from TinkerForge.tforge_con import create_connection
from TinkerForge.Bricklet_ID_Configuration import N2_GAS_UID, N2_GAS_FREQ_STEP, N2_GAS_PWM_FREQUENCY, N2_GAS_ACCELERATION, N2_GAS_DECELERATION, N2_GAS_VELOCITY

import time

def n2_gas_initialize_stepper(ipcon):
    """
    Initializes the BrickletPerformanceDC Bricklet.

    Args:
        ipcon (IPConnection): Tinkerforge IP connection instance.
        uid (str): Unique ID of the stepper motor.

    Returns:
        BrickletPerformanceDC: The initialized stepper instance.
    """
    pdc = BrickletPerformanceDC(N2_GAS_UID, ipcon)
    return pdc
    
# Function to set velocity
def n2_gas_set_velocity(pdc, velocity):
    pdc.set_velocity(velocity)  
    print(f"Velocity set to {velocity}")

# Function to set acceleration and deceleration
def n2_gas_set_acceleration(pdc, acceleration, deceleration):
    pdc.set_motion(acceleration, deceleration)
    print(f"Acceleration set to {acceleration}, Deceleration set to {deceleration}")

# Function to set PWM frequency
def n2_gas_set_pwm_frequency(pdc, frequency):
    pdc.set_pwm_frequency(frequency)
    print(f"PWM frequency set to {frequency} Hz")

# Function to handle user options
def user_control(pdc):
    max_pwm = 32767
    min_pwm = 0
    current_pwm = pdc.get_current_velocity()
    while True:
        print("\nUSER OPTIONS:")
        print("1. Increase Velocity")
        print("2. Decrease Velocity")
        print("3. OFF the PWM")
        print("4. Exit")

        choice = input("Enter your choice: ")

        if choice == "1":
            if max_pwm > current_pwm:
                current_pwm += N2_GAS_FREQ_STEP
                n2_gas_set_velocity(pdc, current_pwm)
            else:
                print("PWM Reached Maximum velocity")

        elif choice == "2":
            if min_pwm < current_pwm:
                current_pwm -= N2_GAS_FREQ_STEP
                if current_pwm <= 0:
                    current_pwm = 0
                n2_gas_set_velocity(pdc, current_pwm)
            else:
                print("PWM Reached Minimum velocity")

        elif choice == "3":
            n2_gas_set_velocity(pdc, 0)
            pdc.set_enabled(False)
            print("PWM turned OFF.")

        elif choice == "4":
            print("Exiting...")
            break

        else:
            print("Invalid choice! Please try again.")

# Main function to control motor
def main():
    ip_con = create_connection()
    
    # Initialize Performance DC Bricklet
    pdc = n2_gas_initialize_stepper(ip_con)
    
    # Set PWM frequency
    n2_gas_set_pwm_frequency(pdc, N2_GAS_PWM_FREQUENCY)

    # Set acceleration and deceleration
    n2_gas_set_acceleration(pdc, N2_GAS_ACCELERATION, N2_GAS_DECELERATION)

    # Set velocity (positive for forward, negative for reverse)
    n2_gas_set_velocity(pdc, N2_GAS_VELOCITY)

    # Enable motor
    pdc.set_enabled(True)
    print("Motor enabled.")

    # User control
    user_control(pdc)

    # Disable motor after stopping
    pdc.set_enabled(False)
    print("Motor stopped and disabled.")

    # Disconnect
    ip_con.disconnect()

if __name__ == "__main__":
    main()

