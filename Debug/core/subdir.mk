################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../core/Bus.cpp \
../core/Factory.cpp \
../core/IdGraph.cpp \
../core/IdUndirectedGraph.cpp \
../core/LibraryParser.cpp \
../core/Link.cpp \
../core/NetworkParser.cpp \
../core/Node.cpp \
../core/NodeLinkLibrary.cpp \
../core/Quantity.cpp 

OBJS += \
./core/Bus.o \
./core/Factory.o \
./core/IdGraph.o \
./core/IdUndirectedGraph.o \
./core/LibraryParser.o \
./core/Link.o \
./core/NetworkParser.o \
./core/Node.o \
./core/NodeLinkLibrary.o \
./core/Quantity.o 

CPP_DEPS += \
./core/Bus.d \
./core/Factory.d \
./core/IdGraph.d \
./core/IdUndirectedGraph.d \
./core/LibraryParser.d \
./core/Link.d \
./core/NetworkParser.d \
./core/Node.d \
./core/NodeLinkLibrary.d \
./core/Quantity.d 


# Each subdirectory must supply rules for building sources it contributes
core/%.o: ../core/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


