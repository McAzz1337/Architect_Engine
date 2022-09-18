#include "timer.h"
#include <iostream>

namespace archt {

	Timer::Timer() {
		start = std::clock();
	}

	Timer::~Timer() {
		end = std::clock();
		printf("function took %i milliseconds\n", end - start);
	}

}