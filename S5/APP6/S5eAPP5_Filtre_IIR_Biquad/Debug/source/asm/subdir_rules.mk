################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
source/asm/IIR_2ndOrder_directII_ASM.obj: ../source/asm/IIR_2ndOrder_directII_ASM.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/c6000_7.4.20/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti/ccsv7/tools/compiler/c6000_7.4.20/include" --include_path="C:/ti/ccsv7/ccs_base/C6xCSL/include" --include_path="C:/ti/dsk6713/include" --include_path="C:/Users/coll2903/Downloads/APPXX-TeamA/APPXX-TeamA/S5/APP6/S5eAPP5_Filtre_IIR_Biquad/includes" --define=CHIP_6713 --diag_wrap=off --display_error_number --diag_warning=225 --mem_model:data=far --mem_model:const=far --preproc_with_compile --preproc_dependency="source/asm/IIR_2ndOrder_directII_ASM.d" --obj_directory="source/asm" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/asm/Vectors_intr.obj: ../source/asm/Vectors_intr.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/c6000_7.4.20/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti/ccsv7/tools/compiler/c6000_7.4.20/include" --include_path="C:/ti/ccsv7/ccs_base/C6xCSL/include" --include_path="C:/ti/dsk6713/include" --include_path="C:/Users/coll2903/Downloads/APPXX-TeamA/APPXX-TeamA/S5/APP6/S5eAPP5_Filtre_IIR_Biquad/includes" --define=CHIP_6713 --diag_wrap=off --display_error_number --diag_warning=225 --mem_model:data=far --mem_model:const=far --preproc_with_compile --preproc_dependency="source/asm/Vectors_intr.d" --obj_directory="source/asm" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/asm/direct1FIR_ASM.obj: ../source/asm/direct1FIR_ASM.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/c6000_7.4.20/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti/ccsv7/tools/compiler/c6000_7.4.20/include" --include_path="C:/ti/ccsv7/ccs_base/C6xCSL/include" --include_path="C:/ti/dsk6713/include" --include_path="C:/Users/coll2903/Downloads/APPXX-TeamA/APPXX-TeamA/S5/APP6/S5eAPP5_Filtre_IIR_Biquad/includes" --define=CHIP_6713 --diag_wrap=off --display_error_number --diag_warning=225 --mem_model:data=far --mem_model:const=far --preproc_with_compile --preproc_dependency="source/asm/direct1FIR_ASM.d" --obj_directory="source/asm" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


