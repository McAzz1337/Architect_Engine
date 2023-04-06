#include "fm.h"

namespace archt {
	FM::FM() {

	}

	FM::~FM() {

	}

	void FM::clearUnused(const std::vector<void*>& retained) {
		for (auto iter = shaders.begin(); iter != shaders.end(); iter++) {
			// TODO figure out how to delete unused elements
		}
	}
}