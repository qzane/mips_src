#include "sys/types.h"
#include "stddef.h"
#include "stdarg.h"
#include "stdio.h"

int kprintf(char *fmt,...){
   int res;
   char buffer[BUFSIZ];
   va_list ap;
   va_start(ap,fmt);
   res = vsnprintf(buffer,BUFSIZ,fmt,ap);
   kputs(buffer);
   va_end(ap);  
   return res;
}

void kputs(const char *buf){
    // Printer: 0x1f000010 | 0xbf000010 
    volatile unsigned char *pointer = (volatile unsigned char *)(PA_PRINTER+0xa0000000);
    for(; *buf != '\0';++buf)
        *pointer = *buf;

}

int main(void){
    kputs("Hello kern!\n");
}


