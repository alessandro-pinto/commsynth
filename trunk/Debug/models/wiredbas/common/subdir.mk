################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../models/wiredbas/common/BusCost.cpp \
../models/wiredbas/common/BusDelay.cpp \
../models/wiredbas/common/LinkDelayModel.cpp \
../models/wiredbas/common/NetworkCost.cpp \
../models/wiredbas/common/NetworkDelay.cpp \
../models/wiredbas/common/NodeCost.cpp \
../models/wiredbas/common/NodeDelay.cpp \
../models/wiredbas/common/WireCost.cpp \
../models/wiredbas/common/WireDelayModel.cpp 

OBJS += \
./models/wiredbas/common/BusCost.o \
./models/wiredbas/common/BusDelay.o \
./models/wiredbas/common/LinkDelayModel.o \
./models/wiredbas/common/NetworkCost.o \
./models/wiredbas/common/NetworkDelay.o \
./models/wiredbas/common/NodeCost.o \
./models/wiredbas/common/NodeDelay.o \
./models/wiredbas/common/WireCost.o \
./models/wiredbas/common/WireDelayModel.o 

CPP_DEPS += \
./models/wiredbas/common/BusCost.d \
./models/wiredbas/common/BusDelay.d \
./models/wiredbas/common/LinkDelayModel.d \
./models/wiredbas/common/NetworkCost.d \
./models/wiredbas/common/NetworkDelay.d \
./models/wiredbas/common/NodeCost.d \
./models/wiredbas/common/NodeDelay.d \
./models/wiredbas/common/WireCost.d \
./models/wiredbas/common/WireDelayModel.d 


# Each subdirectory must supply rules for building sources it contributes
models/wiredbas/common/%.o: ../models/wiredbas/common/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


