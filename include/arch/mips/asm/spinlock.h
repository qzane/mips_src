#ifndef _ASM_SPINLOCK_H
#define _ASM_SPINLOCK_H

#include <sys/types.h>
#include <asm/atomic.h>
#include <asm/smp.h>


/* ... */
struct spinlock {
        uint32_t        state;
#define LOCK_BIT        0 
#define LOCK_CPUID_OFFSET 1
#define LOCK_CPUID_MASK 0x0E
};
/* ... */


static void spinlock_lock(struct spinlock *lock)
{
	bool result;
	uint32_t *addr;
	uint32_t bit;
	addr = &(lock->state);
	asm volatile (
		"1:	ll	%[reg], %[mem];"
		"   and %[reg1], %[reg], %[val];"
		"   bnez %[reg1], 1b;"
		"	or	%[reg], %[val];"
		"	sc	%[reg], %[mem];"
		"	beqz	%[reg], 1b;"
		: [reg]"=&r"(result), [mem]"+m"(*addr), [reg1]"=&r"(bit)
		: [val]"ir"(1 << LOCK_BIT)
	);
	*addr |= (cpuid()<<LOCK_CPUID_OFFSET)&LOCK_CPUID_MASK;
}
static void spinlock_unlock(struct spinlock *lock)
{
	uint32_t *addr;
	addr = &(lock->state);
	if(((*addr&LOCK_CPUID_MASK)>>LOCK_CPUID_OFFSET)==cpuid()){
        *addr |= LOCK_CPUID_MASK;
        *addr ^= LOCK_CPUID_MASK;
		atomic32_clear_bit(addr,LOCK_BIT);
    }
}



#endif
