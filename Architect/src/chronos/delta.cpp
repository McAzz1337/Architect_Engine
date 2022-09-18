#include "delta.h"

namespace archt {

	namespace delta {
		archt_time getTimePoint() {
			return std::chrono::system_clock::now();
		}

	}


}