/* As we can see, using the @task clause allows for traversal of non-contiguous structures, such as the list in this example. Other than that
It is useful in situations where the work should be "coordinated" by a master and executed by the slaves. Here, the master thread walks trough
the data structure, assign the work for a given iteration to a queue and moves on instead of waiting for the result.
*/

#include <cstdio>
#include "timer.h"
#include <list>
#include <stdlib.h>

#define MAX 100
std::list<int> l;

int work(int n) {
	
	if(n==0) return 0;
	else if (n==1 || n==2) return 1;
	else return work(n-1) + work(n-2);
}

int main() {

	long long sum = 0;
	for(int i=0; i<MAX; i++)
		l.push_back(35);
	
	{ Timer t1("Task");
		
		// Initializing parallel region
		#pragma omp parallel
		{
			// First to arrive gets to command the others
			#pragma omp single
			{
				auto it = l.begin();
				while(it != l.end()) {
					// Which work should be distributed
					#pragma omp task
					{
					sum += work(*it);
					}
					// Only the master thread does this
					it++;
				}
			}
		}
	}

	std::cout << sum << std::endl;
}
