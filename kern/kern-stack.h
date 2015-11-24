#ifndef KERN_STACK
#define KERN_STACK
#define KSTACK_SIZE     8192    /* or some other value */
#ifndef __ASSEMBLER__
#include "kern-stack.h"
#include "config.h"
extern unsigned char cpu_stack[CPU_NUM][KSTACK_SIZE];
#else   /* __ASSEMBLER__ */
#endif  /* !__ASSEMBLER__ */


#endif
