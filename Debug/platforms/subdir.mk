################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../platforms/NocPlatform.cpp 

OBJS += \
./platforms/NocPlatform.o 

CPP_DEPS += \
./platforms/NocPlatform.d 


# Each subdirectory must supply rules for building sources it contributes
platforms/%.o: ../platforms/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


