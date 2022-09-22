#include "bezierrecurve.h"

namespace archt {
	BezierreCurve::BezierreCurve() {}

	BezierreCurve::BezierreCurve(const glm::vec3& start) : start(start) {}

	BezierreCurve::BezierreCurve(const glm::vec3& start, const glm::vec3& end)
		: start(start), end(end) {}

	BezierreCurve::~BezierreCurve() {}

	void BezierreCurve::addAnchor(const glm::vec3& anchor) {

		anchors.push_back(anchor);
	}

	void BezierreCurve::removeAnchor(glm::vec3* anchor) {

		for (int i = 0; i < anchors.size(); i++) {

			if (&anchors[i] == anchor) {

				anchors.erase(anchors.begin() + i);
				anchors.shrink_to_fit();
				break;
			}
		}
	}

	glm::vec3 BezierreCurve::calculateCurve(float deltaTime) const {


		float t = deltaTime / duration;

		int size = anchors.size() + 2;
		const glm::vec3** nodes = new const glm::vec3*[size];
		nodes[0] = &start;
		nodes[size - 1] = &end;

		for (int i = 0; i < anchors.size(); i++) {

			nodes[i + 1] = &anchors[i];
		}

		glm::vec3 pos = interpolateLinearly(t, nodes, size);

		delete[] nodes;

		return pos;
	}

	glm::vec3 BezierreCurve::interpolateLinearly(float t, const glm::vec3** _nodes, int _size) const {

		if (_size == 2)
			return *_nodes[0] + (*_nodes[1] - *_nodes[0]) * t;

		int size = _size - 1;
		const glm::vec3** nodes = new const glm::vec3*[size];

		for (int i = 0; i < size; i++) {
			
			const glm::vec3& a = *_nodes[i];
			const glm::vec3& b = *_nodes[i + 1];

			nodes[i] = new glm::vec3(a + (b - a) * t);
		}

		glm::vec3 pos = interpolateLinearly(t, nodes, size);

		for (int i = 0; i < size; i++) 
			delete nodes[i];
		
		delete[] nodes;


		return pos;
	}

	Curve BezierreCurve::getCurve() const {

		float step = 0.05f;
		
		int size = (int) (duration / step);
		glm::vec3* nodes = new glm::vec3[size];



		for (int i = 0; i < size; i++) {
			float t = i * step;
			nodes[i] = calculateCurve(t);
		}

		return Curve(nodes, size);
	}
}