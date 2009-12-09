################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libraries/wiredbas/common/Bus.cpp \
../libraries/wiredbas/common/InstantiableBus.cpp \
../libraries/wiredbas/common/InstantiableLink.cpp \
../libraries/wiredbas/common/InstantiableNode.cpp \
../libraries/wiredbas/common/InstantiableSubNetwork.cpp \
../libraries/wiredbas/common/Link.cpp \
../libraries/wiredbas/common/Node.cpp \
../libraries/wiredbas/common/SubNetwork.cpp 

OBJS += \
./libraries/wiredbas/common/Bus.o \
./libraries/wiredbas/common/InstantiableBus.o \
./libraries/wiredbas/common/InstantiableLink.o \
./libraries/wiredbas/common/InstantiableNode.o \
./libraries/wiredbas/common/InstantiableSubNetwork.o \
./libraries/wiredbas/common/Link.o \
./libraries/wiredbas/common/Node.o \
./libraries/wiredbas/common/SubNetwork.o 

CPP_DEPS += \
./libraries/wiredbas/common/Bus.d \
./libraries/wiredbas/common/InstantiableBus.d \
./libraries/wiredbas/common/InstantiableLink.d \
./libraries/wiredbas/common/InstantiableNode.d \
./libraries/wiredbas/common/InstantiableSubNetwork.d \
./libraries/wiredbas/common/Link.d \
./libraries/wiredbas/common/Node.d \
./libraries/wiredbas/common/SubNetwork.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/wiredbas/common/%.o: ../libraries/wiredbas/common/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


