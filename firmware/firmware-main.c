#include "sys/types.h"
#include "stddef.h"
#include "stdarg.h"
#include "stdio.h"

int kprintf(char *fmt,...);
void kputs(const char *buf);


int main(void){
    int a=34;
    //while(1)
    kputs("Hello there!");
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

