# note to myself: use python 3.10.14
import yaml
from pathlib import Path

def load_config(path: Path) -> dict:
    with open(path, "r", encoding="utf-8") as f:
        return yaml.safe_load(f)

def generate_header(cfg: dict, out_path: Path) -> None:
    """Generate a C header file from the config."""
    project = cfg["project"]
    mcu     = cfg["hardware"]["mcu"]
    fans    = cfg["hardware"]["fans"]
    can_ids = cfg["communication"]["can_ids"]
    pressure_sensors = cfg["sensors"]["pressure"]
    temp_common      = cfg["sensors"]["temperature"]["common"]
    temp_sensors     = cfg["sensors"]["temperature"]["sensors"]
    control          = cfg["control"]

    with open(out_path, "w", encoding="utf-8") as f:
        f.write("// AUTO-GENERATED FILE. DO NOT EDIT.\n")
        f.write("// Generated from acc_config.yaml\n\n")
        f.write("#pragma once\n\n")

        f.write(f"// Project: {project['name']}\n")
        f.write(f"// Version: {project['version']}\n")
        f.write(f"// Author:  {project['author']}\n")
        f.write(f"// Contributors: {project.get('contributor(s)', '')}\n\n")

        f.write(f"#define ACC_ADC_RES_BITS      {mcu['adc_resolution_bits']}\n")
        f.write(f"#define ACC_ADC_VREF_VOLTS    {mcu['adc_vref']}f\n")
        f.write(f"#define ACC_TIMER_TICK_HZ     {mcu['timer_tick_hz']}u\n\n")

        f.write(f"#define ACC_FAN_COUNT         {fans['count']}\n")
        f.write(f"#define ACC_FAN_PULSES_PER_REV {fans['pulses_per_rev']}\n")
        f.write(f"#define ACC_FAN_PWM_MIN_DUTY  {fans['pwm']['min_duty']}f\n")
        f.write(f"#define ACC_FAN_PWM_MAX_DUTY  {fans['pwm']['max_duty']}f\n\n")

        f.write("// CAN IDs\n")
        for name, value in can_ids.items():
            macro = f"ACC_CAN_ID_{name.upper()}"
            f.write(f"#define {macro:24s}  0x{value:03X}u\n")
        f.write("\n")

        f.write("// Pressure sensors\n")
        f.write(f"#define ACC_PRESSURE_SENSOR_COUNT  {len(pressure_sensors)}u\n\n")
        for idx, p in enumerate(pressure_sensors):
            base = f"ACC_PRESS_{p['name'].upper()}"
            f.write(f"#define {base}_ADC_CH       {p['adc_channel']}u\n")
            f.write(f"#define {base}_MIN_PSI       {p['min_psi']}f\n")
            f.write(f"#define {base}_MAX_PSI       {p['max_psi']}f\n\n")

        coeffs = temp_common["coefficients"]
        f.write("// Thermistor (Steinhart-Hart) coefficients (common)\n")
        f.write(f"#define ACC_TH_A   {coeffs['a']}f\n")
        f.write(f"#define ACC_TH_B   {coeffs['b']}f\n")
        f.write(f"#define ACC_TH_C   {coeffs['c']}f\n")
        f.write(f"#define ACC_TH_D   {coeffs['d']}f\n\n")

        f.write(f"#define ACC_TEMP_SENSOR_COUNT       {len(temp_sensors)}u\n\n")
        for t in temp_sensors:
            base = f"ACC_TEMP_{t['name'].upper()}"
            f.write(f"#define {base}_ADC_CH       {t['adc_channel']}u\n")

        f.write("\n// Control thresholds\n")
        f.write(f"#define ACC_TEMP_THRESHOLD1_DEGC   {control['temp_threshold1']}f\n")
        f.write(f"#define ACC_TEMP_THRESHOLD2_DEGC   {control['temp_threshold2']}f\n")
        f.write(f"#define ACC_TEMP_HYSTERESIS_DEGC   {control['hysteresis_degC']}f\n")

def main():
    in_yaml     = Path("acc_config.yaml")
    script_path = Path(__file__).resolve()
    project_path = script_path.parent.parent
    out_header  = Path(f"{project_path}/ACC_Firmware/App/Inc/acc_config.h")
    
    cfg = load_config(in_yaml)
    generate_header(cfg, out_header)
    print(f"Generated {out_header} from {in_yaml}")

if __name__ == "__main__":
    main()
