PROJ_NAME = project


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


# Generate source list and obj file names for the drivers that need compiling.
SRC := $(DIR_DRIVERS_CMSIS_STM32F4)/src/system_stm32f4xx.c
SRC := $(SRC) $(wildcard $(DIR_DRIVERS_STM_STM32F4)/src/*)
OBJ := $(SRC:.c=.o)
# Add project source list and CMSIS startup code.
SRC := $(wildcard $(DIR_SRC)/*) $(SRC) $(DIR_DRIVERS_CMSIS_STM32F4)/src/startup_stm32f410rx.s


# Tools.
CC   = arm-none-eabi-gcc
GDB  = arm-none-eabi-gdb
PROG = openocd
RM   = rm -f


# General gcc compiler flags.
# Enable all warning, prototypes must have explicit arguments and make all
# warnings errors
CFLAGS  = -Wall -Wstrict-prototypes -Werror
# Do not allow the compiler to assume pointers are not aliases. Lets you play
# around with c pointer fuckery to your hearts content without worrying about
# the compiler "optimizing" it all away.
CFLAGS += -fno-strict-aliasing

# uC specific flags.
CFLAGS += -mcpu=cortex-m4 -mfloat-abi=soft -mthumb
# Defines required for the STM drivers.
CFLAGS += -D STM32F410Rx -D USE_FULL_LL_DRIVER
CFLAGS += --specs=nosys.specs

# Release flags.
CRFLAGS  = $(CFLAGS)
# Allows linker to remove dead code.
CRFLAGS += -ffunction-sections -fdata-sections -Wl,--gc-sections
# Optimize binary size.
CRFLAGS += -Os
# Optimize binary performance.
#CRFLAGS += -O3

# Debug flags.
CDFLAGS  = $(CFLAGS)
# Add GDB debug symbols.
CDFLAGS += -ggdb
# Produces a link map.
#CDFLAGS += -Wl,-Map=$(PROJ_NAME).map

LIBS  =-I $(DIR_DRIVERS_CMSIS_STM32F4)/inc
LIBS +=-I $(DIR_DRIVERS_STM_STM32F4)/inc
LIBS +=-I $(DIR_SRC)

LDSCRIPT = -L $(DIR_LDSCRIPT) -T stm32f410rb_flash.ld


PROG_CFG = -f interface/stlink.cfg -f target/stm32f4x.cfg
PROG_DBG = -c "gdb_port pipe" -c "log_output $(PROJ_NAME)_oocd.log"


.PHONY: all prog clean

all: prog

prog: $(PROJ_NAME).elf
	$(PROG) $(PROG_CFG) -c "program $^ verify reset exit"

$(PROJ_NAME).elf: $(SRC)
	$(CC) $(CRFLAGS) $(LIBS) $^ -o $@ $(LDSCRIPT)

debug: $(PROJ_NAME)_dbg.elf
	$(GDB) -q --eval-command='target remote | $(PROG) $(PROG_CFG) $(PROG_DBG) -c "program $^ verify reset"' $^

$(PROJ_NAME)_dbg.elf: $(SRC)
	$(CC) $(CDFLAGS) $(LIBS) $^ -o $@ $(LDSCRIPT)

clean:
	$(RM) $(PROJ_NAME).elf $(PROJ_NAME)_dbg.elf $(PROJ_NAME).map $(PROJ_NAME)_oocd.log
