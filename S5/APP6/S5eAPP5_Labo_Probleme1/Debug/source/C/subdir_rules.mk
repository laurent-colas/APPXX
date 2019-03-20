################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
source/C/C6713Helper_UdeS.obj: ../source/C/C6713Helper_UdeS.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/c6000_7.4.20/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti/ccsv7/tools/compiler/c6000_7.4.20/include" --include_path="C:/ti/ccsv7/ccs_base/C6xCSL/include" --include_path="C:/ti/dsk6713/include" --include_path="C:/Users/chap1202/Dropbox/Documents on DropBox/Enseignement/S5/S5elec_2017__Hiv/CCS/S5eAPP5_Labo_Probleme1/include" --define=CHIP_6713 --diag_warning=225 --diag_wrap=off --display_error_number --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="source/C/C6713Helper_UdeS.d" --obj_directory="source/C" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/C/bitrev_index.obj: ../source/C/bitrev_index.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/c6000_7.4.20/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti/ccsv7/tools/compiler/c6000_7.4.20/include" --include_path="C:/ti/ccsv7/ccs_base/C6xCSL/include" --include_path="C:/ti/dsk6713/include" --include_path="C:/Users/chap1202/Dropbox/Documents on DropBox/Enseignement/S5/S5elec_2017__Hiv/CCS/S5eAPP5_Labo_Probleme1/include" --define=CHIP_6713 --diag_warning=225 --diag_wrap=off --display_error_number --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="source/C/bitrev_index.d" --obj_directory="source/C" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/C/probleme1.obj: ../source/C/probleme1.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/c6000_7.4.20/bin/cl6x" -mv6700 --abi=coffabi -g --include_path="C:/ti/ccsv7/tools/compiler/c6000_7.4.20/include" --include_path="C:/ti/ccsv7/ccs_base/C6xCSL/include" --include_path="C:/ti/dsk6713/include" --include_path="C:/Users/chap1202/Dropbox/Documents on DropBox/Enseignement/S5/S5elec_2017__Hiv/CCS/S5eAPP5_Labo_Probleme1/include" --define=CHIP_6713 --diag_warning=225 --diag_wrap=off --display_error_number --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="source/C/probleme1.d" --obj_directory="source/C" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


