################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Base directory for application code
APP_SRC_DIR := ../Application_Source

C_SRCS += \
../Application_Source/main.c \
../Application_Source/drivers/gpio/gpio.c \
../Application_Source/drivers/clock/clock.c \
../Application_Source/drivers/adc/adc.c \
../Application_Source/drivers/timers/timer.c \
../Application_Source/drivers/uart/uart.c \
../Application_Source/middleware/button/button.c \
../Application_Source/middleware/ldr/ldr.c \
../Application_Source/middleware/motor/motor.c \
../Application_Source/app/app_logic.c

# ------------------------------------------------------------------------------
# Object files
# ------------------------------------------------------------------------------
OBJS += \
Application_Source/main.o \
Application_Source/drivers/gpio/gpio.o \
Application_Source/drivers/clock/clock.o \
Application_Source/drivers/adc/adc.o \
Application_Source/drivers/timers/timer.o \
Application_Source/drivers/uart/uart.o \
Application_Source/middleware/button/button.o \
Application_Source/middleware/ldr/ldr.o \
Application_Source/middleware/motor/motor.o \
Application_Source/app/app_logic.o

# Each subdirectory must supply rules for building sources it contributes
Application_Source/%.o: ../Application_Source/%.c
	@echo 'Building file: $^'
	@echo 'Invoking: ARM-GCC C Compiler'
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^  
	@echo 'Finished building: $^'

