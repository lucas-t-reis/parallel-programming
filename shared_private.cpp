/* Exploring the private and shared clauses. Even though in most cases it is done by default when starting a 
parallel region/loop, sometimes it may be useful to explicit variables that are shared - default - from private ones.
Side notes: 
1 - Defining N to a multiple of @thread_num evenly distributes the task load, while having another value will result
in some threads with less/more work than the others.
2 - A thread start isn't necessarily followed by it's execution.  In some ocasions it can start early and finish late
*/

#include <cstdio>
#include <omp.h>

#define N 100 

int main() {

	float a[N], b[N], c[N];
	int thread_num, tId;

	for(int i=0; i<N; i++)
		a[i] = b[i] = i;

	// Initializing parallel region
	#pragma omp parallel shared(a,b,c) private(tId) 
	{
		tId = omp_get_thread_num();
		
		if(tId == 0) {
			thread_num = omp_get_num_threads();
			printf("Number of threads %d\n", thread_num );
		}

		printf("Thread #%d starting...\n", tId);

		// Initializing parallel loop
		#pragma omp for 
		for(int i=0; i<N; i++){
			c[i] = a[i] + b[i];
			printf("Thread %d: c[%d] = %f\n", tId, i, c[i]);
		}
	}
}
