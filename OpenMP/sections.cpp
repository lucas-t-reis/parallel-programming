/* Sections allow for distributing work among threads. It is implementation
defined which thread is assigned to what section and there is an implicit barrier at
the end of @sections to sync threads. It is specially useful if you need to paralellize
different workloads and not a loop*/

#include <cstdio>
#include <omp.h>

void work1(int id) { printf("work1 by thread %d\n", id); }
void work2(int id) { printf("work2 by thread %d\n", id); }
void work3(int id) { printf("work3 by thread %d\n", id); }
int main() {

	int thread;

	#pragma omp parallel sections private(thread) 
	{
		#pragma omp section
		{	
			thread = omp_get_thread_num();
			work1(thread);
		}
		
		#pragma omp section
		{	
			thread = omp_get_thread_num();
			work2(thread);
		}
		
		#pragma omp section
		{	
			thread = omp_get_thread_num();
			work3(thread);
		}
	}
}
