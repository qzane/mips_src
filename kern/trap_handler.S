#include <asm/asm.h>
#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/smp.h>
#include <kern-stack.h>
#include "stackframe.h"

NESTED(_TRAP_HANDLER, 0, ra)
    SAVE_ALL 
    # set ao=*trapframe???
    jal handle_exception
    RESTORE_ALL_AND_RET
END(_TRAP_HANDLER)




