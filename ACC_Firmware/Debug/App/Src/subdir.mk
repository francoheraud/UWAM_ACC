################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Src/can_driver.c \
../App/Src/control_system.c \
../App/Src/sensor_inputs.c 

OBJS += \
./App/Src/can_driver.o \
./App/Src/control_system.o \
./App/Src/sensor_inputs.o 

C_DEPS += \
./App/Src/can_driver.d \
./App/Src/control_system.d \
./App/Src/sensor_inputs.d 


# Each subdirectory must supply rules for building sources it contributes
App/Src/%.o App/Src/%.su App/Src/%.cyclo: ../App/Src/%.c App/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F412Rx -c -I../Core/Inc -IC:/Users/Franc/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/Franc/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/Franc/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/Franc/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I"C:/Users/Franc/OneDrive/Documents/stm32_proj_plz/ACC_Firmware_2/App/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Src

clean-App-2f-Src:
	-$(RM) ./App/Src/can_driver.cyclo ./App/Src/can_driver.d ./App/Src/can_driver.o ./App/Src/can_driver.su ./App/Src/control_system.cyclo ./App/Src/control_system.d ./App/Src/control_system.o ./App/Src/control_system.su ./App/Src/sensor_inputs.cyclo ./App/Src/sensor_inputs.d ./App/Src/sensor_inputs.o ./App/Src/sensor_inputs.su

.PHONY: clean-App-2f-Src

