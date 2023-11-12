/* cpu.h */

#define NCPU 4	/* number of cores simulation */

/* Hold current pid executing on each processor */

struct cpuent {
	pid32 cpid;					/* ID of currently executing process */
	pid32 ppid;					/* ID of previously executing process */
	uint32 preempt;				/* count 5 ms for cpu */ 
};

extern 	struct	cpuent	cputab[];
extern	cid32	currcpu;	/* Currently executing process		*/

#define CPU_NONE	-1
#define INITCPU		0

#define isbadcid(x)	(x < 0 || x >= NCPU)

