################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../environment/onchipcommunication/InstallationFace.cpp \
../environment/onchipcommunication/InstallationSite.cpp \
../environment/onchipcommunication/IpCore.cpp \
../environment/onchipcommunication/PlacedIpCore.cpp 

OBJS += \
./environment/onchipcommunication/InstallationFace.o \
./environment/onchipcommunication/InstallationSite.o \
./environment/onchipcommunication/IpCore.o \
./environment/onchipcommunication/PlacedIpCore.o 

CPP_DEPS += \
./environment/onchipcommunication/InstallationFace.d \
./environment/onchipcommunication/InstallationSite.d \
./environment/onchipcommunication/IpCore.d \
./environment/onchipcommunication/PlacedIpCore.d 


# Each subdirectory must supply rules for building sources it contributes
environment/onchipcommunication/%.o: ../environment/onchipcommunication/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


