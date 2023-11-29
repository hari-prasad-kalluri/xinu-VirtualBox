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

void mem_bound() {
	int big_arr[100000];

	int i=0;
	for(;i<100000;i++){
		big_arr[i] = 0;
	}

	int num_iter=0, max_iter=1000;

	while(num_iter < max_iter) {
		for(i=0; i<100000; i++) {
			big_arr[i] += 1;
		}
	}

	return;
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
	kprintf("Process creation started\n");
	resume(create(cpu_bound, 8192, 10, "cpu", 0));
	resume(create(cpu_bound, 8192, 10, "cpu", 0));
	resume(create(cpu_bound, 8192, 10, "cpu", 0));
	resume(create(io_bound, 8192, 10, "io", 0));
	resume(create(io_bound, 8192, 10, "io", 0));
	resume(create(io_bound, 8192, 10, "io", 0));
	resume(create(mem_bound, 1000000, 10, "io", 0));
	resume(create(mem_bound, 1000000, 10, "io", 0));
	resume(create(mem_bound, 1000000, 10, "io", 0));
	kprintf("Process creation done\n");
	return OK;
}
