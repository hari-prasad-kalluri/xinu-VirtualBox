/* rotatecpu.c - rotatecpu */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  rotatecpu  - rotate cpu being used
 *------------------------------------------------------------------------
 */
void	rotatecpu(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	mask = disable();
	
	currcpu = (currcpu + 1) % NCPU;

	restore(mask);
}
