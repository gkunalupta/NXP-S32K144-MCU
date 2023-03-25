################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../EXT_Driv/src/GB_MFRC522.c 

OBJS += \
./EXT_Driv/src/GB_MFRC522.o 

C_DEPS += \
./EXT_Driv/src/GB_MFRC522.d 


# Each subdirectory must supply rules for building sources it contributes
EXT_Driv/src/%.o: ../EXT_Driv/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@EXT_Driv/src/GB_MFRC522.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


