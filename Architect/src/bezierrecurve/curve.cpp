#include "curve.h"

#include <math.h>
#include <iostream>

namespace archt {

	Curve::Curve() {

	}

	Curve::Curve(glm::vec3* nodes, int size) : nodes(nodes), size(size) {

	}

	Curve::~Curve() {
		delete[] nodes;
	}

	glm::vec3 Curve::getPosition(float deltaTime) const {
		
		float t = deltaTime / duration;
	
		if (t > 1.0f && fmodf(t, 1.0f) > 0.0f) {
			t = fmodf(t, 1.0f);
		}


		int index = (int) (size * t);
		
		return nodes[index] + (nodes[index + 1] - nodes[index]) * t;
	}

	

}