/* In this example you can see that there is not particular
order of threads for execution - race condition. So the "# of threads" 
print can occur anywhere and you can see the "Hello parallel... thread #" 
with random thread identifications from (0,1,...,nThreads) */

#include <cstdio>
#include <omp.h>

int main() {
	
	int tId, nThreads;
	int master = 0;

	// Spawning threads, each one with its own @tId and @nThreads
	// which is possible due to the private(var1, var2, ..., varN) function
	#pragma omp parallel private(tId, nThreads)
	{
	
		tId = omp_get_thread_num();

		printf("Hello parallel world, from thread #%d\n", tId);

		// Assuming master threads to be 0
		if(tId == master) {
			nThreads = omp_get_num_threads();
			printf("# of threads %d\n", nThreads);
		}

	} // Reunite threads

}
