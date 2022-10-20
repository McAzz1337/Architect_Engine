#pragma once

#include "ray.h"

#include <glm/vec3.hpp>


namespace archt {



	glm::vec3 getFaceNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, bool clockWise);

	bool rayIntersectFace(const Ray& ray, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);


}