#include "glmesh.h"

#include "../../filesystem/filemanager.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "../../math/rayequation.h"

#include "../gui/gui.h"

extern bool debug;

namespace archt {

	int GLMesh::count = 0;


	GLMesh::GLMesh() {
		id = count;
		count++;
		
		
		Gui::instance->addGuiWindow([this]() {
			std::string windowName = "Pokemon " + std::to_string(id); 
			ImGui::Begin(windowName.c_str());
			glm::mat4 m = this->getModelMatrix();
			ImGui::Text("Matrix");
			ImGui::Text("%f\t%f\t%f\t%f", m[0][0], m[0][1], m[0][2], m[0][3]);
			ImGui::Text("%f\t%f\t%f\t%f", m[1][0], m[1][1], m[1][2], m[1][3]);
			ImGui::Text("%f\t%f\t%f\t%f", m[2][0], m[2][1], m[2][2], m[2][3]);
			ImGui::Text("%f\t%f\t%f\t%f", m[3][0], m[3][1], m[3][2], m[3][3]);

			//int size = 0;
			//glm::vec2* pos = getPositionsTranslated(cam, size);
			//
			//ImGui::Text("Positions:");
			//for (int i = 0; i < size; i++) {
			//	ImGui::Text("Vertex[%i]:\t%f\t%f", i, pos->x, pos->y);
			//}

			if (this->colided)
				ImGui::Text("Collided with id = %i", coliderId);

			ImGui::Text("----------------------------");

			ImGui::End();
		});
	
	}

	GLMesh::GLMesh(VBO* vbo, IBO* ibo) : vbo(vbo), ibo(ibo) {}

	GLMesh::GLMesh(VBO* vbo, IBO* ibo, const std::string& texture) : vbo(vbo), ibo(ibo) {

	}

	GLMesh::GLMesh(Vertex* verteces, uint32_t vSize, uint32_t* indeces, uint32_t iSize) {
		vbo = new VBO(verteces, vSize);
		ibo = new IBO(indeces, iSize);
	}


	GLMesh::~GLMesh() {
		if (vbo) delete vbo;
		if (ibo) delete ibo;
	}

	void GLMesh::translate(const glm::vec3& t) {
		modelMatrix = glm::translate(modelMatrix, t);
	}

	void GLMesh::rotate(float angle, const glm::vec3& axis) {
		modelMatrix = glm::rotate(modelMatrix, angle, axis);

	}

	void GLMesh::scale(const glm::vec3& s) {
		modelMatrix = glm::scale(modelMatrix, s);
	}

	void GLMesh::resetMatrix() {
		modelMatrix = glm::mat4(1.0f);
	}


	void GLMesh::setVbo(VBO* vbo) {
		if (this->vbo)
			delete this->vbo;

		this->vbo = vbo;
	}

	void GLMesh::setVbo(Vertex* verteces, uint32_t size) {
		if (vbo)
			delete vbo;

		vbo = new VBO(verteces, size);
	}

	void GLMesh::setIbo(IBO* ibo) {
		if (this->ibo)
			delete this->ibo;

		this->ibo = ibo;
	}

	void GLMesh::setIbo(uint32_t* indeces, uint32_t size) {
		if (ibo)
			delete ibo;

		ibo = new IBO(indeces, size);
	}

	void GLMesh::setTexture(GLTexture* tex) {
		if (this->tex)
			delete this->tex;

		this->tex = tex;
	}

	void GLMesh::setShader(GLShader* shader) {
		this->shader = shader;
	}

	void GLMesh::setModelMatrix(const glm::mat4& m) {
		modelMatrix = m;
	}

	bool GLMesh::checkCollision(const GLMesh& other, const Camera& cam) const {
		
		int oSize = 0;
		glm::vec2* oPos = other.getPositionsTranslated(cam, oSize);

		int size = 0;
		glm::vec2* pos = getPositionsTranslated(cam, size);

		for (int i = 0; i < size; i++) {

			int ii = i == size - 1 ? 0 : i + 1;
			for (int j = 0; j < oSize; j++) {

				if (rayIntersect(	pos[i],
									pos[ii],
									oPos[j],
									oPos[(j == oSize - 1) ? 0 : j + 1])) {
					colided = true;
					coliderId = other.id; 
					return true;
				}
			}
		}


		delete[] oPos;
		delete[] pos;
		
		colided = false;
		coliderId = -1;

		return false;
	}

	glm::vec2* GLMesh::getPositionsTranslated(const Camera& cam, int& out_size) const {

		out_size = vbo->getSize();
		Vertex* verteces = vbo->getData();
		glm::vec2* positions = new glm::vec2[out_size];

		glm::vec4 pos = modelMatrix[3];
		glm::mat4 vp = cam.getProjectionView();


		for (int i = 0; i < out_size; i++) {
			glm::vec4 t = vp * modelMatrix * glm::vec4(verteces[i].pos, 1.0f);
			positions[i] = glm::vec2(t.x, t.y);
		}

		return positions;
	}

	void GLMesh::printMatrix() const {
		printf("Matrix:\n");
		for (int i = 0; i < 4; i++) {
			printf("%f\t%f\t%f\t%f\n", modelMatrix[i][0], modelMatrix[i][1], modelMatrix[i][2], modelMatrix[i][3]);
		}
		printf("----------------------\n");
	}

}