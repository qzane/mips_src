/* entry.S */
/*
 * Note: this entry point is for kernel, not firmware.
 * Put this file at a reasonable position.  You can also change the file name
 * according to your flavor.
 */
#include <asm/asm.h>
#include <asm/regdef.h>
#include <asm/cp0regdef.h>

NESTED(__start, 0, ra)
        mfc0    a0, CP0_STATUS
        ori     a0, ST_ERL
        xori    a0, ST_ERL
        mtc0    a0, CP0_STATUS
        LA      sp, cpu_stack
        ADDIU   sp, KSTACK_SIZE
        jal     main
END(__start)

