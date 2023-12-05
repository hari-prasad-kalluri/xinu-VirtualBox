#include <xinu.h>

inline void clflush(void *p) {
    asm volatile ("clflushopt (%0)" :: "r"(p));
}

void stkflush(pid32 pid) {
	// kprintf("stkflush started\n");
	struct procent *prptr = &proctab[pid];
	
	char* start = prptr->prstkptr;
	char* end = prptr->prstkbase;

	unsigned long clflush_size = 32;

    char* current = (char *)((unsigned long)start & ~(clflush_size - 1));

	while(current <= end) {
		clflush((void *)current);
		// kprintf("stack flushing\n");
		current += clflush_size;
	}
	// kprintf("stkflush ended\n");
}
