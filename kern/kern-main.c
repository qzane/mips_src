#define PA_PRINTER 0x1f000010

void kputs(const char *buf){
    // Printer: 0x1f000010 | 0xbf000010 
    volatile unsigned char *pointer = (volatile unsigned char *)(PA_PRINTER+0xa0000000);
    for(; *buf != '\0';++buf)
        *pointer = *buf;

}

int main(void){
    kputs("Hello kern!\n");
    for(;;);
}


