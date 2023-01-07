/* 
Using firstprivate() makes each thread use a given variable with it's original value (initialized) instead of mem trash - using
private() - and lastprivate() assures that the values calculated by a thread at the last iteration of a loop or a section (from @sections)
will be assigned to the private variable, effectively changing it. It might be useful for programs that depends on the last value
calculated from an array or function.. 
*/

#include <omp.h>
#include <cstdio>

int main() {

	
	int tid;
	int x = 42;

	printf("----------SETTING X AS FIRSTPRIVATE----------\n");
	// While you can use firstprivate here, you can't do the same for lastprivate, since it relies on the last step of an iteration
	// or section and not the last thread to execute.
	#pragma omp parallel firstprivate(x) private(tid)
	{

		tid = omp_get_thread_num();
		printf("Thread #%d start..\n", tid);
		x += tid; // 42 + threadId

		printf("Thread #%d x = %d\n", tid, x);


	}

	printf("Final value of x : %d\n\n", x);

	int y = 42;
	printf("----------SETTING Y AS LASTPRIVATE----------\n");
	#pragma omp parallel private(tid)
	{

		tid = omp_get_thread_num();
		printf("Thread #%d start..\n", tid);

		#pragma omp for lastprivate(y)
		for(int i=0; i<32; i++){
			
			y = tid;
			printf("Thread #%d and i %d : y = %d\n", tid,i, y);

		}

	}
	
	printf("Final value of y: %d\n", y);

	
	
	
}
