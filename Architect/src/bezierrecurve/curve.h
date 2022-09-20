#pragma once


#include <glm/vec3.hpp>


namespace archt {

	class Curve {


		float duration = 5.0f;
		int size = 0;
		glm::vec3* nodes = nullptr;

	public:
		Curve();
		Curve(glm::vec3* nodes, int size);
		~Curve();

		glm::vec3 getPosition(float deltaTime) const;
	

	};


}