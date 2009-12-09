################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../networks/onchipcommunication/ChannelDependencyGraph.cpp \
../networks/onchipcommunication/Implementation.cpp \
../networks/onchipcommunication/PlatformInstance.cpp \
../networks/onchipcommunication/Specification.cpp 

OBJS += \
./networks/onchipcommunication/ChannelDependencyGraph.o \
./networks/onchipcommunication/Implementation.o \
./networks/onchipcommunication/PlatformInstance.o \
./networks/onchipcommunication/Specification.o 

CPP_DEPS += \
./networks/onchipcommunication/ChannelDependencyGraph.d \
./networks/onchipcommunication/Implementation.d \
./networks/onchipcommunication/PlatformInstance.d \
./networks/onchipcommunication/Specification.d 


# Each subdirectory must supply rules for building sources it contributes
networks/onchipcommunication/%.o: ../networks/onchipcommunication/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


