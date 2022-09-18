#include "transform.h"

#include <glm/gtx/transform.hpp>

namespace archt {
	
	
	Transform::Transform() : matrix(glm::mat4(1.0f)) {}
	
	Transform::Transform(const Transform& other) : matrix(other.matrix) {

	}

	
	Transform::Transform(float f) : matrix(glm::mat4(f)) {}
	
	Transform::Transform(const glm::mat4& matrix) : matrix(matrix) {}
	
	Transform::~Transform() {}
	
	void Transform::translate(const glm::vec3& t) {
		matrix = glm::translate(matrix, t);
	}
	
	void Transform::rotate(float angle, const glm::vec3& axis) {
		matrix = glm::rotate(matrix, angle, axis);
	}
	
	void Transform::scale(const glm::vec3& s) {
		matrix = glm::scale(matrix, s);
	}
	
	void Transform::reset() {
		matrix = glm::mat4(1.0f);
	}

	Transform Transform::inverse() const {
		return Transform(glm::inverse(matrix));
	}

	
	Transform Transform::operator*(const Transform other) const {
		return Transform(matrix * other.matrix);
	}
	
	void Transform::operator*=(const Transform other) {
		matrix *= other.matrix;
	}
	
	void Transform::operator<<(const Transform other) {
		matrix = glm::inverse(other.matrix);
	}

	glm::vec4 Transform::operator*(const glm::vec3& v) const {
		return matrix * glm::vec4(v, 1.0f);
	}
	
	void Transform::operator*=(const glm::vec3& v) {
		translate(v);
	}

}