#pragma once

#include "../component.h"

#include "camera_new.h"

#include "transform.h"

#include "../../gfx/opengl/buffers.h"

#include <glm/vec2.hpp>

namespace archt {

	class Mesh : public Component {

		VBO* vbo = nullptr;
		IBO* ibo = nullptr;
		mutable Transform modelMatrix;

		glm::vec3 rotation = glm::vec3(0.0f);
		glm::vec3 position = glm::vec3(0.0f);

	public:
		Mesh();
		~Mesh();

		void translate(const glm::vec3& t);
		void rotate(float angle, const glm::vec3& axis);
		void scale(const glm::vec3& s);

		void resetMatrix();

		bool checkCollision(ptr<Mesh> other, ptr<Camera_new> cam) const;

		glm::vec2* getPositionsTranslated(ptr<Camera_new> cam, int& out_size) const;


		void createGuiWindow(ptr<Camera_new> cam);

		void setVBO(Vertex* verteces, uint32_t size);
		void setIBO(uint32_t* indeces, uint32_t size);
		void setTransform(const Transform& t);


		inline VBO* getVBO() { return vbo; }
		inline IBO* getIBO() { return ibo; }

		inline  Transform& getTransform() const {
			modelMatrix = Transform();
			modelMatrix.rotate(rotation.x, { 1.0f, 0.0f, 0.0f });
			modelMatrix.rotate(rotation.y, { 0.0f, 1.0f, 0.0f });
			modelMatrix.rotate(rotation.z, { 0.0f, 0.0f, 1.0f });

			modelMatrix.translate(position);
			return modelMatrix;
		}

	};


}