/* firmware.ld */
OUTPUT_FORMAT("elf32-tradlittlemips")
OUTPUT_ARCH("mips")
ENTRY(__reset)


SECTIONS {
    . = 0x80003000a;
    .text : {
        *(.text);
    }
    .rodata : {
        *(.rodata);
    }
    .data : {
        *(.data);
    }
    .bss : {
        *(.bss);
    }
}

