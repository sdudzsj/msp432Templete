################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/CCS/ccs1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./system_msp432p401r.obj" \
"./ccs/startup_msp432p401r_ccs.obj" \
"./dsp/fft.obj" \
"./hardware/FMAM.obj" \
"./hardware/LCD12864.obj" \
"./hardware/adc.obj" \
"./hardware/dac7811.obj" \
"./hardware/dac8532.obj" \
"./hardware/key.obj" \
"./hardware/key4x4.obj" \
"./hardware/led.obj" \
"./hardware/oled.obj" \
"./hardware/swspi.obj" \
"./hardware/tim32.obj" \
"./hardware/timA.obj" \
"./hardware/tpl0401.obj" \
"./sys/baudrate_calculate.obj" \
"./sys/delay.obj" \
"./sys/sysinit.obj" \
"./sys/usart.obj" \
"./user/main.obj" \
"../msp432p401r.cmd" \
$(GEN_CMDS__FLAG) \
-l"ti/display/lib/display.aem4f" \
-l"ti/grlib/lib/ccs/m4f/grlib.a" \
-l"third_party/spiffs/lib/ccs/m4f/spiffs.a" \
-l"ti/drivers/lib/drivers_msp432p401x.aem4f" \
-l"third_party/fatfs/lib/ccs/m4f/fatfs.a" \
-l"ti/devices/msp432p4xx/driverlib/ccs/msp432p4xx_driverlib.lib" \
-llibc.a \
-l"ti/iqmathlib/ccs/IQmathLib_CCS_MSP432.lib" \
-l"ti/iqmathlib/ccs/QmathLib_CCS_MSP432.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include ccs/subdir_vars.mk
-include dsp/subdir_vars.mk
-include hardware/subdir_vars.mk
-include sys/subdir_vars.mk
-include user/subdir_vars.mk
-include subdir_rules.mk
-include ccs/subdir_rules.mk
-include dsp/subdir_rules.mk
-include hardware/subdir_rules.mk
-include sys/subdir_rules.mk
-include user/subdir_rules.mk
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
ccs432demoserve.out \

EXE_OUTPUTS__QUOTED += \
"ccs432demoserve.out" \

BIN_OUTPUTS += \
ccs432demoserve.hex \

BIN_OUTPUTS__QUOTED += \
"ccs432demoserve.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "ccs432demoserve.out"

# Tool invocations
ccs432demoserve.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: Arm Linker'
	"D:/CCS/ccs1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --advice:power=none --define=__MSP432P401R__ --define=DeviceFamily_MSP432P401x -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on -z -m"ccs432demoserve.map" --stack_size=512 -i"D:/CCS/msp432sdk/simplelink_msp432p4_sdk_3_40_01_02/source" -i"D:/CCS/ccs1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/lib" --diag_wrap=off --display_error_number --warn_sections --xml_link_info="ccs432demoserve_linkInfo.xml" --rom_model -o "ccs432demoserve.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

ccs432demoserve.hex: $(EXE_OUTPUTS)
	@echo 'Building secondary target: "$@"'
	@echo 'Invoking: Arm Hex Utility'
	"D:/CCS/ccs1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armhex" --diag_wrap=off -o "ccs432demoserve.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building secondary target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "system_msp432p401r.obj" "ccs\startup_msp432p401r_ccs.obj" "dsp\fft.obj" "hardware\FMAM.obj" "hardware\LCD12864.obj" "hardware\adc.obj" "hardware\dac7811.obj" "hardware\dac8532.obj" "hardware\key.obj" "hardware\key4x4.obj" "hardware\led.obj" "hardware\oled.obj" "hardware\swspi.obj" "hardware\tim32.obj" "hardware\timA.obj" "hardware\tpl0401.obj" "sys\baudrate_calculate.obj" "sys\delay.obj" "sys\sysinit.obj" "sys\usart.obj" "user\main.obj" 
	-$(RM) "system_msp432p401r.d" "ccs\startup_msp432p401r_ccs.d" "dsp\fft.d" "hardware\FMAM.d" "hardware\LCD12864.d" "hardware\adc.d" "hardware\dac7811.d" "hardware\dac8532.d" "hardware\key.d" "hardware\key4x4.d" "hardware\led.d" "hardware\oled.d" "hardware\swspi.d" "hardware\tim32.d" "hardware\timA.d" "hardware\tpl0401.d" "sys\baudrate_calculate.d" "sys\delay.d" "sys\sysinit.d" "sys\usart.d" "user\main.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

