################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libraries/onchipcommunication/link/PointToPointBus.cpp 

OBJS += \
./libraries/onchipcommunication/link/PointToPointBus.o 

CPP_DEPS += \
./libraries/onchipcommunication/link/PointToPointBus.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/onchipcommunication/link/%.o: ../libraries/onchipcommunication/link/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


