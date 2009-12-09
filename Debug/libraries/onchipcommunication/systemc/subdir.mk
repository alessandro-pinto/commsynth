################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libraries/onchipcommunication/systemc/Technology.cpp 

OBJS += \
./libraries/onchipcommunication/systemc/Technology.o 

CPP_DEPS += \
./libraries/onchipcommunication/systemc/Technology.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/onchipcommunication/systemc/%.o: ../libraries/onchipcommunication/systemc/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


