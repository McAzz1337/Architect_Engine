#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


namespace archt {


	glm::vec2 solveRayEquation(const glm::vec2& a1, const glm::vec2& b1, const glm::vec2& a2, const glm::vec2& b2);
	bool rayIntersect(const glm::vec2& a1, const glm::vec2& b1, const glm::vec2& a2, const glm::vec2& b2);

	float getAngle(const glm::vec3& point, const glm::vec3& a, const glm::vec3& b);
}