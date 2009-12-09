################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../io/common/ConfigParser.cpp 

OBJS += \
./io/common/ConfigParser.o 

CPP_DEPS += \
./io/common/ConfigParser.d 


# Each subdirectory must supply rules for building sources it contributes
io/common/%.o: ../io/common/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


