#include "mesh.h"

#include "../../ptr.h"

#include "../../gfx/gui/gui.h"
#include "../../math/rayequation.h"

namespace archt {

	Mesh::Mesh() {
		//addComponent(make_ptr<Transform>());
		modelMatrix = Transform();
	}

	Mesh::~Mesh() {}


	void Mesh::translate(const glm::vec3& t) {
		position += t;
		//modelMatrix.translate(t);
	}

	void Mesh::rotate(float angle, const glm::vec3& axis) {
		rotation = angle * axis;
		//modelMatrix.rotate(angle, axis);

	}

	void Mesh::scale(const glm::vec3& s) {
		modelMatrix.scale(s);
	}

	void Mesh::resetMatrix() {
		modelMatrix.reset();
	}

	bool Mesh::checkCollision(ptr<Mesh> other, ptr<Camera_new> cam) const {
		int oSize = 0;
		glm::vec2* oPos = other->getPositionsTranslated(cam, oSize);

		int size = 0;
		glm::vec2* pos = getPositionsTranslated(cam, size);

		for (int i = 0; i < size; i++) {

			int ii = i == size - 1 ? 0 : i + 1;
			for (int j = 0; j < oSize; j++) {

				if (rayIntersect(pos[i],
					pos[ii],
					oPos[j],
					oPos[(j == oSize - 1) ? 0 : j + 1])) {
					
					return true;
				}
			}
		}


		delete[] oPos;
		delete[] pos;

		

		return false;
	}

	glm::vec2* Mesh::getPositionsTranslated(ptr<Camera_new> cam, int& out_size) const {
		out_size = vbo->getSize();
		Vertex* verteces = vbo->getData();
		glm::vec2* positions = new glm::vec2[out_size];

		Transform vp = cam->getProjectionView();


		for (int i = 0; i < out_size; i++) {
			glm::vec4 t = vp * modelMatrix * verteces[i].pos;
			positions[i] = glm::vec2(t.x, t.y);
		}

		return positions;
	}

	void Mesh::createGuiWindow(ptr<Camera_new> cam) {
		
		auto lambda = [this, &cam] {
			glm::mat4 m = (cam->getProjectionView() * modelMatrix).getMatrix();

			ImGui::Begin("Mesh");
			ImGui::Text("Matrix:");

			for (int i = 0; i < 4; i++) {
				ImGui::Text("%f\t%f\t%f\t%f", m[i][0], m[i][1], m[i][2], m[i][3]);
			}

			ImGui::End();
		};
		//Gui::instance->addGuiWindow(lambda);
	}

	void Mesh::setVBO(Vertex* verteces, uint32_t size) {
		if (!vbo)
			vbo = new VBO();
		vbo->setVerteces(verteces, size);
	}

	void Mesh::setIBO(uint32_t* indeces, uint32_t size) {
		if (!ibo)
			ibo = new IBO();
		ibo->setIndeces(indeces, size);
	}

	void Mesh::setTransform(const Transform& t) {
		modelMatrix = t;
	}

}