################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../environment/bas/Face.cpp \
../environment/bas/InstSurface.cpp \
../environment/bas/Wall.cpp 

OBJS += \
./environment/bas/Face.o \
./environment/bas/InstSurface.o \
./environment/bas/Wall.o 

CPP_DEPS += \
./environment/bas/Face.d \
./environment/bas/InstSurface.d \
./environment/bas/Wall.d 


# Each subdirectory must supply rules for building sources it contributes
environment/bas/%.o: ../environment/bas/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


