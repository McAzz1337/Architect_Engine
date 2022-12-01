#pragma once

#include <chrono>

namespace archt {

	namespace delta {

		typedef std::chrono::steady_clock::time_point Time;


		typedef std::nano nanos;
		typedef std::micro micros;
		typedef std::milli millis;
		typedef std::ratio<1, 1> seconds;


		Time getTimePoint();

		//template<typename T>
		//float getDelta(archt_time time) {
		//
		//	archt_time now = std::chrono::system_clock::now();
		//	uint64_t delta = (float) std::chrono::duration_cast<std::chrono::nanoseconds>(now - time).count();
		//
		//	if (typeid(T) == typeid(std::chrono::nanoseconds) {
		//		return (float) delta;
		//	}
		//	else if (typeid(T) == typeid(std::chrono::microseconds) {
		//		return (float) delta * 0.001;
		//	}
		//	else if (typeid(T) == typeid(std::chrono::milliseconds) {
		//		return (float) delta * 0.000001;
		//	}
		//	else { // seconds
		//		return (float) delta * 0.000000001;
		//	}
		//}

		template<typename T>
		float getDelta(Time& time, bool outputNewTime = true) {

			Time now = std::chrono::high_resolution_clock().now();

			float delta =  std::chrono::duration<float, T>(now - time).count();

			if (outputNewTime)
				time = now;

			return delta;
		}


	

	}


}