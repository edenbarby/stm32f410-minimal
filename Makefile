# CMSIS device specific headers.
DIR_DRIVERS_CMSIS_STM32F4 = lib/cmsis_stm32f4xx
# STM device drivers.
DIR_DRIVERS_STM_STM32F4   = lib/stm_stm32f4xx
# Linker scripts.
DIR_LDSCRIPT              = ldscripts
# Project source files.
DIR_SRC                   = src
# All files produced during the build get dumped here.
DIR_BUILD                 = build


PROJ_NAME = test


# Generate source list and obj file names for the drivers that need compiling.
SRC := $(DIR_DRIVERS_CMSIS_STM32F4)/src/system_stm32f4xx.c
SRC := $(SRC) $(wildcard $(DIR_DRIVERS_STM_STM32F4)/src/*)
OBJ := $(SRC:.c=.o)
# Add project source list and CMSIS startup code.
SRC := $(wildcard $(DIR_SRC)/*) $(SRC) $(DIR_DRIVERS_CMSIS_STM32F4)/src/startup_stm32f410rx.s


# Defines required for the STM drivers.
CFLAGS  = -D STM32F410Rx -D USE_FULL_LL_DRIVER

CFLAGS += -Wall -Wstrict-prototypes -Werror
CFLAGS += -std=gnu99 -Os -fno-strict-aliasing
CFLAGS += -mlittle-endian -mfloat-abi=soft -mcpu=cortex-m4 -mthumb
CFLAGS += -ffunction-sections -fdata-sections -Wl,--gc-sections

LIBS  =-I $(DIR_DRIVERS_CMSIS_STM32F4)/inc
LIBS +=-I $(DIR_DRIVERS_STM_STM32F4)/inc
LIBS +=-I $(DIR_SRC)

LDSCRIPT = -L $(DIR_LDSCRIPT) -T stm32f410rb_flash.ld


# Tools.
CC = arm-none-eabi-gcc


.PHONY: all prog clean

all: $(PROJ_NAME).elf

prog: $(PROJ_NAME).elf
	openocd -f interface/stlink-v2-1.cfg -f target/stm32f4x.cfg -c "program $^ verify reset exit"

$(PROJ_NAME).elf: $(SRC)
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@ $(LDSCRIPT)

clean:
	rm $(PROJ_NAME)
