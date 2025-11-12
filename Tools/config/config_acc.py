# note to myself: use python 3.10.14
import yaml
from pathlib import Path

def load_config(path: Path) -> dict:
    with open(path, "r", encoding="utf-8") as f:
        return yaml.safe_load(f)

def generate_header(cfg: dict, out_path: Path) -> None:
    project = cfg["project"]
    mcu     = cfg["hardware"]["mcu"]
    fans    = cfg["hardware"]["fans"]
    can_ids = cfg["communication"]["can_ids"]
    pressure_sensors = cfg["sensors"]["pressure"]
    temp_sensors     = cfg["sensors"]["temperature"]["sensors"]
    control          = cfg["control"]
    num_seg_temp     = cfg["num_seg_temp"]

    with open(out_path, "w", encoding="utf-8") as f:
        f.write("// AUTO-GENERATED FILE. DO NOT EDIT.\n")
        f.write("// Generated from acc_config.yaml\n\n")
        f.write("#pragma once\n\n")

        f.write(f"// Project: {project['name']}\n")
        f.write(f"// Version: {project['version']}\n")
        f.write(f"// Author:  {project['author']}\n")
        f.write(f"// Contributors: {project.get('(contributor(s))', '')}\n\n")

        f.write("#include <math.h>\n")
        f.write("#include <stdio.h>\n")
        f.write("#include <stdint.h>\n")
        f.write("#include <stdlib.h>\n")
        f.write("#include <limits.h>\n")
        f.write("#include <string.h>\n")
        f.write("#include <stdbool.h>\n")
        f.write("#include <limits.h>\n")
        f.write('#include "main.h"\n\n')

        f.write(f"#define ADC_CH_COUNT              {len(temp_sensors)+len(pressure_sensors)}u\n")
        f.write(f"#define ACC_ADC_RES_BITS          {mcu['adc_resolution_bits']}\n")
        f.write(f"#define ACC_ADC_VREF_VOLTS        {mcu['adc_vref']}f\n")
        f.write(f"#define ACC_TIMER_TICK_HZ         {mcu['timer_tick_hz']}u\n")
        f.write(f"#define ACC_FAN_PULSES_PER_REV    {fans['pulses_per_rev']}\n\n")

        f.write("// Status LEDs\n")
        f.write("#define STATUS_LED1                (uint16_t)0x0040\n")
        f.write("#define STATUS_LED2                (uint16_t)0x0080\n")
        f.write("#define STATUS_LED3                (uint16_t)0x0800\n")
        f.write("#define STATUS_LED4                (uint16_t)0x1000\n")
        f.write("#define STATUS_LED5                (uint16_t)0x2000\n\n")

        f.write(f"#define ACC_PWM_RESOLUTION        {fans['pwm']['resolution']}u\n\n")


        f.write("// CAN IDs\n")
        for name, value in can_ids.items():
            macro = f"ACC_CAN_ID_{name.upper()}"
            f.write(f"#define {macro:24s}  0x{value:03X}u\n")
        f.write("\n")

        f.write("// Pressure sensors\n")
        f.write(f"#define ACC_PRESSURE_SENSOR_COUNT {len(pressure_sensors)}u\n\n")
        for idx, p in enumerate(pressure_sensors):
            base = f"ACC_PRESS_{p['name'].upper()}"
            f.write(f"#define {base}_ADC_CH         {p['adc_channel']}u\n")

        f.write("// Temperature sensors\n")
        f.write(f"#define ACC_TEMP_SENSOR_COUNT     {len(temp_sensors)}u\n\n")
        for t in temp_sensors:
            base = f"ACC_TEMP_{t['name'].upper()}"
            f.write(f"#define {base}_ADC_CH         {t['adc_channel']}u\n")

        f.write("\n// Control thresholds\n")
        f.write(f"#define ACC_TEMP_THRESHOLD1_DEGC  {control['temp_threshold1']}.0f\n")
        f.write(f"#define ACC_TEMP_THRESHOLD2_DEGC  {control['temp_threshold2']}.0f\n")
        f.write(f"#define ACC_TEMP_HYSTERESIS_DEGC  {control['hysteresis_degC']}.0f\n\n")

        f.write(f"#define ACC_NUM_SEG_TEMPS         {num_seg_temp}u\n\n")

        f.write("// End\n")

def main():
    in_yaml     = Path("acc_config.yaml")
    script_path = Path(__file__).resolve()
    project_path = script_path.parent.parent.parent
    out_header  = Path(f"{project_path}/ACC_Firmware/App/Inc/Other/acc_config.h")
    
    cfg = load_config(in_yaml)
    generate_header(cfg, out_header)
    print(f"Generated {out_header} from {in_yaml}")

if __name__ == "__main__":
    main()
