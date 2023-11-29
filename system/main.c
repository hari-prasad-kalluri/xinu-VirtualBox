/*  main.c  - main */

#include <xinu.h>

uint32 endtime = 0;

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
	int arr_size=10000;
	int big_arr[arr_size];

	int i=0;
	for(;i<arr_size;i++){
		big_arr[i] = 0;
	}

	int num_iter=0, max_iter=1000;

	while(num_iter < max_iter) {
		for(i=0; i<arr_size; i++) {
			big_arr[i] += 1;
		}
	}

	endtime = clkcountermsec;
}

void cpu_bound2(){
	int x, i, j;
	j = 0;
	while (j < 50) {
		for (i = 0; i < 1000000000; i++) {
			x = i;
		}
		j++;
	}
	endtime = clkcountermsec;
}

void io_bound2(){
	int j = 0;
	while (j < 10000) {
        int x, i;
        for (i = 0; i < 10000; i++) {
            x = i;
        }
        sleepms(5);
		j++;
    }
	endtime = clkcountermsec;
}

void test_benchmark2(){
	resume(create(cpu_bound2, 8192, 10, "cpu", 0));
	resume(create(cpu_bound2, 8192, 10, "cpu", 0));
	resume(create(io_bound2, 8192, 10, "io", 0));
	resume(create(io_bound2, 8192, 10, "io", 0));
	resume(create(mem_bound, 100000, 10, "io", 0));
	resume(create(mem_bound, 100000, 10, "io", 0));
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
	/*
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
	*/
	uint32 begintime = clkcountermsec;

	test_benchmark2();

	sleep(2000);

	kprintf("Total time taken: %d\n", endtime - begintime);
	return OK;
}
