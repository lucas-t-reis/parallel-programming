/* 

Code taken from https://stackoverflow.com/q/42970700/9164894 as an example to understand profiling with gprof and schedules

 -- specs from the computer running this code can be found in the README of this directory --
 	
	* compile with g++ -g -pg -o program_name program.cpp -fopenmp
	* run program_name once, it will spawn a gmon.out file
	* in shell, type 
		$ gprof program_name (no extensions, you can use the flag -b for no verbose output) 
	* 

schedule(guided,1)
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  ms/call  ms/call  name    
101.38      8.99     8.99       31   290.08   290.08  work(long)

static ( iterations/threads )
Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  ms/call  ms/call  name    
 95.14      8.52     8.52       27   315.38   315.38  work(long)

schedule(dynamic)
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  ms/call  ms/call  name    
101.38      7.49     7.49       27   277.49   277.49  work(long)


What can be seen from the flat profile is that dynamic scheduling yields better results when compared to static  or guided.
For this problem, as the iterations inscrease it gets easier to compute @work(iteration), emulating a dynamic computational task. 
In this type of scenario if you assign statically, the last thread will stay idle for a long time since it finishes first. 
As for the guided scheduling, it starts assigning big chunks but as threads begin to do "uneven" work it tries to balance it cutting the
chunk size in hopes that threads will "align" and do a somewhat similar amount of work in the end. Experimenting with the chunk size is a valid 
way to try and improve the guided perfomance. Fewer calls were needed in guided and dynamic. Why? No clue yet..

Yeah... I know.. gprof. I mean...you have to start somewhere right? As soon as I figure out score-p and vampir I shall be using them.

*/

#include <omp.h>
void work(long ww) {
    volatile long sum = 0;
    for (long w = 0; w < ww; w++) sum += w;
}

int main() {
    const long max = 32, factor = 10000000l;
    #pragma omp parallel for schedule(guided, 1) 
    for (int i = 0; i < max; i++) {
		 // As @i increases, work(ww) will get done faster
         work((max - i) * factor);
    }
}
