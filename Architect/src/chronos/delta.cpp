#include "delta.h"

namespace archt {

	namespace delta {
		
		Time getTimePoint() {
			
			return std::chrono::high_resolution_clock().now();
		}

	}


}