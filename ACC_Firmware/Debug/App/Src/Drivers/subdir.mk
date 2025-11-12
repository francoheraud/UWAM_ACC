################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Src/Drivers/can_driver.c \
../App/Src/Drivers/control_system.c \
../App/Src/Drivers/sensor_inputs.c 

OBJS += \
./App/Src/Drivers/can_driver.o \
./App/Src/Drivers/control_system.o \
./App/Src/Drivers/sensor_inputs.o 

C_DEPS += \
./App/Src/Drivers/can_driver.d \
./App/Src/Drivers/control_system.d \
./App/Src/Drivers/sensor_inputs.d 


# Each subdirectory must supply rules for building sources it contributes
App/Src/Drivers/%.o App/Src/Drivers/%.su App/Src/Drivers/%.cyclo: ../App/Src/Drivers/%.c App/Src/Drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F412Rx -c -I../Core/Inc -I"C:/Users/Franc/OneDrive/Documents/GitHub/UWAM_ACC/ACC_Firmware/App/Inc/Drivers" -I"C:/Users/Franc/OneDrive/Documents/GitHub/UWAM_ACC/ACC_Firmware/App/Inc/Other" -IC:/Users/Franc/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/Franc/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/Franc/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/Franc/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I"C:/Users/Franc/OneDrive/Documents/GitHub/UWAM_ACC/ACC_Firmware/App/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Src-2f-Drivers

clean-App-2f-Src-2f-Drivers:
	-$(RM) ./App/Src/Drivers/can_driver.cyclo ./App/Src/Drivers/can_driver.d ./App/Src/Drivers/can_driver.o ./App/Src/Drivers/can_driver.su ./App/Src/Drivers/control_system.cyclo ./App/Src/Drivers/control_system.d ./App/Src/Drivers/control_system.o ./App/Src/Drivers/control_system.su ./App/Src/Drivers/sensor_inputs.cyclo ./App/Src/Drivers/sensor_inputs.d ./App/Src/Drivers/sensor_inputs.o ./App/Src/Drivers/sensor_inputs.su

.PHONY: clean-App-2f-Src-2f-Drivers

