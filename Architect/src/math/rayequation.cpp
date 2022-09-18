#include "rayequation.h"
#include <glm/glm.hpp>
namespace archt {


	glm::vec2 solveRayEquation(const glm::vec2& a1, const glm::vec2& b1, const glm::vec2& a2, const glm::vec2& b2) {

		glm::vec2 r = a1;
		glm::vec2 v = b1 - a1;

		glm::vec2 s = a2;
		glm::vec2 w = b2 - a2;

		float t = 0.0f;
		float u = 0.0f;

		u = ((v.x * s.y - v.x * r.y) + (v.y * r.x - v.y * s.x)) / (w.x * v.y - w.y * v.x);
		t = (s.x - r.x + u * w.x) / v.x;

		return { t, u };
	}

	bool rayIntersect(const glm::vec2& a1, const glm::vec2& b1, const glm::vec2& a2, const glm::vec2& b2) {
		glm::vec2 result = solveRayEquation(a1, b1, a2, b2);

		return result.x >= 0.0f && result.x <= 1.0f && result.y >= 0.0f && result.y <= 1.0f;
	}

	float getAngle(const glm::vec3& point, const glm::vec3& a, const glm::vec3& b) {

		glm::vec3 dir = a - point;
		glm::vec3 dir1 = b - point;

		return glm::dot(dir, dir1) / (dir.length() * dir.length());
	}


}