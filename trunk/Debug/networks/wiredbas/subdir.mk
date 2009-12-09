################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../networks/wiredbas/WiredBasImplementation.cpp \
../networks/wiredbas/WiredBasPlatformInstance.cpp \
../networks/wiredbas/WiredBasSpecificaion.cpp 

OBJS += \
./networks/wiredbas/WiredBasImplementation.o \
./networks/wiredbas/WiredBasPlatformInstance.o \
./networks/wiredbas/WiredBasSpecificaion.o 

CPP_DEPS += \
./networks/wiredbas/WiredBasImplementation.d \
./networks/wiredbas/WiredBasPlatformInstance.d \
./networks/wiredbas/WiredBasSpecificaion.d 


# Each subdirectory must supply rules for building sources it contributes
networks/wiredbas/%.o: ../networks/wiredbas/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


