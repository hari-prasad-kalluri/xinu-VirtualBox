/*  cpu.c */
#include <xinu.h>

/*------------------------------------------------------------------------
 *  cpuinit  -  Initialize CPUs
 *------------------------------------------------------------------------
 */
void cpuinit(void){
	uint32 i;				/* iterator over cores */
	struct cpuent* cpuptr;	/* pointer to cpu entry */

	for(i = 1; i < NCPU; i++){
		cpuptr = &cputab[i];

		/* Scheduling is not currently blocked */
		cpuptr->defer.ndefers = 0;
		cpuptr->defer.attempt = FALSE;

		/* Initialize current and previous processes */
		cpuptr->cpid = NULLPROC;
		cpuptr->ppid = NULLPROC;

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