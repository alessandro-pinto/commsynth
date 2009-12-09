################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include rules/wiredbas/node/subdir.mk
-include rules/wiredbas/link/subdir.mk
-include rules/wiredbas/common/subdir.mk
-include rules/onchipcommunication/subdir.mk
-include rules/subdir.mk
-include quantities/onchipcommunication/subdir.mk
-include quantities/subdir.mk
-include position/subdir.mk
-include platforms/wiredbas/subdir.mk
-include platforms/onchipcommunication/subdir.mk
-include platforms/subdir.mk
-include networks/wiredbas/subdir.mk
-include networks/onchipcommunication/subdir.mk
-include networks/graph/subdir.mk
-include models/wiredbas/node/subdir.mk
-include models/wiredbas/link/subdir.mk
-include models/wiredbas/common/subdir.mk
-include models/onchipcommunication/node/subdir.mk
-include models/onchipcommunication/link/subdir.mk
-include models/onchipcommunication/common/subdir.mk
-include libraries/wiredbas/node/subdir.mk
-include libraries/wiredbas/link/subdir.mk
-include libraries/wiredbas/common/subdir.mk
-include libraries/onchipcommunication/systemc/subdir.mk
-include libraries/onchipcommunication/node/subdir.mk
-include libraries/onchipcommunication/link/subdir.mk
-include libraries/onchipcommunication/common/subdir.mk
-include libraries/subdir.mk
-include label/subdir.mk
-include io/onchipcommunication/subdir.mk
-include io/common/subdir.mk
-include io/bas/subdir.mk
-include environment/onchipcommunication/subdir.mk
-include environment/bas/subdir.mk
-include core/subdir.mk
-include algorithms/wiredbas/subdir.mk
-include algorithms/onchipcommunication/subdir.mk
-include algorithms/graph/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: libCOSI.a

# Tool invocations
libCOSI.a: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Archiver'
	ar -r "libCOSI.a" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(ARCHIVES)$(C++_DEPS)$(CC_DEPS)$(C_DEPS)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS) libCOSI.a
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
