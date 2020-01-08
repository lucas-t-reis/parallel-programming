/* Even though OpenMP implicitly controls the team of threads in loops and parallel regions, it might be useful to delegate
tasks in another manner, given you know that there will be no conflict between threads in different parts of the code. The nowait
clause is necessary to "break" the barrier that is automatically imposed in @omp for loop.

nowait - each threads executes without needing to wait and sync with others - works on the given section under the clause
pragma barrier - explicit call to force threads wait for other to reach the same point
pragma master - the thread that spawned the team is assigned to a unique task
pragma single - first to finish gets assigned a unique task

*/

#include <cstdio>
#include <omp.h>

void doWork(int t) {
	printf("thread %d doing even more work\n", t);
}

int main() {

	int myId;
	int nthreads = omp_get_max_threads(); 

	printf("Removing implicit barrier at the end of the loop\n");

#pragma omp parallel private(myId)
{
	#pragma omp for nowait
	for(int i=0; i<nthreads; i++) {
		myId = omp_get_thread_num();
		printf("Doing work at %d\n", myId);
	}
	
	myId = omp_get_thread_num();
	doWork(myId);
}
printf("\n");

	printf("Using explicit barrier to join team of threads\n");
#pragma omp parallel private(myId)
{
	#pragma omp for nowait
	for(int i=0; i<nthreads; i++) {
		myId = omp_get_thread_num();
		printf("Doing work at %d\n", myId);
	}
	
	#pragma omp barrier
	{
	myId = omp_get_thread_num();
	doWork(myId);
	}

}
	printf("\n");

	printf("Removing barrier and assigning task to master\n");
#pragma omp parallel private(myId)
{
	#pragma omp for nowait
	for(int i=0; i<nthreads; i++) {
		myId = omp_get_thread_num();
		printf("Doing work at %d\n", myId);
	}
	
	#pragma omp master 
	{
		// I know it will be 0, this is just to double check the clause is working
		myId = omp_get_thread_num(); 		
		doWork(myId);
	}

}
	printf("\n");

	printf("First thread to finish gets to execute a @single block of code\n");

#pragma omp parallel private(myId)
{
	#pragma omp for nowait
	for(int i=0; i<nthreads; i++) {
		myId = omp_get_thread_num();
		printf("Doing work at %d\n", myId);
	}
	
	#pragma omp single
	{
		int thread = omp_get_thread_num();
		printf("#%d finished first and so it will do work!\n", thread);
		doWork(thread);
	}
		
}

}
