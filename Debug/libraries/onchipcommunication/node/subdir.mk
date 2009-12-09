################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libraries/onchipcommunication/node/Ip.cpp \
../libraries/onchipcommunication/node/IpDestPort.cpp \
../libraries/onchipcommunication/node/IpSourcePort.cpp \
../libraries/onchipcommunication/node/RelayStation.cpp \
../libraries/onchipcommunication/node/Router.cpp 

OBJS += \
./libraries/onchipcommunication/node/Ip.o \
./libraries/onchipcommunication/node/IpDestPort.o \
./libraries/onchipcommunication/node/IpSourcePort.o \
./libraries/onchipcommunication/node/RelayStation.o \
./libraries/onchipcommunication/node/Router.o 

CPP_DEPS += \
./libraries/onchipcommunication/node/Ip.d \
./libraries/onchipcommunication/node/IpDestPort.d \
./libraries/onchipcommunication/node/IpSourcePort.d \
./libraries/onchipcommunication/node/RelayStation.d \
./libraries/onchipcommunication/node/Router.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/onchipcommunication/node/%.o: ../libraries/onchipcommunication/node/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


