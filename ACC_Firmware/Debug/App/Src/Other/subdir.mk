################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Src/Other/scheduler.c 

OBJS += \
./App/Src/Other/scheduler.o 

C_DEPS += \
./App/Src/Other/scheduler.d 


# Each subdirectory must supply rules for building sources it contributes
App/Src/Other/%.o App/Src/Other/%.su App/Src/Other/%.cyclo: ../App/Src/Other/%.c App/Src/Other/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F412Rx -c -I../Core/Inc -I"C:/Users/Franc/OneDrive/Documents/GitHub/UWAM_ACC/ACC_Firmware/App/Inc/Drivers" -I"C:/Users/Franc/OneDrive/Documents/GitHub/UWAM_ACC/ACC_Firmware/App/Inc/Other" -IC:/Users/Franc/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/Franc/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/Franc/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/Franc/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I"C:/Users/Franc/OneDrive/Documents/GitHub/UWAM_ACC/ACC_Firmware/App/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Src-2f-Other

clean-App-2f-Src-2f-Other:
	-$(RM) ./App/Src/Other/scheduler.cyclo ./App/Src/Other/scheduler.d ./App/Src/Other/scheduler.o ./App/Src/Other/scheduler.su

.PHONY: clean-App-2f-Src-2f-Other

