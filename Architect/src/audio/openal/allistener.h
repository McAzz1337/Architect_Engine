#pragma once

#include <glm/vec3.hpp>
#include <typeindex>

namespace archt {

	class ALListener {

		uint32_t id = 0;
		float pos[3] = { 0.0f, 0.0f, 0.0f };
		float velocity[3] = { 0.0f, 0.0f, 0.0f };
		float orientation[6] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

	public:
		ALListener();
		~ALListener();


		void setPosition(const glm::vec3& pos);
		void setVelocity(const glm::vec3& vel);

	};


}