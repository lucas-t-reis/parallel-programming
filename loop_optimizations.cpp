/* 

When opting for a loop-level parallelization, there must be no dependencies when determining the value of each element in the
array, meaning you shouldn't mess with indexes different of your own. Another thing important to notice is that the speedup seems
to be greater if the cost of the operation is significant - something higher than O(n). Maybe this is due to the overhead of the
parallelization process.

Also worth mentioning, altough you can fix the dependency issue on a shared variable with critical, atomic or reduce, things are
not that simple when it comes to manipulating indexes different from your own - you can get an index from a region that does not
belong to your thread and has not been modified yet by the other thread.

The improvement over the normal time was calculated using the following formula : ceil(improvement = normal_time/parallel_time)

*/

#include <omp.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>

#define MAX 100000000
int randNums[MAX];
int randA[MAX];
int randB[MAX];
int randC[MAX];

int factorial(int n) {

	if(n==0) return 1;
	else return n*factorial(n-1);

}

int main() {

	srand(time(NULL));
	
	using namespace std::chrono;
	high_resolution_clock::time_point start;
	high_resolution_clock::time_point end;
	duration<double> time_span_N;
	duration<double> time_span_P;
	
	// Filling the arrays with random elements
	for(int i=0; i<MAX; i++){
		randA[i] = rand()%1000;
		randB[i] = rand()%100;
		randC[i] = rand()%10 + 1; // From 1 ~ 10
	}

	// Let MAX = n for the Order of Complexity representation

	long long int sumN_IndexOperations = 0;
	long long int sumN = 0;
	for(int i=0; i<MAX-4; i++){
		sumN_IndexOperations = randA[i] + randA[i+1] - randB[i+2]*randC[i+3];
		sumN += randA[i] + randB[i]*randC[i];
	}

	long long int sumP_IndexOperations = 0;
	long long int sumP = 0;
	#pragma omp parallel for // uncomment to make it give the correct result reduction(+:sumP) in sumP
		for(int i=0; i<MAX-4; i++){
			sumP_IndexOperations = randA[i] + randA[i+1] - randB[i+2]*randC[i+3];
			sumP += randA[i] + randB[i]*randC[i];
		}

	// Demonstrating race conditions / dependency problems
	std::cout << "Sum of all elements from arrays A,B and C\n";
	std::cout << "Normal: " << sumN << " and Parallel: " << sumP << "\n" << std::endl;
	std::cout << "Sum manipulating indexes from A,B and C\n";
	std::cout << "Normal: " << sumN_IndexOperations << " and Parallel: " << sumP_IndexOperations << "\n";
	std::cout << std::endl;


	// Normal execution - O(1)
	start = high_resolution_clock::now();
	for(int i=0; i<MAX; i++)
		randNums[i] = randA[i] + randB[i]*randC[i];

	end = high_resolution_clock::now();
	time_span_N = duration_cast<duration<double>> (end - start);

	// Parallel execution - O(1)
	start = high_resolution_clock::now();
	#pragma omp parallel for
	for(int i=0; i<MAX; i++)
		randNums[i] = randA[i] + randB[i]*randC[i];
	
	end = high_resolution_clock::now();
	time_span_P = duration_cast<duration<double>> (end - start);
	
	std::cout << "Cost of each iteration O(1)\n";
	std::cout << "Normal : " << time_span_N.count() <<" ms\n";
	std::cout << "Parallel : " << time_span_P.count() << " ms\n";
	std::cout << "Parallel execution was approximately " << ceil(time_span_N.count()/time_span_P.count()) << " times faster\n";
	std::cout << std::endl;

	/* -------  Including a component with higher computational complexity ----------- */ 

	// Non-parallel execution - O(n*10!)
	start = high_resolution_clock::now();
	for(int i=0; i<MAX; i++)
		randNums[i] = randA[i] + randB[i]*factorial(randC[i]);
	
	end = high_resolution_clock::now();
	time_span_N = duration_cast<duration<double>> (end - start);
	
	// Parallel execution - O(n*10!)
	start = high_resolution_clock::now();
	#pragma omp parallel for
	for(int i=0; i<MAX; i++)
		randNums[i] = randA[i] + randB[i]*factorial(randC[i]);
	
	end = high_resolution_clock::now();
	time_span_P = duration_cast<duration<double>> (end - start);
	
	std::cout << "Cost of each iteration O(n!)\n";
	std::cout << "Normal : " << time_span_N.count() <<" ms\n";
	std::cout << "Parallel : " << time_span_P.count() << " ms\n";
	std::cout << "Parallel execution was approximately " << ceil(time_span_N.count()/time_span_P.count()) << " times faster\n";
	
}
