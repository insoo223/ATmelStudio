################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../config.c \
../DHT11.c \
../DS1307.c \
../EEPROM.c \
../LCD_DS1307_DHT11.c \
../globals.c \
../I2C.c \
../intrpt.c \
../LCD.c \
../LCDutil.c \
../main.c \
../PowerSave.c \
../Sound.c \
../strFunc.c \
../TimingDebug.c \
../tinudht.c \
../Menu.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
config.o \
DHT11.o \
DS1307.o \
EEPROM.o \
LCD_DS1307_DHT11.o \
globals.o \
I2C.o \
intrpt.o \
LCD.o \
LCDutil.o \
main.o \
PowerSave.o \
Sound.o \
strFunc.o \
TimingDebug.o \
tinudht.o \
Menu.o

OBJS_AS_ARGS +=  \
config.o \
DHT11.o \
DS1307.o \
EEPROM.o \
LCD_DS1307_DHT11.o \
globals.o \
I2C.o \
intrpt.o \
LCD.o \
LCDutil.o \
main.o \
PowerSave.o \
Sound.o \
strFunc.o \
TimingDebug.o \
tinudht.o \
Menu.o

C_DEPS +=  \
config.d \
DHT11.d \
DS1307.d \
EEPROM.d \
LCD_DS1307_DHT11.d \
globals.d \
I2C.d \
intrpt.d \
LCD.d \
LCDutil.d \
main.d \
PowerSave.d \
Sound.d \
strFunc.d \
TimingDebug.d \
tinudht.d \
Menu.d

C_DEPS_AS_ARGS +=  \
config.d \
DHT11.d \
DS1307.d \
EEPROM.d \
LCD_DS1307_DHT11.d \
globals.d \
I2C.d \
intrpt.d \
LCD.d \
LCDutil.d \
main.d \
PowerSave.d \
Sound.d \
strFunc.d \
TimingDebug.d \
tinudht.d \
Menu.d

OUTPUT_FILE_PATH +=LCD_RTC_DS1307_DHT11.elf

OUTPUT_FILE_PATH_AS_ARGS +=LCD_RTC_DS1307_DHT11.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler



































./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\include"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega328p -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\gcc\dev\atmega328p" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="LCD_RTC_DS1307_DHT11.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega328p -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\gcc\dev\atmega328p"  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "LCD_RTC_DS1307_DHT11.elf" "LCD_RTC_DS1307_DHT11.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "LCD_RTC_DS1307_DHT11.elf" "LCD_RTC_DS1307_DHT11.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "LCD_RTC_DS1307_DHT11.elf" > "LCD_RTC_DS1307_DHT11.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "LCD_RTC_DS1307_DHT11.elf" "LCD_RTC_DS1307_DHT11.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "LCD_RTC_DS1307_DHT11.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "LCD_RTC_DS1307_DHT11.elf" "LCD_RTC_DS1307_DHT11.a" "LCD_RTC_DS1307_DHT11.hex" "LCD_RTC_DS1307_DHT11.lss" "LCD_RTC_DS1307_DHT11.eep" "LCD_RTC_DS1307_DHT11.map" "LCD_RTC_DS1307_DHT11.srec" "LCD_RTC_DS1307_DHT11.usersignatures"
	