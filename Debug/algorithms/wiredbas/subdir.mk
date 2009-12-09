################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../algorithms/wiredbas/ChainCover.cpp \
../algorithms/wiredbas/ChainGenerator.cpp \
../algorithms/wiredbas/HeuristicChainGenerator.cpp \
../algorithms/wiredbas/PartitionGenerator.cpp \
../algorithms/wiredbas/PbCoverGeneration.cpp \
../algorithms/wiredbas/SecondLevelNetwork.cpp 

OBJS += \
./algorithms/wiredbas/ChainCover.o \
./algorithms/wiredbas/ChainGenerator.o \
./algorithms/wiredbas/HeuristicChainGenerator.o \
./algorithms/wiredbas/PartitionGenerator.o \
./algorithms/wiredbas/PbCoverGeneration.o \
./algorithms/wiredbas/SecondLevelNetwork.o 

CPP_DEPS += \
./algorithms/wiredbas/ChainCover.d \
./algorithms/wiredbas/ChainGenerator.d \
./algorithms/wiredbas/HeuristicChainGenerator.d \
./algorithms/wiredbas/PartitionGenerator.d \
./algorithms/wiredbas/PbCoverGeneration.d \
./algorithms/wiredbas/SecondLevelNetwork.d 


# Each subdirectory must supply rules for building sources it contributes
algorithms/wiredbas/%.o: ../algorithms/wiredbas/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


