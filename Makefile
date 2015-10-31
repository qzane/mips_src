# Makefile (top-level)

CROSS_COMPILE   = mips-linux-gnu-

CC              = $(CROSS_COMPILE)gcc
LD              = $(CROSS_COMPILE)ld
OBJCOPY         = $(CROSS_COMPILE)objcopy

AS              = $(CC)
export

all:
	$(MAKE) -C firmware
	$(MAKE) -C mbr
	$(MAKE) -C kern
	# more subdirectories...

clean:
	$(MAKE) -C firmware clean
	$(MAKE) -C mbr clean
	$(MAKE) -C kern clean
	# more subdirectories...

msim:
	$(MAKE) -C firmware msim
	$(MAKE) -C mbr msim
	$(MAKE) -C kern msim
	#sth more
run: msim
	msim -c ./msim/msim.conf


.PHONY: all clean msim


