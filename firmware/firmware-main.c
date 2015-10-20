#include "sys/types.h"
#include "stddef.h"
#include "stdarg.h"
#include "stdio.h"

extern char __rodata_end__[];
extern char __data_begin__[],__data_end__[];
extern char __bss_begin__[],__bss_end__[];

void reloc_data_bss(void){
   memcpy(__data_begin__, __rodata_end__, __data_end__ - __data_begin__);
   memset(__bss_begin__, 0, __bss_end__ - __bss_begin__);
}

int kprintf(char *fmt,...);
void kputs(const char *buf);

int g = 6;

void change_global_data(void){
   kprintf("g = %d\n",g);
   kprintf("addr g = %x\n",&g);
   g = 8;
   kprintf("g = %d\n",g);
}

int main(void){
    int a=34;
   //while(1)
    reloc_data_bss();
    kputs("Hello there!\n");
    change_global_data();
    kprintf("One: %d Hex of 34: 0x%x done\n",1,a);
    while(1);/* endless loop */
    return 0;
}

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
    volatile unsigned char *pointer = (volatile unsigned char *)0xbf000010;
    for(; *buf != '\0';++buf)
        *pointer = *buf;

}

