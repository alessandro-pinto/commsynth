################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../quantities/Bandwidth.cpp \
../quantities/Commodity.cpp \
../quantities/CommoditySet.cpp \
../quantities/Name.cpp \
../quantities/Ports.cpp \
../quantities/Position.cpp \
../quantities/RealDelay.cpp \
../quantities/RealLatency.cpp \
../quantities/Thread.cpp \
../quantities/ThreadSet.cpp \
../quantities/TransferTable.cpp \
../quantities/Type.cpp \
../quantities/WiringPath.cpp 

OBJS += \
./quantities/Bandwidth.o \
./quantities/Commodity.o \
./quantities/CommoditySet.o \
./quantities/Name.o \
./quantities/Ports.o \
./quantities/Position.o \
./quantities/RealDelay.o \
./quantities/RealLatency.o \
./quantities/Thread.o \
./quantities/ThreadSet.o \
./quantities/TransferTable.o \
./quantities/Type.o \
./quantities/WiringPath.o 

CPP_DEPS += \
./quantities/Bandwidth.d \
./quantities/Commodity.d \
./quantities/CommoditySet.d \
./quantities/Name.d \
./quantities/Ports.d \
./quantities/Position.d \
./quantities/RealDelay.d \
./quantities/RealLatency.d \
./quantities/Thread.d \
./quantities/ThreadSet.d \
./quantities/TransferTable.d \
./quantities/Type.d \
./quantities/WiringPath.d 


# Each subdirectory must supply rules for building sources it contributes
quantities/%.o: ../quantities/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


