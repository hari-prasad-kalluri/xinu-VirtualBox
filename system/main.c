/*  main.c  - main */

#include <xinu.h>

uint32 cpu_endtime = 0;
uint32 io_endtime = 0;

uint32 stop_time = 1200000;

void cpu_bound(){
	while (clkcountermsec < stop_time) {
        continue;
    }
}

void io_bound(){
	while (clkcountermsec < stop_time) {
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

	int num_iter=0, billion_counter=0;

	while(clkcountermsec < stop_time) {
		for(i=0; i<arr_size; i++) {
			big_arr[i] += 1;
			num_iter++;
		}
		if(num_iter >= 1000000000){
			billion_counter++;
			num_iter = num_iter - 1000000000;
		}
	}
	intmask	mask;
	mask = disable();
	kprintf("Number of iterations: %d billion %d\n", billion_counter, num_iter);
	restore(mask);
}

void cpu_bound2(){
	int x, i, j;
	j = 0;
	while (j < 100) {
		for (i = 0; i < 1000000000; i++) {
			x = i;
		}
		j++;
	}
	cpu_endtime = clkcountermsec;
}

void io_bound2(){
	int j = 0;
	while (j < 10000) {
        int x, i;
        for (i = 0; i < 10000; i++) {
            x = i;
        }
        sleepms(1);
		j++;
    }
	io_endtime = clkcountermsec;
}

void test_benchmark2(){
	resume(create(cpu_bound2, 8192, 10, "cpu", 0));
	resume(create(cpu_bound2, 8192, 10, "cpu", 0));
	resume(create(cpu_bound2, 8192, 10, "cpu", 0));
	resume(create(io_bound2, 8192, 10, "io", 0));
	resume(create(io_bound2, 8192, 10, "io", 0));
	resume(create(io_bound2, 8192, 10, "io", 0));
	resume(create(mem_bound, 100000, 10, "io", 0));
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

	sleepms(stop_time - clkcountermsec + 1000);

	kprintf("Total time taken [cpu bound]: %d\n", cpu_endtime - begintime);
	kprintf("Total time taken [io bound]: %d\n", io_endtime - begintime);
	return OK;
}
