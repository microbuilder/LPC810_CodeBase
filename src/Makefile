#
# gcc Makefile for LPC810
# Kamal Mostafa <kamal@whence.com>
#
# License: Public Domain
#

CROSS = arm-none-eabi-
CPU = -mthumb -mcpu=cortex-m0plus -MD
FPU = # -mfloat-abi=soft

CFLAGS+= -Os -ffunction-sections -fdata-sections --specs=nano.specs

CC = ${CROSS}gcc
LD = ${CROSS}ld
OBJCOPY = ${CROSS}objcopy

# use these on the $(LD) line, as needed:
LIBM = ${shell ${CC} ${CFLAGS} --print-file-name=libm.a}
LIBC = ${shell ${CC} ${CFLAGS} --print-file-name=libc.a}
LIBGCC = ${shell ${CC} ${CFLAGS} --print-libgcc-file-name}

CFLAGS += $(CPU) $(FPU)
LDFLAGS += --gc-sections


INC += -I../cmsis
CFLAGS += $(INC) -D__USE_CMSIS # -DCLOCK_SETUP=1

BLINKY_OBJS=main.o gpio.o uart.o mrt.o printf.o printf-retarget.o \
     system_LPC8xx.o gcc_startup_lpc8xx.o


all: blinky.bin


blinky.axf: blinky.ld $(BLINKY_OBJS)
	$(LD) -o $@ $(LDFLAGS) -T blinky.ld $(BLINKY_OBJS) $(LIBGCC)

clean:
	rm -f *.o *.d
	rm -f blinky.axf blinky.bin

%.bin:%.axf
	$(OBJCOPY) ${@:.bin=.axf} -O binary $@
