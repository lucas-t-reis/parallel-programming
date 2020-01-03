/*
The reduction clause can come quite in handy as you can see it saves the trouble of having to declare private variables, a parallel region
and an atomic operation. Other than that, there is not much to say about it, at least for now...
*/
#include <cstdio>
#include <iostream>
#include <omp.h>

#define MAX 80

int main() {

	int a[MAX];
	int errorCheck = MAX*(MAX+1)/2; // Sum of n-terms from 1 ~ 100
	int sum = 0;

	for(int i=0; i<MAX; i++)
		a[i] = i+1;

	#pragma omp parallel for
	for(int i=0; i<MAX; i++)
		sum += a[i];

	std::cout << "No reduction clause \n";
	std::cout << "expected " << errorCheck << " found " << sum << std::endl;
		
	int tId;
	int threadSum = 0;
	sum = 0;
	#pragma omp parallel private(tId) firstprivate(threadSum)
	{
		
		#pragma omp for ordered
		for(int i=0; i<MAX; i++)
			threadSum += a[i];

		#pragma omp atomic 
		sum += threadSum;
	}
	
	std::cout << "\nUsing private variables\n";
	std::cout << "expected " << errorCheck << " found " << sum << std::endl;

	sum = 0;
	#pragma omp parallel for reduction(+:sum)
	for(int i=0; i<MAX; i++)
		sum += a[i];
	
	std::cout << "\nUsing reduction\n";
	std::cout << "expected " << errorCheck << " found " << sum << std::endl;

}

