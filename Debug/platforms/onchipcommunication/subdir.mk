################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../platforms/onchipcommunication/NocPlatform.cpp \
../platforms/onchipcommunication/NocPlatformInterface.cpp \
../platforms/onchipcommunication/Platform.cpp 

OBJS += \
./platforms/onchipcommunication/NocPlatform.o \
./platforms/onchipcommunication/NocPlatformInterface.o \
./platforms/onchipcommunication/Platform.o 

CPP_DEPS += \
./platforms/onchipcommunication/NocPlatform.d \
./platforms/onchipcommunication/NocPlatformInterface.d \
./platforms/onchipcommunication/Platform.d 


# Each subdirectory must supply rules for building sources it contributes
platforms/onchipcommunication/%.o: ../platforms/onchipcommunication/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


