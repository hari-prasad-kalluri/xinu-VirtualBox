/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;

void print_process_table(){
	int i=0;
	for (i = 0; i < NPROC; i++) {
		if (proctab[i].prstate != PR_FREE) {
			kprintf("PID: %d State: %d CPU: %d Name: %s\n", i, proctab[i].prstate, proctab[i].prcpu, proctab[i].prname);
		}
	}
}

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/
	struct cpuent *cpuold;		/* Ptr to cpu entry	 */
	struct cpuent *cpunew;		/* Ptr to cpu entry	 */

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	kprintf("currcpu: %d  currpid: %d  preempt: %d clkcountermsec: %d\n", currcpu, currpid, preempt, clkcountermsec);
	print_process_table();

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];
	cpuold = &cputab[currcpu];

	//Rotate CPU
	rotatecpu();

	cpunew = &cputab[currcpu];

	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;
	}
	else {
		cpuold->cpid = NOPROC;
	}

	if (!isbadpid(cpunew->cpid) && proctab[cpunew->cpid].prstate == PR_READY && proctab[cpunew->cpid].prprio <= firstkey(readylist)) {
		/* Force context switch to highest priority ready process */

		currpid = dequeue(readylist);
		insert(cpunew->cpid, readylist, proctab[cpunew->cpid].prprio);
	}
	else if(!isbadpid(cpunew->cpid) && proctab[cpunew->cpid].prstate == PR_READY) {
		currpid = cpunew->cpid;
	}
	else {
		/* Force context switch to highest priority ready process */
		currpid = dequeue(readylist);
		if (currpid == EMPTY) {
			currpid = NULLPROC;
		}
	}
	cpunew->ppid = cpunew->cpid;		/* record previous process		*/
	cpunew->cpid = currpid;	/* get and record new process	*/
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	if(ptnew->prcpu != currcpu) {
		//perform cache flush
		stkflush(currpid);
	}
	ptnew->prcpu = currcpu;
	preempt = cpunew->preempt;		/* Reset time slice for process	*/

	


	kprintf("currcpu: %d  currpid: %d  preempt: %d clkcountermsec: %d\n", currcpu, currpid, preempt, clkcountermsec);

	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
