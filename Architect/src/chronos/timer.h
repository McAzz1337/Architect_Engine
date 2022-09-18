#pragma once

#include <ctime>

namespace archt {

	class Timer {

		std::clock_t start, end;

	public:
		Timer();
		~Timer();

	};


}