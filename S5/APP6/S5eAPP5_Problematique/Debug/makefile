################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv7/tools/compiler/c6000_7.4.20

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./source/C/C6713Helper_UdeS.obj" \
"./source/C/bitrev_index.obj" \
"./source/C/findErrAccordage.obj" \
"./source/C/genCos.obj" \
"./source/C/getNewCommand.obj" \
"./source/C/main_accordeur.obj" \
"./source/asm/Vectors_intr.obj" \
"../C6713dsk.cmd" \
"../includes/dsp67x.lib" \
$(GEN_CMDS__FLAG) \
-l"C:/ti/ccsv7/tools/compiler/c6000_7.4.20/lib/rts6700.lib" \
-l"C:/ti/ccsv7/ccs_base/C6xCSL/lib_3x/csl6713.lib" \
-l"C:/ti/dsk6713/lib/dsk6713bsl.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include includes/subdir_vars.mk
-include source/C/subdir_vars.mk
-include source/asm/subdir_vars.mk
-include subdir_rules.mk
-include includes/subdir_rules.mk
-include source/C/subdir_rules.mk
-include source/asm/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
S5eAPP5_Problematique.out \

EXE_OUTPUTS__QUOTED += \
"S5eAPP5_Problematique.out" \


# All Target
all: S5eAPP5_Problematique.out

# Tool invocations
S5eAPP5_Problematique.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C6000 Linker'
	"C:/ti/ccsv7/tools/compiler/c6000_7.4.20/bin/cl6x" -mv6700 --abi=coffabi -g --define=CHIP_6713 --diag_warning=225 --diag_wrap=off --display_error_number --mem_model:const=far --mem_model:data=far -z -m"S5eAPP5_Problematique.map" --stack_size=0x8000 --heap_size=0x8000 --reread_libs --diag_suppress=16002 --warn_sections --display_error_number --diag_wrap=off --xml_link_info="S5eAPP5_Problematique_linkInfo.xml" --rom_model -o "S5eAPP5_Problematique.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "source\C\C6713Helper_UdeS.obj" "source\C\bitrev_index.obj" "source\C\findErrAccordage.obj" "source\C\genCos.obj" "source\C\getNewCommand.obj" "source\C\main_accordeur.obj" "source\asm\Vectors_intr.obj" 
	-$(RM) "source\C\C6713Helper_UdeS.d" "source\C\bitrev_index.d" "source\C\findErrAccordage.d" "source\C\genCos.d" "source\C\getNewCommand.d" "source\C\main_accordeur.d" 
	-$(RM) "source\asm\Vectors_intr.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

