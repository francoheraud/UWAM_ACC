# UWAM Accumulator Cooling Control
## Brief Overview
The ACC is a board that takes in data from temperature and pressure sensors, and adjusts the cooling fans of the accumulator when the LV system is on. ACC functionality must include:

* Receiving additional segment temperature sensor data from the AMS via CAN.

* Must be able to log all sensor data (i.e. fan RPM, temperatures, pressure values) and power consumption and send it out on the CAN bus.

* Must develop a closed-loop control system that robustly controls the fan speed based off sensor inputs.

# Datasheets:

* Fans:
https://au.mouser.com/datasheet/2/471/San_Ace_60HV38_E-1082928.pdf

* Air Temperature Sensors:
https://4donline.ihs.com/images/VipMasterIC/IC/AMPH/AMPH-S-A0010510552/AMPH-S-A0010543824-1.pdf?hkey=CECEF36DEECDED6468708AAF2E19C0C6
 
* Air Pressure Sensors:
https://4donline.ihs.com/images/VipMasterIC/IC/HWSC/HWSC-S-A0012718244/HWSC-S-A0013884204-1.pdf?hkey=CECEF36DEECDED6468708AAF2E19C0C6

# TODO List:

* Add another ADC input to accept the buck regulator output power signal from the switch circuit for logging the power consumption data.

* Re-implement the main control loop to use the fan tachometer input on top of the temperature inputs to determine the desired pwm/fan speed setpoint value. 
* Maybe use PID for controlling the PWM signal to allow the actual temperature to adhere to the desired temperature?

* Make official firmware documentation (LaTeX doc is preferred).
