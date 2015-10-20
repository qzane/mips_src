/* firmware.ld */
OUTPUT_FORMAT("elf32-tradlittlemips")
OUTPUT_ARCH("mips")
ENTRY(__reset)

MEMORY {
        rom (RX) : ORIGIN = 0xbfc00000, LENGTH = 1M
        ram (RW) : ORIGIN = 0x80000000, LENGTH = 256M
}


SECTIONS {
    .text : {
        firmware-entry.o(.text);
        *(.text);
    } >rom
    .rodata : {
        *(.rodata);
        __rodata_end__ = .;
    } >rom
    .data : AT(__rodata_end__){
        __data_begin__ = .;
        *(.data);
        __data_end__ = .;
    } >ram
    .bss : {
        __bss_begin__ = .;
        *(.bss);
        __bss_end__ = .;
    } >ram
    /DISCARD/ : {
        *(.reginfo);
    }
}

