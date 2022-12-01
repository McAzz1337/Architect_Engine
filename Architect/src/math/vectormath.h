#pragma once

#include "ray.h"

#include <glm/vec3.hpp>

#include "../gfx/opengl/buffers.h"

namespace archt {



	glm::vec3 getFaceNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, bool clockWise);

	bool rayIntersectFace(const Ray& ray, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);

	bool rayIntersectGeometry(const Ray& ray, VBO* vbo, IBO* ibo);


}