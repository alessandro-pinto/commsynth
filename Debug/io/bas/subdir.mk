################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../io/bas/HmetisInterface.cpp \
../io/bas/InkScapeCodeGen.cpp \
../io/bas/PatohInterface.cpp \
../io/bas/PbCodeGen.cpp \
../io/bas/SvgBuildingParser.cpp \
../io/bas/SynthesisReport.cpp 

OBJS += \
./io/bas/HmetisInterface.o \
./io/bas/InkScapeCodeGen.o \
./io/bas/PatohInterface.o \
./io/bas/PbCodeGen.o \
./io/bas/SvgBuildingParser.o \
./io/bas/SynthesisReport.o 

CPP_DEPS += \
./io/bas/HmetisInterface.d \
./io/bas/InkScapeCodeGen.d \
./io/bas/PatohInterface.d \
./io/bas/PbCodeGen.d \
./io/bas/SvgBuildingParser.d \
./io/bas/SynthesisReport.d 


# Each subdirectory must supply rules for building sources it contributes
io/bas/%.o: ../io/bas/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


