#include <cstdio>
#include <omp.h>

int main() {
	
	int tId, nThreads;
	int master = 0;

	// Spawning threads, each one with its own @tId and @nThreads
	#pragma omp parallel private(tId, nThreads)
	{
	
	tId = omp_get_thread_num();

	printf("Hello parallel world, from thread n# %d\n", tId);
	
	if(tId == master) printf("# of threads %d\n", nThreads);
	
	} // Reunite threads

	
}
