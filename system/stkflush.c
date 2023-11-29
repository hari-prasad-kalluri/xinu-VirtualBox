#include <xinu.h>

inline void clflush(void *p) {
    asm volatile ("clflushopt (%0)" :: "r"(p));
}

void stkflush(pid32 pid) {
	// kprintf("stkflush started\n");
	struct procent *prptr = &proctab[pid];
	
	void* start = prptr->prstkptr;
	void* end = prptr->prstkbase;

    unsigned long rem= ((unsigned long)start)&64;

	void* current = rem == 0 ? start: start - rem;

	while(current <= end) {
		clflush(current);
		current += 64;
		// kprintf("stack flushing\n");
	}
	// kprintf("stkflush ended\n");
}
