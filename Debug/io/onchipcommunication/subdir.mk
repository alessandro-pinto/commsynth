################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../io/onchipcommunication/ConfigParser.cpp \
../io/onchipcommunication/Constraint.cpp \
../io/onchipcommunication/ConstraintParser.cpp \
../io/onchipcommunication/HmetisInterface.cpp \
../io/onchipcommunication/HoOrionModelParser.cpp \
../io/onchipcommunication/InkscapeCodeGen.cpp \
../io/onchipcommunication/IpParser.cpp \
../io/onchipcommunication/ParquetCodeGen.cpp \
../io/onchipcommunication/PlacementParser.cpp \
../io/onchipcommunication/ProjectParser.cpp \
../io/onchipcommunication/Report.cpp \
../io/onchipcommunication/SyscCodeGen.cpp \
../io/onchipcommunication/ToGraph.cpp 

OBJS += \
./io/onchipcommunication/ConfigParser.o \
./io/onchipcommunication/Constraint.o \
./io/onchipcommunication/ConstraintParser.o \
./io/onchipcommunication/HmetisInterface.o \
./io/onchipcommunication/HoOrionModelParser.o \
./io/onchipcommunication/InkscapeCodeGen.o \
./io/onchipcommunication/IpParser.o \
./io/onchipcommunication/ParquetCodeGen.o \
./io/onchipcommunication/PlacementParser.o \
./io/onchipcommunication/ProjectParser.o \
./io/onchipcommunication/Report.o \
./io/onchipcommunication/SyscCodeGen.o \
./io/onchipcommunication/ToGraph.o 

CPP_DEPS += \
./io/onchipcommunication/ConfigParser.d \
./io/onchipcommunication/Constraint.d \
./io/onchipcommunication/ConstraintParser.d \
./io/onchipcommunication/HmetisInterface.d \
./io/onchipcommunication/HoOrionModelParser.d \
./io/onchipcommunication/InkscapeCodeGen.d \
./io/onchipcommunication/IpParser.d \
./io/onchipcommunication/ParquetCodeGen.d \
./io/onchipcommunication/PlacementParser.d \
./io/onchipcommunication/ProjectParser.d \
./io/onchipcommunication/Report.d \
./io/onchipcommunication/SyscCodeGen.d \
./io/onchipcommunication/ToGraph.d 


# Each subdirectory must supply rules for building sources it contributes
io/onchipcommunication/%.o: ../io/onchipcommunication/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


