#pragma once
#include <chrono>

namespace archt {

	class DeltaTime {

		std::chrono::system_clock::time_point last;
		std::chrono::system_clock::time_point now;
		int64_t delta;

	public:
		static DeltaTime instance;

		
			
		DeltaTime();
		~DeltaTime();

		void update();

		int64_t getNanos() const;
		double getMicros() const;
		double getMillis() const;
		double getSeconds() const;

		static std::chrono::system_clock::time_point getTime() {
			return std::chrono::system_clock().now();
		}

		template<typename T>
		static double getDelta(std::chrono::system_clock::time_point now, 
							   std::chrono::system_clock::time_point last) {

			double delta = (double) std::chrono::duration_cast<std::chrono::nanoseconds>(now - last).count();

			double mul = 1.0;
			
			
			if (typeid(T) == typeid(std::chrono::microseconds)) {
				mul = 0.001;
			}
			else if (typeid(T) == typeid(std::chrono::milliseconds)) {
				mul = 0.000001;
			}
			else if (typeid(T) == typeid(std::chrono::seconds)) {
				mul = 0.000000001;
			}
	
			return delta * mul;
		}
		
	};


}