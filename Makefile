##########################################################################
# User configuration and firmware specific object files	
##########################################################################

PROJECT = firmware
LDSCRIPT = lpc810.ld
OPTIMIZATION = s

##########################################################################
# Output directories
##########################################################################

BIN_PATH = bin
OBJ_PATH = bin/obj

##########################################################################
# Source Files
##########################################################################

VPATH  = src
OBJS   = $(OBJ_PATH)/gcc_startup_lpc8xx.o 
OBJS  += $(OBJ_PATH)/system_LPC8xx.o
OBJS  += $(OBJ_PATH)/main.o
OBJS  += $(OBJ_PATH)/gpio.o
OBJS  += $(OBJ_PATH)/mrt.o
OBJS  += $(OBJ_PATH)/printf.o
OBJS  += $(OBJ_PATH)/printf-retarget.o
OBJS  += $(OBJ_PATH)/spi.o

##########################################################################
# Include paths
##########################################################################

ROOT_PATH = src
INCLUDE_PATHS = -I$(ROOT_PATH) -Icmsis

##########################################################################
# GNU GCC compiler prefix
##########################################################################

# Use the default toolchain (based on the PATH variable, etc.)
CROSS_COMPILE = arm-none-eabi-

# Use a toolchain at a specific location
# CROSS_COMPILE = C:/arm/gnu4.7.2012.q4/bin/arm-none-eabi-

AS      = $(CROSS_COMPILE)gcc
CC      = $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)gcc
SIZE    = $(CROSS_COMPILE)size
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
OUTFILE = $(BIN_PATH)/$(PROJECT)
REMOVE  = rm -f

##########################################################################
# Compiler settings, parameters and flags
##########################################################################

# Compiler Options
GCFLAGS  = -c 
GCFLAGS += -std=gnu99 
GCFLAGS += -g 
GCFLAGS += -O$(OPTIMIZATION) 
GCFLAGS += $(INCLUDE_PATHS) 
GCFLAGS += -Wall 
GCFLAGS += -mthumb 
GCFLAGS += -ffunction-sections 
GCFLAGS += -fdata-sections 
GCFLAGS += -fmessage-length=0 
GCFLAGS += -fno-builtin 
GCFLAGS += -mcpu=cortex-m0 
GCFLAGS += -DTARGET=$(TARGET)
# For use with the GCC ARM Embedded toolchain
# GCFLAGS += --specs=nano.specs

# Assembler Options
ASFLAGS  = -c 
ASFLAGS += -g 
ASFLAGS += -O$(OPTIMIZATION) 
ASFLAGS += $(INCLUDE_PATHS) 
ASFLAGS += -Wall 
ASFLAGS += -mthumb 
ASFLAGS += -ffunction-sections 
ASFLAGS += -fdata-sections 
ASFLAGS += -fmessage-length=0 
ASFLAGS += -mcpu=cortex-m0 
ASFLAGS += -D__ASSEMBLY__ 
ASFLAGS += -x assembler-with-cpp

# Linker Options
LDFLAGS  = -nostartfiles 
LDFLAGS  = -nostdlib
LDFLAGS += -mcpu=cortex-m0 
LDFLAGS += -mthumb 
LDFLAGS += -O$(OPTIMIZATION) 
LDFLAGS += -Wl,--gc-sections 
LDFLAGS += -T $(LDSCRIPT)

# External Libraries
LDLIBS   = -lm -lgcc
# The following libraries are required with the LPCXpresso toolchain
# LDLIBS  += -lcr_c -lcr_eabihelpers

OCFLAGS = --strip-unneeded

##########################################################################
# Rules
##########################################################################

all: firmware

$(OBJ_PATH)/%.o : %.c
	@mkdir -p $(dir $@)
	-@echo "COMPILING $(@F)"
	@$(CC) $(GCFLAGS) -o $@ $<

$(OBJ_PATH)/%.o : %.s
	@mkdir -p $(dir $@)
	-@echo "ASSEMBLING $(@F)"
	@$(AS) $(ASFLAGS) -o $@ $<

firmware: $(OBJS) $(SYS_OBJS)
	@mkdir -p $(BIN_PATH)
	-@echo ""
	-@echo "LINKING $(OUTFILE).elf (-O$(OPTIMIZATION))"
	@$(LD) -o $(OUTFILE).elf $(OBJS) $(LDLIBS) $(LDFLAGS)
	-@echo ""
	@$(SIZE) $(OUTFILE).elf
	-@echo ""
	-@echo "Generating $(OUTFILE).hex"
	@$(OBJCOPY) $(OCFLAGS) -O ihex $(OUTFILE).elf $(OUTFILE).hex
	-@echo "Generating $(OUTFILE).bin"
	@$(OBJCOPY) $(OCFLAGS) -O binary $(OUTFILE).elf $(OUTFILE).bin
	-@echo ""

clean:
	@$(REMOVE) $(OBJS) $(OUTFILE).elf $(OUTFILE).bin $(OUTFILE).hex

#########################################################################
