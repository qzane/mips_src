#ifndef _RTC_H
#define _RTC_H
#include "sys/types.h"
#include <asm/smp.h>
#include <asm/mipsregs.h>
#include "kstdio.h"

#define COUNT_PER_MS 1981
#define COUNT_PER_S 1981117


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
        sleep(10);
        kprintf("This is ten second\n");
    }
}




#endif
