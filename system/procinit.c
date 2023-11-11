/* procinit.c - procinit */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  procinit  - Initialize process variables
 *------------------------------------------------------------------------
 */
status procinit(void){

	uint32 i;					/* iterator over proctab */
	struct	procent	*prptr;		/* Ptr to process table entry	*/

	/* Count the Null processes as the first processes in the system */

	prcount = 1;

	/* Initialize process table entries free */

	for (i = 0; i < NPROC; i++) {
		prptr = &proctab[i];
		prptr->prstate = PR_FREE;
		prptr->prname[0] = NULLCH;
		prptr->prstkbase = NULL;
		prptr->prprio = 0;
		prptr->prcpu = CPU_NONE;
	}

	/* Initialize the Null process entries */

	prptr = &proctab[NULLPROC];
	prptr->prstate = PR_CURR;
	prptr->prprio = 0;
	strncpy(prptr->prname, "prnull", 7);
	prptr->prstkbase = getstk(NULLSTK);
	prptr->prstklen = NULLSTK;
	prptr->prstkptr = 0;
	cputab[INITCPU].cpid = NULLPROC;
	cputab[INITCPU].defer.ndefers = 0;
	cputab[INITCPU].defer.attempt = FALSE;
	prptr->prcpu = INITCPU;
	currpid = NULLPROC;
	currcpu = INITCPU;

	return OK;
}
