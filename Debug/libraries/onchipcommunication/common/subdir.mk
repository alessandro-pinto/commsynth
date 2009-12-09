################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libraries/onchipcommunication/common/Link.cpp \
../libraries/onchipcommunication/common/Node.cpp \
../libraries/onchipcommunication/common/_Link.cpp \
../libraries/onchipcommunication/common/_Node.cpp 

OBJS += \
./libraries/onchipcommunication/common/Link.o \
./libraries/onchipcommunication/common/Node.o \
./libraries/onchipcommunication/common/_Link.o \
./libraries/onchipcommunication/common/_Node.o 

CPP_DEPS += \
./libraries/onchipcommunication/common/Link.d \
./libraries/onchipcommunication/common/Node.d \
./libraries/onchipcommunication/common/_Link.d \
./libraries/onchipcommunication/common/_Node.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/onchipcommunication/common/%.o: ../libraries/onchipcommunication/common/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


