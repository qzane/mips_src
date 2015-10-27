# Makefile
CROSS_COMPILE   = mips-linux-gnu-

CC              = $(CROSS_COMPILE)gcc
LD              = $(CROSS_COMPILE)ld
OBJCOPY         = $(CROSS_COMPILE)objcopy

AS              = $(CC)


INCS            = -I. -I../include -I../include/arch/mips

CFLAGS          = -DELF32  -EL -nostdinc -nostdlib -mabi=32 -mips32 -fno-pic -g \
                  -mno-abicalls $(INCS)

ASFLAGS         = $(CFLAGS)

LDSCRIPT        = mbr.ld

ELF             = mbr
BINARY          = mbr.bin
# Take caution of the order, if you didn't make the required changes
# in linker script in previous exercise.
# Add snprintf.o to the list if you have date & time printing
OBJS            = bootsect.o
DISK            = ../disk.img

# See if you can make further changes to your own variables.

$(BINARY): $(ELF)
	$(OBJCOPY) -O binary -j .text -j .rodata -j .data -j .bss $(ELF) $(BINARY)

$(ELF): $(OBJS)
	$(LD) -T $(LDSCRIPT) -o $(ELF) $(OBJS)

# firmware-main.o is omitted and being processed by default rules.
# And so is snprintf.o

# You should have already removed firmware-entry.o rule by now according to
# the above exercise.

all: $(BINARY)

clean:
	rm -f $(OBJS) $(ELF) $(BINARY)
msim: all
	if [ $(shell wc -c $(BINARY)  | cut -f 1 -d " ") -ge 446 ]; then \
		 echo Bootloader too large; \
	else \
		 dd if=$(BINARY) of=$(DISK) conv=notrunc;\
	fi

.PHONY: all clean 

