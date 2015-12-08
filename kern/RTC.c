#ifndef _RTC_H
#define _RTC_H
#include "sys/types.h"
#include <asm/smp.h>
#include <asm/mipsregs.h>
#include "kstdio.h"
#include "config.h"
#define MSIM_RTC_BASE  0x9f000090



#define COUNT_PER_MS 2020
#define COUNT_PER_S 2020740

volatile uint32_t time(){
    return *((volatile uint32_t*)MSIM_RTC_BASE);
}
volatile uint32_t utime(){//seemed not to work
    return *(((volatile uint32_t*)MSIM_RTC_BASE)+1);
}


void usleep(unsigned int ms){
    uint32_t count;
    count = read_c0_count() + ms*COUNT_PER_MS;
    while(read_c0_count()<count);
}
void sleep(unsigned int s){
    uint32_t count;
    count = read_c0_count()+s*COUNT_PER_S;
    while(read_c0_count()<count);
}


void show_count_inc(){
    uint32_t count;
    int i;
    count = read_c0_count();
    while(1){
        for(i=0;i<300000;++i);
        kprintf("count: %20ud\n",read_c0_count());
    }

}


void RTC_MAIN(){
    kprintf("This is RTC main! cpu:%d\n",cpuid());
    ///show_count_inc();
    while(1){
        sleep(5);
        kprintf("This is five second. Time:%u uTime:%u\n",time(),utime());
    }
}




#endif
