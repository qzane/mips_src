# Makefile (top-level)

CROSS_COMPILE   = mips-linux-gnu-

CC              = $(CROSS_COMPILE)gcc
LD              = $(CROSS_COMPILE)ld
OBJCOPY         = $(CROSS_COMPILE)objcopy

AS              = $(CC)
export

all:
	$(MAKE) -C firmware
	# more subdirectories...

clean:
	$(MAKE) -C firmware clean
	# more subdirectories...

msim:
	$(MAKE) -C firmware msim
	#sth more

.PHONY: all clean


