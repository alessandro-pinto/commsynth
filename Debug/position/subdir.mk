################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../position/RoutingTable.cpp \
../position/Xyz.cpp 

OBJS += \
./position/RoutingTable.o \
./position/Xyz.o 

CPP_DEPS += \
./position/RoutingTable.d \
./position/Xyz.d 


# Each subdirectory must supply rules for building sources it contributes
position/%.o: ../position/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


