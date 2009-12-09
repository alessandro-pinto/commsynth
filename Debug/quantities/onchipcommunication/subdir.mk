################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../quantities/onchipcommunication/Interface.cpp \
../quantities/onchipcommunication/IpGeometry.cpp \
../quantities/onchipcommunication/LinkParameter.cpp \
../quantities/onchipcommunication/NodeParameter.cpp \
../quantities/onchipcommunication/PhysicalInterface.cpp \
../quantities/onchipcommunication/Ports.cpp 

OBJS += \
./quantities/onchipcommunication/Interface.o \
./quantities/onchipcommunication/IpGeometry.o \
./quantities/onchipcommunication/LinkParameter.o \
./quantities/onchipcommunication/NodeParameter.o \
./quantities/onchipcommunication/PhysicalInterface.o \
./quantities/onchipcommunication/Ports.o 

CPP_DEPS += \
./quantities/onchipcommunication/Interface.d \
./quantities/onchipcommunication/IpGeometry.d \
./quantities/onchipcommunication/LinkParameter.d \
./quantities/onchipcommunication/NodeParameter.d \
./quantities/onchipcommunication/PhysicalInterface.d \
./quantities/onchipcommunication/Ports.d 


# Each subdirectory must supply rules for building sources it contributes
quantities/onchipcommunication/%.o: ../quantities/onchipcommunication/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


