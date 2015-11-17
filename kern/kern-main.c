#include "config.h"


extern void __slave_entry(void);

void kputs(const char *buf){
    // Printer: 0x1f000010 | 0xbf000010 
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

void slave_main(void)
{
        kprintf("Hello from CPU #%d!\n", cpuid());
        for (;;)
                /* nothing */;
}

void smp_startup(){
    int i;
    int **mailbox_addr;
    mailbox_addr = (int **)MSIM_ORDER_MAILBOX_BASE_VA;
    for(i=1;i<CPU_NUM;++i){
        kprintf("starting %d!\n",i);
        *mailbox_addr =(int *)((unsigned int)__slave_entry+(i<<4));
    }

}
int main(void){
    kputs("Hello kern MAIN!!\n");
    smp_startup();
    for(;;);
}


