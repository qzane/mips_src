#ifndef _ASM_SPINLOCK_H
#define _ASM_SPINLOCK_H

#include <sys/types.h>
#include <asm/atomic.h>


/* ... */
struct spinlock {
        uint32_t        state;
#define LOCK_BIT        0 
};
/* ... */


static void spinlock_lock(struct spinlock *lock)
{
	bool result;
	uint32_t *addr;
	uint32_t bit;
	addr = lock->state;
	asm volatile (
		"1:	ll	%[reg], %[mem];"
		"   and %[reg1], %[reg], %[val]"
		"   bne %[reg1], 1b;"
		"	or	%[reg], %[val];"
		"	sc	%[reg], %[mem];"
		"	beqz	%[reg], 1b;"
		: [reg]"=&r"(result), [mem]"+m"(*addr), [reg1]"=&r"(bit)
		: [val]"ir"(1 << LOCK_BIT)
	);
}
static void spinlock_unlock(struct spinlock *lock)
{
	uint32_t *addr;
	addr = lock->state;
	atomic32_clear_bit(addr,LOCK_BIT); 
}



#endif