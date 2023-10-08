################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../S&M_Driv/src/ST7789_low_level.c \
../S&M_Driv/src/fonts.c 

OBJS += \
./S&M_Driv/src/ST7789_low_level.o \
./S&M_Driv/src/fonts.o 

C_DEPS += \
./S&M_Driv/src/ST7789_low_level.d \
./S&M_Driv/src/fonts.d 


# Each subdirectory must supply rules for building sources it contributes
S&M_Driv/src/%.o: ../S&M_Driv/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@S&M_Driv/src/ST7789_low_level.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


