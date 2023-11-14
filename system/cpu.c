/*  cpu.c */
#include <xinu.h>

/*------------------------------------------------------------------------
 *  cpuinit  -  Initialize CPUs
 *------------------------------------------------------------------------
 */
void cpuinit(void){
	uint32 i;				/* iterator over cores */
	struct cpuent* cpuptr;	/* pointer to cpu entry */

	for(i = 0; i < NCPU; i++){
		cpuptr = &cputab[i];
		
		/* Initialize current and previous processes */
		cpuptr->cpid = NOPROC;
		cpuptr->ppid = NOPROC;

		/* Set initial preemption time */
		cpuptr->preempt = QUANTUM;

	}

}

/*------------------------------------------------------------------------
 *  getcid  -  Return the ID of the currently executing core
 *------------------------------------------------------------------------
 */
cid32 getcid(void){
	return (currcpu);
}