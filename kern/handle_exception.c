#include "sys/types.h"
#include "stddef.h"
#include "stdarg.h"
#include "stdio.h"
#include "config.h"
///#include "kstdio.h"
#include <asm/smp.h>
#include <asm/spinlock.h>
#include "RTC.h"
#include "stackframe.h"

void handle_exception(struct trapframe *tf){
    kputs("here is handle_exception\n");
    while(1);
}

