################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libs/l2hal/drivers/display/gc9a01_local_framebuffer/src/l2hal_gc9a01_lfb.c 

C_DEPS += \
./libs/l2hal/drivers/display/gc9a01_local_framebuffer/src/l2hal_gc9a01_lfb.d 

OBJS += \
./libs/l2hal/drivers/display/gc9a01_local_framebuffer/src/l2hal_gc9a01_lfb.o 


# Each subdirectory must supply rules for building sources it contributes
libs/l2hal/drivers/display/gc9a01_local_framebuffer/src/%.o: ../libs/l2hal/drivers/display/gc9a01_local_framebuffer/src/%.c libs/l2hal/drivers/display/gc9a01_local_framebuffer/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -flto -Wall -Wextra -g -DNDEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


