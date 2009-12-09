################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../models/onchipcommunication/common/AreaModel.cpp \
../models/onchipcommunication/common/DelayModel.cpp \
../models/onchipcommunication/common/PowerModel.cpp 

OBJS += \
./models/onchipcommunication/common/AreaModel.o \
./models/onchipcommunication/common/DelayModel.o \
./models/onchipcommunication/common/PowerModel.o 

CPP_DEPS += \
./models/onchipcommunication/common/AreaModel.d \
./models/onchipcommunication/common/DelayModel.d \
./models/onchipcommunication/common/PowerModel.d 


# Each subdirectory must supply rules for building sources it contributes
models/onchipcommunication/common/%.o: ../models/onchipcommunication/common/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


