################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libraries/wiredbas/link/EthernetLink.cpp \
../libraries/wiredbas/link/TwistedPair.cpp 

OBJS += \
./libraries/wiredbas/link/EthernetLink.o \
./libraries/wiredbas/link/TwistedPair.o 

CPP_DEPS += \
./libraries/wiredbas/link/EthernetLink.d \
./libraries/wiredbas/link/TwistedPair.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/wiredbas/link/%.o: ../libraries/wiredbas/link/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


