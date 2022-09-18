#include "camera.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>



namespace archt {
	
	
	Camera::Camera() {

	}

	Camera::Camera(float fov, float aspect, float near, float far) {
		projection = glm::perspective(fov, aspect, near, far);
		view = glm::mat4(1.0f);

		translate({ 0.0f, 0.0f, 1.0f });
	}

	Camera::~Camera() {

	}

	void Camera::translate(const glm::vec3& t) {
		view = glm::translate(view, t);
	}

	void Camera::rotate(const glm::vec3& axis, float angle) {
		view = glm::rotate(view, angle, axis);
	}

	void Camera::resetMatrix() {
		view = glm::mat4(1.0f);
		translate({ 0.0f, 0.0f, 1.0f });
	}

}