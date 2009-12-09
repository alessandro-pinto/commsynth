################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../rules/wiredbas/common/BusCapacity.cpp \
../rules/wiredbas/common/BusRule.cpp \
../rules/wiredbas/common/NetworkRule.cpp 

OBJS += \
./rules/wiredbas/common/BusCapacity.o \
./rules/wiredbas/common/BusRule.o \
./rules/wiredbas/common/NetworkRule.o 

CPP_DEPS += \
./rules/wiredbas/common/BusCapacity.d \
./rules/wiredbas/common/BusRule.d \
./rules/wiredbas/common/NetworkRule.d 


# Each subdirectory must supply rules for building sources it contributes
rules/wiredbas/common/%.o: ../rules/wiredbas/common/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


