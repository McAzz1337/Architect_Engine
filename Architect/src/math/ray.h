#pragma once

#include <glm/vec3.hpp>


namespace archt {

	struct Ray {

		float length = 0.0f;
		glm::vec3 origin = { 0.0f };
		glm::vec3 direction = { 0.0f };

	public:
		Ray();
		Ray(const glm::vec3& origin, const glm::vec3& direction, float length);
		~Ray();

	};


}