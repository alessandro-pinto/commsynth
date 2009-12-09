################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../models/onchipcommunication/link/HoAreaModel.cpp \
../models/onchipcommunication/link/HoDelayModel.cpp \
../models/onchipcommunication/link/HoPowerModel.cpp 

OBJS += \
./models/onchipcommunication/link/HoAreaModel.o \
./models/onchipcommunication/link/HoDelayModel.o \
./models/onchipcommunication/link/HoPowerModel.o 

CPP_DEPS += \
./models/onchipcommunication/link/HoAreaModel.d \
./models/onchipcommunication/link/HoDelayModel.d \
./models/onchipcommunication/link/HoPowerModel.d 


# Each subdirectory must supply rules for building sources it contributes
models/onchipcommunication/link/%.o: ../models/onchipcommunication/link/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


