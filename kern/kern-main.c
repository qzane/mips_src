#include "sys/types.h"
#include "stddef.h"
#include "stdarg.h"
#include "stdio.h"
#include "config.h"
#include <asm/smp.h>
#include <asm/spinlock.h>
#include <asm/mipsregs.h> ///todo: i don't think it should appear here


// todo: fix it
//#define cpuid() 9

extern void __slave_entry(void);

int lock_test1=0;
int lock_test2=0;
struct spinlock lock;

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
        for(i=0;i<1000;++i)
            lock_test1=lock_test1+1;
        for(i=0;i<1000;++i){
            spinlock_unlock(&lock);//to test cpuid
            spinlock_lock(&lock);
            lock_test2=lock_test2+1;
            spinlock_unlock(&lock);
        }
        kprintf("CPU %d finished\n", cpuid());
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
        *(volatile unsigned int **)((unsigned int)mailbox_addr+(i<<4)) =(unsigned int *)__slave_entry;
        kprintf("Addr:%x,DATA:%x,%x %x\n",((int)mailbox_addr+(i<<4)),*(char *)(mailbox_addr+(i<<4)),mailbox_addr,(i<<4));
    }
    for(i=0;i<99999;++i);
    kprintf("lock_test1(without lock) %d\n",lock_test1);
    kprintf("lock_test2(with lock) %d\n",lock_test2);
}
int main(void){
    kputs("Hello kern MAIN!!\n");
    smp_startup();
    kputs("smp_startup end\n");
    for(;;);
}


