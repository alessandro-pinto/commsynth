################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../algorithms/graph/ConnectedComponents.cpp \
../algorithms/graph/FacilityLocation.cpp \
../algorithms/graph/Kmedian.cpp \
../algorithms/graph/MinimuSpanningTree.cpp \
../algorithms/graph/ShortestPath.cpp \
../algorithms/graph/Tsp.cpp 

OBJS += \
./algorithms/graph/ConnectedComponents.o \
./algorithms/graph/FacilityLocation.o \
./algorithms/graph/Kmedian.o \
./algorithms/graph/MinimuSpanningTree.o \
./algorithms/graph/ShortestPath.o \
./algorithms/graph/Tsp.o 

CPP_DEPS += \
./algorithms/graph/ConnectedComponents.d \
./algorithms/graph/FacilityLocation.d \
./algorithms/graph/Kmedian.d \
./algorithms/graph/MinimuSpanningTree.d \
./algorithms/graph/ShortestPath.d \
./algorithms/graph/Tsp.d 


# Each subdirectory must supply rules for building sources it contributes
algorithms/graph/%.o: ../algorithms/graph/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


