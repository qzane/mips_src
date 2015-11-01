/* firmware.ld */
OUTPUT_FORMAT("elf32-tradlittlemips")
OUTPUT_ARCH("mips")
ENTRY(__MBR)


SECTIONS {
    . = 0x80001000;
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
    /DISCARD/ : {
        *(.reginfo);
    }
}

