#include "config.h"


void kputs(const char *buf){
    // Printer: 0x1f000010 | 0xbf000010 
    volatile unsigned char *pointer = (volatile unsigned char *)(PA_PRINTER+ADDR_PtoV2);
    for(; *buf != '\0';++buf)
        *pointer = *buf;

}

int main(void){
    kputs("Hello kern!\n");
    for(;;);
}


