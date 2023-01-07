/* If part of the code needs to be executed in a sequential order, the ordered clause can help. Beware not to convert your parallel
code into a serial one... Here, the load of doing @work is shared but "presenting" the data is sequential. 
Much has yet to be explored - using schedule() do determine chunk sizes - but for now, knowing it exist is enough for this example.
The reason that ordered time will be slower than parallel non-ordered one is related to the stall time of each thread, that has to wait
for its predecessor to do the ordered part of the code. I couldn't yet find a reasonable use for this clause. Theoretically, it should 
yield better results for computational heavy steps. Maybe in future trials I should think of a something "heavier" than what has been done
here (bad cache and factorial).
*/

#include <cstdio>
#include <omp.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <vector>
#include <cmath>

#define MAX 10000

int work(int id, int i) {

	if(i <= 0) return 1;
	else return i*work(id,i-1) + work(id, i-2)*2;
	//printf("Thread %d is working at iteration %d...\n", id, i);
	
}

int main() {
	

	int tid;
	srand(time(NULL));

	using namespace std::chrono;
	high_resolution_clock::time_point start, end;
	duration<double> time_N; // Non-Parallel
	duration<double> time_P;  // Parallel
	duration<double> time_O; // ordered and not scheduled

	std::vector<int> ans;
	std::vector<int> a;
	std::vector<int> b;
	std::vector<int> c;


	std::cout << "Filling vectors...\n";
	for(int i=0; i<MAX*MAX; i++) {
		
		a.push_back(rand()%15);
		b.push_back(rand()%20);
		c.push_back(rand()%30);

	}

	std::cout << "Starting tests\n";

	start = high_resolution_clock::now();
	for(int i=0; i<MAX; i++) {

		tid = 0;

		int x = a[i]*work(tid, i%15) * b[i]*work(tid, i%10) * c[i]*work(tid, (i+i)%10);
		double y = 0;	
		double y2 = 0;
		double y3 = 0;

		for(int j=0; j<100; j++){
			y += ( (int) sqrt(pow(x, j))*c[i]/(b[j]+1) )%15;
			for(int k=0; k<100; k++)
				y2 += ( c[i]/(b[j]+1) +c[j]+c[i]+c[k] + a[k] + b[j]+b[i]+b[k])%15;
			y3 += work(tid, int(a[j]%5+c[i]%5) );
		}
	
		ans.push_back(y3);
		//printf("Done tid:%d i:%d\n", tid, i);	

	}
	ans.clear();
	end = high_resolution_clock::now();
	time_N = duration_cast<duration<double>> (end - start);
	std::cout << "Non-parallel time: " << time_N.count() << " ms\n";	

	// Note that the critical clause DOES NOT guarantee order. It was only used to have a notion of time elapsed
	// to do one block of instructions at a time.
	start = high_resolution_clock::now();
	#pragma omp parallel for private(tid)
	for(int i=0; i<MAX; i++) {

		tid = omp_get_thread_num();

		int x = a[i]*work(tid, i%15) * b[i]*work(tid, i%10) * c[i]*work(tid, (i+i)%10);
		double y = 0;	
		double y2 = 0;
		double y3 = 0;

		for(int j=0; j<100; j++){
			y += ( (int) sqrt(pow(x, j))*c[i]/(b[j]+1) )%15;
			for(int k=0; k<100; k++)
				y2 += ( c[i]/(b[j]+1) +c[j]+c[i]+c[k] + a[k] + b[j]+b[i]+b[k])%15;
			y3 += work(tid, int(a[j]%5+c[i]%5) );
		}
		
		#pragma omp critical
		{
		ans.push_back(y3);
		//printf("Done tid:%d i:%d\n", tid, i);	
		}

	}

	ans.clear();
	end = high_resolution_clock::now();
	time_P = duration_cast<duration<double>> (end - start);
	std::cout << "Parallel time: " << time_P.count() << " ms\n";	


	start = high_resolution_clock::now();
	#pragma omp parallel for ordered private(tid)
	for(int i=0; i<MAX; i++) {

		tid = omp_get_thread_num();
		
		int x = a[i]*work(tid, i%15) * b[i]*work(tid, i%10) * c[i]*work(tid, (i+i)%10);
		double y = 0;	
		double y2 = 0;
		double y3 = 0;

		for(int j=0; j<100; j++){
			y += ( (int) sqrt(pow(x, j))*c[i]/(b[j]+1) )%15;
			for(int k=0; k<100; k++)
				y2 += ( c[i]/(b[j]+1) +c[j]+c[i]+c[k] + a[k] + b[j]+b[i]+b[k])%15;
			y3 += work(tid, int(a[j]%5+c[i]%5) );
		}
		
		#pragma omp ordered
		{
		ans.push_back(y3);
		//printf("Done tid:%d i:%d\n", tid, i);	
		}
	}
	ans.clear();
	end = high_resolution_clock::now();
	time_O = duration_cast<duration<double>> (end - start);
	std::cout << "Parallel ordered time: " << time_O.count() << " ms\n";	
	
}
