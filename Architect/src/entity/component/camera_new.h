#pragma once

#if 1 
#include "../component.h"

#include "transform.h"

#include "../entity.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>

namespace archt {

	class Camera_new : public Entity {

		Transform projection;
		Transform view;

		

	public:
		Camera_new();
		Camera_new(float fov, float aspect, float near, float far);
		~Camera_new();



		void translate(const glm::vec3& t);
		void rotate(float angle, const glm::vec3& axis);

		void resetMatrix();


		inline glm::vec3 getPosition() const { return glm::vec3(view.getMatrix()[3]); };

		inline const Transform& getProjection() const { return projection; }
		inline const Transform& getView() const { return view; }
		inline const Transform getProjectionView() const { return projection * view; }


		


	};
}
#endif