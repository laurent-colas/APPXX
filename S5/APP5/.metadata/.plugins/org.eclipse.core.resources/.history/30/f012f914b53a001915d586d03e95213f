################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv7/tools/compiler/c6000_7.4.20

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./source/asm/INT2ULAW.obj" \
"./source/asm/ULAW2INT.obj" \
"./source/asm/Vectors.obj" \
"./source/c/Audio_driver.obj" \
"./source/c/C6713Helper_UdeS.obj" \
"./source/c/SPI_driver.obj" \
"./source/c/Telephone.obj" \
"./source/c/module_example.obj" \
"../C6713dsk.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \
-l"C:/ti/ccsv7/tools/compiler/c6000_7.4.20/lib/rts6700.lib" \
-l"C:/ti/ccsv7/ccs_base/C6xCSL/lib_3x/csl6713.lib" \
-l"C:/ti/dsk6713/lib/dsk6713bsl.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include source/asm/subdir_vars.mk
-include source/c/subdir_vars.mk
-include subdir_rules.mk
-include source/asm/subdir_rules.mk
-include source/c/subdir_rules.mk
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
S5e_APP6.out \

EXE_OUTPUTS__QUOTED += \
"S5e_APP6.out" \

BIN_OUTPUTS += \
S5e_APP6.hex \

BIN_OUTPUTS__QUOTED += \
"S5e_APP6.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "S5e_APP6.out"

# Tool invocations
S5e_APP6.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C6000 Linker'
	"C:/ti/ccsv7/tools/compiler/c6000_7.4.20/bin/cl6x" -mv6700 --abi=coffabi -g --define=CHIP_6713 --diag_warning=225 --diag_wrap=off --display_error_number --mem_model:const=far --mem_model:data=far -z -m"S5e_APP6.map" --stack_size=0x1000 --heap_size=0x1000 -i"C:/ti/ccsv7/tools/compiler/c6000_7.4.20/lib" -i"C:/ti/ccsv7/tools/compiler/c6000_7.4.20/include" --reread_libs --warn_sections --display_error_number --diag_wrap=off --xml_link_info="S5e_APP6_linkInfo.xml" --rom_model -o "S5e_APP6.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

S5e_APP6.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: C6000 Hex Utility'
	"C:/ti/ccsv7/tools/compiler/c6000_7.4.20/bin/hex6x"  -o "S5e_APP6.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "source\asm\INT2ULAW.obj" "source\asm\ULAW2INT.obj" "source\asm\Vectors.obj" "source\c\Audio_driver.obj" "source\c\C6713Helper_UdeS.obj" "source\c\SPI_driver.obj" "source\c\Telephone.obj" "source\c\module_example.obj" 
	-$(RM) "source\c\Audio_driver.d" "source\c\C6713Helper_UdeS.d" "source\c\SPI_driver.d" "source\c\Telephone.d" "source\c\module_example.d" 
	-$(RM) "source\asm\INT2ULAW.d" "source\asm\ULAW2INT.d" "source\asm\Vectors.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

