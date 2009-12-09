################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../models/onchipcommunication/node/BusDelayModel.cpp \
../models/onchipcommunication/node/OrionAreaModel.cpp \
../models/onchipcommunication/node/OrionPowerModel.cpp 

OBJS += \
./models/onchipcommunication/node/BusDelayModel.o \
./models/onchipcommunication/node/OrionAreaModel.o \
./models/onchipcommunication/node/OrionPowerModel.o 

CPP_DEPS += \
./models/onchipcommunication/node/BusDelayModel.d \
./models/onchipcommunication/node/OrionAreaModel.d \
./models/onchipcommunication/node/OrionPowerModel.d 


# Each subdirectory must supply rules for building sources it contributes
models/onchipcommunication/node/%.o: ../models/onchipcommunication/node/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


