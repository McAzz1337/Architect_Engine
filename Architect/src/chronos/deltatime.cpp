#include "deltatime.h"


namespace archt {

	DeltaTime DeltaTime ::instance;


	DeltaTime::DeltaTime() {
		last = std::chrono::system_clock().now();
	}

	DeltaTime::~DeltaTime() {}

	void DeltaTime::update() {
		now = std::chrono::system_clock().now();
		delta = std::chrono::duration_cast<std::chrono::nanoseconds>(now - last).count();
		last = now;
	}

	int64_t DeltaTime::getNanos() const { return delta; }
	double DeltaTime::getMicros() const { return (double) delta / 1000.0; }
	double DeltaTime::getMillis() const { return (double) delta / 1000000.0; }
	double DeltaTime::getSeconds() const { return (double) delta / 1000000000.0; }
}