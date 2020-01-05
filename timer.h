/* A simple class created to encapsulate a repetitive process of creating high_resolution_clock everytime you need to profile something in c++
   Feel free to use/modify it as you wish. If you have any suggestion or found a bug, feel free to create a branch and request a merge (:
   For now it is a basic class but I plan on expanding it later.
*/

#include <chrono>
#include <iostream>
#include <iomanip>

class Timer {

public:

	Timer(const char *_name) : name(_name) {

		// Getting the start time
		timer_start = std::chrono::high_resolution_clock::now();
	}

	~Timer() {
	
		// Getting the ending time
		auto timer_end = std::chrono::high_resolution_clock::now();
		// Calculating the time elapsed since the creation of the timer	
		std::chrono::duration<double> elapsed_time = timer_end - timer_start;
		
		// Safe output - guaranteed by flush
		std::cout << name << " execution time " << std::setprecision(4) << elapsed_time.count() * 1000 << "ms\n" << std::flush;
	}

private:

	const char *name;
	std::chrono::time_point<std::chrono::high_resolution_clock> timer_start;	

};
