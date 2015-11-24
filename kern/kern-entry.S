/* entry.S */
/*
 * Note: this entry point is for kernel, not firmware.
 * Put this file at a reasonable position.  You can also change the file name
 * according to your flavor.
 */
#include <asm/asm.h>
#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/smp.h>
#include <kern-stack.h>
//#define KSTACK_SIZE     8192 

NESTED(__start, 0, ra)
        mfc0    a0, CP0_STATUS
        ori     a0, ST_ERL
        xori    a0, ST_ERL
        mtc0    a0, CP0_STATUS
        LA      sp, cpu_stack
        ADDIU   sp, KSTACK_SIZE
        jal     main
END(__start)


LEAF(__slave_entry)
        mfc0    a0, CP0_STATUS
        ori     a0, ST_ERL
        xori    a0, ST_ERL
        mtc0    a0, CP0_STATUS
		cpuid 	a1, a2 /* get cpuid in a1*/
        ADDIU   a1, 1
		/*sll     a2, a1, 13 /* KSTACK_SIZE == 8192*/
        ADDIU   a2, a2, KSTACK_SIZE
        MUL     a1, a1, a2
        LA      sp, cpu_stack
        /* ADDIU   sp, KSTACK_SIZE */
        ADD     sp, a1
        jal     slave_main
END(__slave_entry)
