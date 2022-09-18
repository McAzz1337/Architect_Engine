#include "renderer2d.h"

#include "../../ptr.h"

#include "../opengl/glshaderconstants.h"
#include "../opengl/glrenderapi.h"
#include "../opengl/gldebug.h";

#include "../../entity/component/mesh.h"
#include "../../entity/component/material/material.h"


#include "../../fileio.h"


#include "../gui/gui_s.h"

namespace archt {

	Renderer2D* Renderer2D::instance = nullptr;

	const int Renderer2D::MAX_OBJECTS = 10000;
	const int Renderer2D::MAX_VERTECES = 4 * MAX_OBJECTS;
	const int Renderer2D::MAX_INDECES = 6 * MAX_OBJECTS;



	Renderer2D::Renderer2D() {
		entities = new ptr<Entity>[MAX_OBJECTS];


		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

		GLRenderAPI::enable(GL_DEPTH_TEST);
		GLRenderAPI::enable(GL_BLEND);

		GLRenderAPI::blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		GLRenderAPI::setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//GLRenderAPI::setCullFace(GL_CW, GL_BACK);

		Vertex* verteces = new Vertex[MAX_VERTECES];
		vbo = new VBO(verteces, MAX_VERTECES);
		vbo->allocateOnGPU();

		uint32_t* indeces = new uint32_t[MAX_INDECES];
		ibo = new IBO(indeces, MAX_INDECES);
		ibo->allocateOnGPU();

		vao = new GLVertexarray(vbo, ibo);
		//vao->setVertexAttribPointer(0, 3, (const void*) offsetof(Vertex, pos));
		//vao->setVertexAttribPointer(1, 3, (const void*) offsetof(Vertex, normal));
		//vao->setVertexAttribPointer(2, 2, (const void*) offsetof(Vertex, uv));
		//vao->setVertexAttribPointer(3, 1, (const void*) offsetof(Vertex, texId));
		//vao->setVertexAttribPointer(4, 1, (const void*) offsetof(Vertex, matrixId));


		matrices = new Transform[GLRenderAPI::maxMatrices];

		textures = new int[GLRenderAPI::maxTextures];
		for (int i = 0; i < GLRenderAPI::maxTextures; i++) {
			textures[i] = 0;
		}


		auto lambda = [this](bool* open, GuiWindow_s* handle) {
			if (!t || !v || !ibo_ptr) {
				return;
			}
			ImGui::Begin("Render Transform");

			const glm::mat4& m = t->getMatrix();
			ImGui::Text("%f\t%f\t%f\t%f", m[0][0], m[0][1], m[0][2], m[0][3]);
			ImGui::Text("%f\t%f\t%f\t%f", m[1][0], m[1][1], m[1][2], m[1][3]);
			ImGui::Text("%f\t%f\t%f\t%f", m[2][0], m[2][1], m[2][2], m[2][3]);
			ImGui::Text("%f\t%f\t%f\t%f", m[3][0], m[3][1], m[3][2], m[3][3]);
			
			ImGui::Text("--------------------------------");

			for (int i = 0; i < v->getSize(); i++) {
				glm::vec4 vertex = m * glm::vec4(v->getData()[i].pos, 1.0f);
				ImGui::Text("Vertex[%i] : %f\t%f\t%f", i, vertex.x, vertex.y, vertex.z);
			}


			ImGui::End();
		};
		Gui_s::getInstance()->addGuiWindow_void(lambda);
	}

	Renderer2D::~Renderer2D() {
		delete[] entities;
		delete[] matrices;

		delete vao;
	}

	void Renderer2D::beginScene(ptr<Camera_new> camera) {
		inScene = true;
		cam = camera;
	}

	void Renderer2D::endScene() {
		inScene = false;
		currentEntity = 0;
	}

	void Renderer2D::submit(ptr<Entity> entity) {
		if (!inScene)
			return;

		if (currentEntity == MAX_OBJECTS) {
			render();
			endScene();
			beginScene(cam);
		}

		entities[currentEntity] = entity;
		currentEntity++;
	}

	void Renderer2D::clear() {
		glClear(GLRenderAPI::clearMask);
	}

	void Renderer2D::render() {
		if (!inScene || currentEntity == 0)
			return;

		sort();
		beginBatch();
		draw();
		flush();
		endBatch();
	}

	void Renderer2D::flush() {
	
		currentVertex = 0;
		currentIndex = 0;
		currentTexture = 0;
		currentMatrix = 0;
	}

	void Renderer2D::beginBatch() {
		//activeShader = &entities[0]->getComponent<Mesh>()->getComponent<Material>()->getShader();
		const Transform projectionView = cam->getProjectionView();

		for (int i = 0; i < currentEntity; i++) {

			
			ptr<Mesh> mesh = entities[i]->getComponent<Mesh>();
			ptr<Material> material = mesh->getComponent<Material>();

			VBO* vb = mesh->getVBO();
			IBO* ib = mesh->getIBO();

			uint32_t vSize = vb->getSize();
			uint32_t iSize = ib->getSize();



			GLTexture* tex = material->getTexture();
			int texIndex = fetchTextureIndex(tex->getId());

			if (currentVertex + vSize >= MAX_VERTECES ||
				currentIndex + iSize >= MAX_INDECES ||
				(texIndex == -1 && currentTexture == GLRenderAPI::maxTextures) ||
				currentMatrix == GLRenderAPI::maxMatrices ||
				&(mesh->getComponent<Material>()->getShader()) != activeShader) {

				draw();
				flush();
				endBatch();

				activeShader = &(mesh->getComponent<Material>()->getShader());
			}

			if (texIndex != -1) {
				vb->setTexId((float) texIndex);
			}
			else {
				tex->bind(currentTexture);
				vb->setTexId((float) currentTexture);
				textures[currentTexture] = tex->getId();
				currentTexture++;
			}


			vb->setMatrixId((float) currentMatrix);
			matrices[currentMatrix] = projectionView * (*(mesh->getComponent<Transform>().get()));
			t = &matrices[currentMatrix];
			v = vb;
			ibo_ptr = ib;

			vbo->write(currentVertex, vb->getData(), vSize);
			ibo->write(currentIndex, ib->getData(), iSize, currentVertex);

			currentVertex += vSize;
			currentIndex += iSize;
			currentMatrix++;
		}
	}

	void Renderer2D::endBatch() {
		currentTexture = 0;
	}

	uint32_t* Renderer2D::reorderIndeces(uint32_t* src, int size, int& out_size) {
		return nullptr;
	}

	void Renderer2D::sort() {
		int index = 0;
		for (int i = 0; i < currentEntity - 1; i++) {
			GLShader* currentShader = &entities[i]->getComponent<Mesh>()->getComponent<Material>()->getShader();
			GLShader* nextShader = &entities[i + 1]->getComponent<Mesh>()->getComponent<Material>()->getShader();
			if (nextShader != currentShader) {
				index = i + 1;
				for (int j = i + 2; j < currentEntity; j++) {
					nextShader = &entities[j]->getComponent<Mesh>()->getComponent<Material>()->getShader();
					if (nextShader == currentShader) {
						ptr<Entity> entity = entities[j];
						entities[j] = entities[index];
						entities[index] = entity;
						index++;
					}
				}
				i = index;
			}
		}
	}

	void Renderer2D::draw() {
		vbo->bind();
		ibo->bind();

		vbo->upload();
		ibo->upload();
		vao->bind();

		activeShader->bind();
		std::string shaderName = "";
		extractFileName(activeShader->getFilePath(), shaderName);
		const std::vector<Uniformbuffer*>& buffers = activeShader->getUniformBuffers();
		if (buffers.size() > 0) {
			for (int i = 0; i < buffers.size(); i++) {
				buffers[i]->bind();
				if (buffers[i]->getName() == "matrices") {
					buffers[i]->write(0, (void*) matrices, currentMatrix * sizeof(glm::mat4));
					printf("writing %i matrices to uniform buffer\n", currentMatrix);
				}
				buffers[i]->upload();
			}
		}
		else {
			activeShader->setMatrixf4v("mvp", (glm::mat4*) matrices, currentMatrix);
		}
		CALL(glDrawElements(GL_TRIANGLES, currentIndex, GL_UNSIGNED_INT, nullptr));
	}

	void Renderer2D::flushTextures() {
		for (int i = 0; i < GLRenderAPI::getMaxTextureCount(); i++)
			textures[i] = 0;

		currentTexture = 0;
	}

	int Renderer2D::fetchTextureIndex(int tex) {
		for (int i = 0; i < GLRenderAPI::getMaxTextureCount(); i++) {

			if (textures[i] == tex)
				return i;
		}

		return -1;
	}

	void Renderer2D::createInstance() {
		if (instance)
			return;
		instance = new Renderer2D();
	}

	void Renderer2D::deleteInstance() {
		delete instance;
	}

}