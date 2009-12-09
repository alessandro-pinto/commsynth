################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../platforms/wiredbas/BusPlatform.cpp \
../platforms/wiredbas/NetworkPlatform.cpp 

OBJS += \
./platforms/wiredbas/BusPlatform.o \
./platforms/wiredbas/NetworkPlatform.o 

CPP_DEPS += \
./platforms/wiredbas/BusPlatform.d \
./platforms/wiredbas/NetworkPlatform.d 


# Each subdirectory must supply rules for building sources it contributes
platforms/wiredbas/%.o: ../platforms/wiredbas/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


