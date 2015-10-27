/* firmware.ld */
OUTPUT_FORMAT("elf32-tradlittlemips")
OUTPUT_ARCH("mips")
ENTRY(__reset)


SECTIONS {
    .0x80003000;
    .text : {
        *(.text);
    } >rom
    .rodata : {
        *(.rodata);
    } >rom
    .data : {
        *(.data);
    }
    .bss : {
        *(.bss);
    }
}

