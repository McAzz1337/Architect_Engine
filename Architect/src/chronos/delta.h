#pragma once

#include <chrono>

namespace archt {

	namespace delta {

		typedef std::chrono::system_clock::time_point archt_time;

		typedef std::chrono::nanoseconds nanos;
		typedef std::chrono::microseconds micros;
		typedef std::chrono::milliseconds millis;
		typedef std::chrono::seconds seconds;



		archt_time getTimePoint();

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
		float getDelta(archt_time& time, bool outputNewTime = true) {

			archt_time now = std::chrono::system_clock::now();
			float delta = (float) std::chrono::duration_cast<nanos>(now - time).count();
			
			if (outputNewTime) {
				time = now;
			}

			if (typeid(T) == typeid(nanos)) {
				return delta;
			}
			else if (typeid(T) == typeid(micros)) {
				return (float) delta * 0.001f;
			}
			else if (typeid(T) == typeid(millis)) {
				return (float) delta * 0.000001f;
			}
			else { // seconds
				return (float) delta * 0.000000001f;
			}
		}


	

	}


}