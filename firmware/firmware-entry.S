/* firmware-entry.S */
#include <asm/asm.h>
#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <config.h>


NESTED(__reset, 0, ra) /* 0xbfc00000 */
    mfc0 a0, CP0_STATUS
    ori a0, ST_ERL
    xori a0, ST_ERL
    mtc0 a0, CP0_STATUS
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



