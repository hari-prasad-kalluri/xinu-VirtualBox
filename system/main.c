/*  main.c  - main */

#include <xinu.h>

void cpu_bound(){
	while (clkcountermsec < 10000) {
        continue;
    }
}

void io_bound(){
	while (clkcountermsec < 10000) {
        int x, i;
        for (i = 0; i < 10000; i++) {
            x = i;
        }
        sleepms(10);
    }
}

process	main(void)
{
	pid32	shpid;		/* Shell process ID */

	printf("\n\n");

	/* Create a local file system on the RAM disk */

	lfscreate(RAM0, 40, 20480);

	/* Run the Xinu shell */

	recvclr();
	//resume(shpid = create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	/*
	while (TRUE) {
	    if (receive() == shpid) {
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(shpid = create(shell, 4096, 20, "shell", 1, CONSOLE));
	    }
	}
	*/
	resume(create(cpu_bound, 8192, 10, "cpu", 0));
	resume(create(cpu_bound, 8192, 10, "cpu", 0));
	resume(create(cpu_bound, 8192, 10, "cpu", 0));
	resume(create(io_bound, 8192, 10, "io", 0));
	resume(create(io_bound, 8192, 10, "io", 0));
	resume(create(io_bound, 8192, 10, "io", 0));
	return OK;
}
