#include "ray.h"

#include <math.h>

namespace archt{




	Ray::Ray() {
	
	}

	Ray::Ray(const glm::vec3& origin, const glm::vec3& direction, float length) 
		: origin(origin), length(length) {
	
		float l = powf(direction.x, direction.x) + powf(direction.y, direction.y) + powf(direction.z, direction.z);
		this->direction = { direction.x / l, direction.y / l, direction.z / l };
	}

	Ray::~Ray() {
	
	}




}