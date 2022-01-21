################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/delay.c \
../Src/led.c \
../Src/main.c \
../Src/uart_driver.c 

OBJS += \
./Src/delay.o \
./Src/led.o \
./Src/main.o \
./Src/uart_driver.o 

C_DEPS += \
./Src/delay.d \
./Src/led.d \
./Src/main.d \
./Src/uart_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F4 -DSTM32F446RETx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/delay.d ./Src/delay.o ./Src/led.d ./Src/led.o ./Src/main.d ./Src/main.o ./Src/uart_driver.d ./Src/uart_driver.o

.PHONY: clean-Src

