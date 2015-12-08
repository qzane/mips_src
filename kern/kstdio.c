#ifndef _KSTDIO_H
#define _KSTDIO_H
#include "stdio.h"
#include "config.h"
void kputs(const char *buf){    
        volatile unsigned char *pointer = (volatile unsigned char *)(PA_PRINTER+ADDR_PtoV2);
            for(; *buf != '\0';++buf)
                        *pointer = *buf;

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

#endif
