#pragma once
#include <glm/vec2.hpp>


namespace archt {

	class SOE2 {

		glm::vec2 a1;
		glm::vec2 a2;

		glm::vec2 b1;
		glm::vec2 b2;


	public:
		SOE2();
		SOE2(const glm::vec2& a1, const glm::vec2& b1, const glm::vec2& a2, const glm::vec2& b2);
		~SOE2();

		bool solve(bool print);

	};



}