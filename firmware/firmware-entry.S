/* firmware-entry.S */
#include <asm/asm.h>
#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <config.h>
#include <asm/smp.h>

NESTED(__reset, 0, ra) /* 0xbfc00000 */
    mfc0 a0, CP0_STATUS
    ori a0, ST_ERL
    xori a0, ST_ERL
    mtc0 a0, CP0_STATUS
    cpuid a1, a2
    bnez a1, __slave_entry /* for slave cpus */
    li sp, FIRMWARE_STACK_ADDR
    jal main
END(__reset)
    .align 9
LEAF(__tlbrefill) /* 0xbfc00200 */
/* Note that this function is empty */
END(__tlbrefill)
    .align 8
LEAF(__cacheerror) /* ??? */
    jal __generic_exception
END(__cacheerror)

__generic_exception:
nop
jal __generic_exception

__slave_entry:
    li      a1, MSIM_ORDER_MAILBOX_BASE_VA
    sw      zero, (a1)
1:  lw      a0, (a1)
    beqz    a0, 1b
    jr      a0

