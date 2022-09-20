#pragma once

#include "curve.h"

#include <glm/vec3.hpp>

#include <vector>

namespace archt {

	class BezierreCurve {
	
		float duration = 5.0f;
		glm::vec3 start;
		glm::vec3 end;

		std::vector<glm::vec3> anchors;

	
	public:
	
		BezierreCurve();
		BezierreCurve(const glm::vec3& start);
		BezierreCurve(const glm::vec3& start, const glm::vec3& end);
		~BezierreCurve();

	
		void addAnchor(const glm::vec3& anchor);
		void removeAnchor(glm::vec3* anchor);

		glm::vec3 calculateCurve(float deltaTime) const;
		Curve getCurve() const;


	private:
		glm::vec3 interpolateLinearly(float t, const glm::vec3** nodes, int size) const;
	
	};


}