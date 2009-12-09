################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libraries/wiredbas/node/ArcnetActuator.cpp \
../libraries/wiredbas/node/ArcnetBus.cpp \
../libraries/wiredbas/node/ArcnetController.cpp \
../libraries/wiredbas/node/ArcnetNode.cpp \
../libraries/wiredbas/node/ArcnetRouter.cpp \
../libraries/wiredbas/node/ArcnetSensor.cpp \
../libraries/wiredbas/node/EthernetCameraSensor.cpp \
../libraries/wiredbas/node/EthernetController.cpp \
../libraries/wiredbas/node/EthernetNode.cpp \
../libraries/wiredbas/node/EthernetSubNetwork.cpp \
../libraries/wiredbas/node/EthernetSwitch.cpp 

OBJS += \
./libraries/wiredbas/node/ArcnetActuator.o \
./libraries/wiredbas/node/ArcnetBus.o \
./libraries/wiredbas/node/ArcnetController.o \
./libraries/wiredbas/node/ArcnetNode.o \
./libraries/wiredbas/node/ArcnetRouter.o \
./libraries/wiredbas/node/ArcnetSensor.o \
./libraries/wiredbas/node/EthernetCameraSensor.o \
./libraries/wiredbas/node/EthernetController.o \
./libraries/wiredbas/node/EthernetNode.o \
./libraries/wiredbas/node/EthernetSubNetwork.o \
./libraries/wiredbas/node/EthernetSwitch.o 

CPP_DEPS += \
./libraries/wiredbas/node/ArcnetActuator.d \
./libraries/wiredbas/node/ArcnetBus.d \
./libraries/wiredbas/node/ArcnetController.d \
./libraries/wiredbas/node/ArcnetNode.d \
./libraries/wiredbas/node/ArcnetRouter.d \
./libraries/wiredbas/node/ArcnetSensor.d \
./libraries/wiredbas/node/EthernetCameraSensor.d \
./libraries/wiredbas/node/EthernetController.d \
./libraries/wiredbas/node/EthernetNode.d \
./libraries/wiredbas/node/EthernetSubNetwork.d \
./libraries/wiredbas/node/EthernetSwitch.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/wiredbas/node/%.o: ../libraries/wiredbas/node/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


