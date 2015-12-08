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

volatile uint32_t time();
volatile uint32_t utime();
void usleep(unsigned int ms);
void sleep(unsigned int s);
void show_count_inc();
void RTC_MAIN();
#endif
