################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../rules/wiredbas/node/ArcnetBusRule.cpp \
../rules/wiredbas/node/BusLayout.cpp \
../rules/wiredbas/node/EthernetNetworkRule.cpp \
../rules/wiredbas/node/NodePosition.cpp 

OBJS += \
./rules/wiredbas/node/ArcnetBusRule.o \
./rules/wiredbas/node/BusLayout.o \
./rules/wiredbas/node/EthernetNetworkRule.o \
./rules/wiredbas/node/NodePosition.o 

CPP_DEPS += \
./rules/wiredbas/node/ArcnetBusRule.d \
./rules/wiredbas/node/BusLayout.d \
./rules/wiredbas/node/EthernetNetworkRule.d \
./rules/wiredbas/node/NodePosition.d 


# Each subdirectory must supply rules for building sources it contributes
rules/wiredbas/node/%.o: ../rules/wiredbas/node/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


