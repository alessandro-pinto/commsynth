################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../algorithms/onchipcommunication/HierarchicalNetwork.cpp \
../algorithms/onchipcommunication/MFDg.cpp \
../algorithms/onchipcommunication/MeshTopology.cpp \
../algorithms/onchipcommunication/NetworkCost.cpp \
../algorithms/onchipcommunication/QueueHandler.cpp \
../algorithms/onchipcommunication/SFDg.cpp \
../algorithms/onchipcommunication/StarTopologyGeneration.cpp 

OBJS += \
./algorithms/onchipcommunication/HierarchicalNetwork.o \
./algorithms/onchipcommunication/MFDg.o \
./algorithms/onchipcommunication/MeshTopology.o \
./algorithms/onchipcommunication/NetworkCost.o \
./algorithms/onchipcommunication/QueueHandler.o \
./algorithms/onchipcommunication/SFDg.o \
./algorithms/onchipcommunication/StarTopologyGeneration.o 

CPP_DEPS += \
./algorithms/onchipcommunication/HierarchicalNetwork.d \
./algorithms/onchipcommunication/MFDg.d \
./algorithms/onchipcommunication/MeshTopology.d \
./algorithms/onchipcommunication/NetworkCost.d \
./algorithms/onchipcommunication/QueueHandler.d \
./algorithms/onchipcommunication/SFDg.d \
./algorithms/onchipcommunication/StarTopologyGeneration.d 


# Each subdirectory must supply rules for building sources it contributes
algorithms/onchipcommunication/%.o: ../algorithms/onchipcommunication/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


