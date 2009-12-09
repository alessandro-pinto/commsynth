################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../models/wiredbas/link/EthernetLinkCost.cpp \
../models/wiredbas/link/EthernetLinkDelay.cpp \
../models/wiredbas/link/TwistedPairCost.cpp \
../models/wiredbas/link/TwistedPairDelay.cpp 

OBJS += \
./models/wiredbas/link/EthernetLinkCost.o \
./models/wiredbas/link/EthernetLinkDelay.o \
./models/wiredbas/link/TwistedPairCost.o \
./models/wiredbas/link/TwistedPairDelay.o 

CPP_DEPS += \
./models/wiredbas/link/EthernetLinkCost.d \
./models/wiredbas/link/EthernetLinkDelay.d \
./models/wiredbas/link/TwistedPairCost.d \
./models/wiredbas/link/TwistedPairDelay.d 


# Each subdirectory must supply rules for building sources it contributes
models/wiredbas/link/%.o: ../models/wiredbas/link/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


