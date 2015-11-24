#include "sys/types.h"
#include "stddef.h"
#include "stdarg.h"
#include "stdio.h"
#include "config.h"
#include <asm/smp.h>

// todo: fix it
//#define cpuid() 9

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
        int i,j;
        kprintf("Hello from CPU #%d!\n", cpuid());
        for(;;);
        kputs("hehehe\n");
        for (i=0;1;++i){
            for(j=0;j<999;++j)
            kputs("slave\n");
        }
                /* nothing */;
}

void smp_startup(){
    unsigned int i;
    volatile unsigned int **mailbox_addr;
    mailbox_addr = (volatile unsigned int **)MSIM_ORDER_MAILBOX_BASE_VA;
    kprintf("Mailbox addr %x\n",mailbox_addr);
    for(i=1;i<CPU_NUM;++i){
        kprintf("starting %d!\n",i);
        kprintf("Addr:%x,DATA:%x,%x %x\n",((int)mailbox_addr+(i<<4)),*(char *)(mailbox_addr+(i<<4)),mailbox_addr,(i<<4));
        *(int **)((int)mailbox_addr+(i<<4)) =(int *)__slave_entry;
        kprintf("Addr:%x,DATA:%x,%x %x\n",((int)mailbox_addr+(i<<4)),*(char *)(mailbox_addr+(i<<4)),mailbox_addr,(i<<4));
    }

}
int main(void){
    kputs("Hello kern MAIN!!\n");
    smp_startup();
    kputs("end:);");
    for(;;);
}


