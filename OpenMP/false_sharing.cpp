/*
Code based on the example in :https://www.dartmouth.edu/~rc/classes/intro_openmp/parallel_regions.html#top with some minor changes.

Note that even though each thread has an unique id, they are in the same cache line, thus making it refresh everytime they perform 
R/W operations (cache trashing). To correct the issue, the following snippet should replace what's inside the parallel region.

#pragma omp parallel
{
	...
	
	double thread_sum = 0.0
	for(int idx=id; idx<num_steps; idx+=t_num) {
		x = (i + 0.5) * step;
		thread_sum += thread_sum +  4.0/(1+x*x);
	}

	#pragma omp critical
	full_sum += thread_sum
	
	...
}

Obs.: the integral probably comes from 
4*arctg(1) => 4 * d(arctg() )/dx => 4 * 1/(1+x^2)

*/

#include <stdio.h>
#include <omp.h>

#define MAX_THREADS 4
long long int num_steps = 100000000;
double step;

int main() {
	
	int i,j;
	double pi, full_sum = 0.0;
	double start, end;
	double sum[MAX_THREADS];

	step = 1.0/ (double) num_steps;

	// Testing with different number of threads
	for(j=1; j<=MAX_THREADS; j++) {

		omp_set_num_threads(j);
		full_sum = 0.0;
		start = omp_get_wtime(); // Get starting wall clock time
	

		#pragma omp parallel
		{
			int id = omp_get_thread_num();
			int t_num = omp_get_num_threads();
			double x;
			sum[id] = 0.0;
			
			// This is where the false sharing occurs.
			for(int idx=id; idx<num_steps; idx+=t_num) {
				
				x = (idx+0.5)*step;
				sum[id] = sum[id] + 4.0/(1.0 + x*x);

			}
		}

	for(full_sum = 0.0, i=0; i<j; i++) full_sum += sum[i];
	pi = step * full_sum;
	end = omp_get_wtime() - start;
	printf("\npi is %f in %f second with %d threads\n", pi,end, j);

	}
}
