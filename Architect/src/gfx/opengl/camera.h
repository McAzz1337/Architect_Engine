#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>

namespace archt {


	class Camera {

		glm::mat4 projection;
		glm::mat4 view;

	public:
		Camera();
		Camera(float fov, float aspect, float near, float far);
		~Camera();

		void translate(const glm::vec3& t);
		void rotate(const glm::vec3& axis, float angle);

		void resetMatrix();

		inline const glm::mat4& getProjection() const { return projection; }
		inline const glm::mat4& getView() const { return glm::inverse(view); }
		inline const glm::mat4 getProjectionView() const { return projection * glm::inverse(view); }


	};



}