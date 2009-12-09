################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../rules/onchipcommunication/ComponentPosition.cpp \
../rules/onchipcommunication/CompositionRule.cpp \
../rules/onchipcommunication/InstantiationId.cpp 

OBJS += \
./rules/onchipcommunication/ComponentPosition.o \
./rules/onchipcommunication/CompositionRule.o \
./rules/onchipcommunication/InstantiationId.o 

CPP_DEPS += \
./rules/onchipcommunication/ComponentPosition.d \
./rules/onchipcommunication/CompositionRule.d \
./rules/onchipcommunication/InstantiationId.d 


# Each subdirectory must supply rules for building sources it contributes
rules/onchipcommunication/%.o: ../rules/onchipcommunication/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


