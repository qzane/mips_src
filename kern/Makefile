# Makefile
INCS            = -I. -I../include -I../include/arch/mips

CFLAGS          = -EL -nostdinc -nostdlib -mabi=32 -mips32 -fno-pic -g \
                  -mno-abicalls $(INCS)

ASFLAGS         = $(CFLAGS)

LDSCRIPT        = kern.ld

ELF             = kern
BINARY          = kern.bin
# Take caution of the order, if you didn't make the required changes
# in linker script in previous exercise.
# Add snprintf.o to the list if you have date & time printing
OBJS            = kern-entry.o \
                  kern-main.o \
                  ../libc/snprintf.o \
                  ../libc/memcpy.o \
                  ../libc/memset.o
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
msim: $(BINARY)
	dd if=$(BINARY) of=$(DISK) bs=512 seek=1 conv=notrunc

.PHONY: all clean msim
