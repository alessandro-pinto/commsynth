################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../models/wiredbas/node/ArcnetBusCost.cpp \
../models/wiredbas/node/ArcnetBusDelay.cpp \
../models/wiredbas/node/ArcnetNodeCost.cpp \
../models/wiredbas/node/EthernetNetworkCost.cpp \
../models/wiredbas/node/EthernetNetworkDelay.cpp \
../models/wiredbas/node/EthernetNodeCost.cpp \
../models/wiredbas/node/EthernetSwitchDelay.cpp 

OBJS += \
./models/wiredbas/node/ArcnetBusCost.o \
./models/wiredbas/node/ArcnetBusDelay.o \
./models/wiredbas/node/ArcnetNodeCost.o \
./models/wiredbas/node/EthernetNetworkCost.o \
./models/wiredbas/node/EthernetNetworkDelay.o \
./models/wiredbas/node/EthernetNodeCost.o \
./models/wiredbas/node/EthernetSwitchDelay.o 

CPP_DEPS += \
./models/wiredbas/node/ArcnetBusCost.d \
./models/wiredbas/node/ArcnetBusDelay.d \
./models/wiredbas/node/ArcnetNodeCost.d \
./models/wiredbas/node/EthernetNetworkCost.d \
./models/wiredbas/node/EthernetNetworkDelay.d \
./models/wiredbas/node/EthernetNodeCost.d \
./models/wiredbas/node/EthernetSwitchDelay.d 


# Each subdirectory must supply rules for building sources it contributes
models/wiredbas/node/%.o: ../models/wiredbas/node/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


