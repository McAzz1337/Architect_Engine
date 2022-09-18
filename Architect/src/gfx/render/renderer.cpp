#include "renderer.h"

#include "../opengl/glrenderapi.h"
#include "../opengl/gldebug.h"


#include "../../entity/component/mesh.h"
#include "../../entity/component/material/material.h"
#include "../../entity/component/transform.h"

#include "../gui/gui.h"

#define RENDERER_NONE_FOUND -1

namespace archt {

	Renderer* Renderer::instance = nullptr;


	const int Renderer::MAX_ENTITIES = 1000;
	const int Renderer::MAX_VERTECES = MAX_ENTITIES * 4;
	const int Renderer::MAX_INDECES = MAX_ENTITIES * 6;

	Renderer::Renderer() {
		
		entities = new ptr<Entity>[MAX_ENTITIES];
		transforms = new glm::mat4[MAX_ENTITIES];
		textures = new uint32_t[GLRenderAPI::maxTextures];

		vbo = new VBO(nullptr, MAX_VERTECES);
		vbo->allocateOnGPU();
	
		ibo = new IBO(nullptr, MAX_INDECES);
		ibo->allocateOnGPU();

		vao = new GLVertexarray(vbo, ibo);
	}

	Renderer::~Renderer() {
		delete[] entities;
		delete[] transforms;
		delete[] textures;

		delete vbo;
		delete ibo;
		delete vao;
	}

	void Renderer::createInstance() {
		if (!instance) instance = new Renderer();
	}

	void Renderer::deleteInstance() {
		delete instance;
		instance = nullptr;
	}

	void Renderer::setRenderSettings() {
		GLRenderAPI::setClearColor({ 0.3f, 0.3f, 0.3f, 1.0f });

		GLRenderAPI::enable(GL_DEPTH);

		GLRenderAPI::enable(GL_BLEND);
		GLRenderAPI::blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GLRenderAPI::setCullFace(GL_CW, GL_BACK);

		GLRenderAPI::setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		vbo->bind();
		ibo->bind();
		vao->bind();
	}

	void Renderer::clear() {
		glClear(GLRenderAPI::clearMask);
	}

	void Renderer::submit(ptr<Entity> entity) {
		if (!inScene)
			return;

		if (currentEntity == MAX_ENTITIES) {
			render();
			flush();
			endScene();
			beginScene();
		}

		entities[currentEntity] = entity;
		currentEntity++;
	}

	void Renderer::beginScene(ptr<Camera_new> camera) {
		inScene = true;
		
		//leave in for camera reusability functionality between render passes
		if (camera != nullptr) 
			cam = camera;

	}

	void Renderer::endScene() {
		inScene = false;
		currentEntity = 0;

		fb->unbind();
		fb = nullptr;
	}

	void Renderer::render() {
	
		sort();
		beginBatch();
		draw();
		endBatch();
	}

	void Renderer::render(ptr<Entity> entity, ptr<Camera_new> cam) {
		VBO* vbo = entity->getComponent<Mesh>()->getVBO();
		IBO* ibo = entity->getComponent<Mesh>()->getIBO();

		GLVertexarray* vao = new GLVertexarray(vbo, ibo);
	
		GLShader* shader = &entity->getComponent<Mesh>()->getComponent<Material>()->getShader();
		shader->bind();
		vbo->bind();
		ibo->bind();
		vao->bind();

		Transform mvp = cam->getProjectionView() * entity->getComponent<Mesh>()->getTransform();

		shader->setMat4("mvp", mvp.getMatrix());

		GLTexture* tex = entity->getComponent<Mesh>()->getComponent<Material>()->getTexture();
		tex->bind(0);
		vbo->setTexId(0.0f);
		vbo->upload();
		ibo->upload();

		CALL(glDrawElements(GL_TRIANGLES, ibo->getSize(), GL_UNSIGNED_INT, nullptr));

		delete vao;
	}

	void Renderer::flush() {
	
	}

	void Renderer::setRendertarget(Framebuffer* frameBuffer) {
		
		fb = frameBuffer;
		
		if (fb)
			fb->bind();
	}


	void Renderer::beginBatch() {
		
		activeShader = &entities[0]->getComponent<Mesh>()->getComponent<Material>()->getShader();
		Transform projectionView = cam->getProjectionView();

		for (int i = 0; i < currentEntity; i++) {

			ptr<Entity> entity = entities[i];
			ptr<Mesh> mesh = entity->getComponent<Mesh>();
			ptr<Material> material = mesh->getComponent<Material>();
			GLTexture* tex = material->getTexture();

			VBO* vb = mesh->getVBO();
			IBO* ib = mesh->getIBO();

			uint32_t vSize = vb->getSize();
			uint32_t iSize = ib->getSize();

			int texSlot = fetchTextureSlot(tex->getId());

			if (activeShader != &entities[i]->getComponent<Mesh>()->getComponent<Material>()->getShader() ||
				(texSlot == RENDERER_NONE_FOUND && currentTexture == GLRenderAPI::maxTextures) ||
				currentVertex + vSize >= MAX_VERTECES ||
				currentIndex + iSize >= MAX_INDECES) {

				draw();
				endBatch();
			}

			if (texSlot == -1) {
				tex->bind(currentTexture);
				vb->setTexId((float) currentTexture);
				textures[currentTexture] = tex->getId();
				currentTexture++;
			}
			else {
				vb->setTexId((float) texSlot);
			}

			transforms[currentTransform] = (projectionView * mesh->getTransform()).getMatrix();
			vb->setMatrixId((float) currentTransform);
			currentTransform++;

			vbo->write(currentVertex, vb->getData(), vSize);
			ibo->write(currentIndex, ib->getData(), iSize, currentVertex);

			currentVertex += vSize;
			currentIndex += iSize;
		}

	}

	void Renderer::endBatch() {
		currentTransform = 0;
		currentVertex = 0;
		currentIndex = 0;
	}

	void Renderer::sort() {

		for (int i = 0; i < currentEntity - 1; i++) {
			GLShader* shader = &entities[i]->getComponent<Mesh>()->getComponent<Material>()->getShader();
			GLShader* nextShader= &entities[i + 1]->getComponent<Mesh>()->getComponent<Material>()->getShader();
			
			if (nextShader == shader) 
				continue;

			int match = findShaderMatch(i + 1, shader);
			if (match == RENDERER_NONE_FOUND)
				continue;

			ptr<Entity> temp = entities[i + 1];
			entities[i + 1] = entities[match];
			entities[match] = temp;
 		}
	}

	int Renderer::findShaderMatch(int start, GLShader* shader) {
		
		for (int i = start; i < currentEntity; i++) {
			GLShader* nextShader = &entities[i]->getComponent<Mesh>()->getComponent<Material>()->getShader();
			if (nextShader == shader) {
				return i;
			}
		}

		return -1;
	}

	void Renderer::draw() {
		activeShader->bind();
		
		std::vector<Uniformbuffer*> buffers = activeShader->getUniformBuffers();
		for (int i = 0; i < buffers.size(); i++) {
			buffers[i]->bind();
			if (buffers[i]->getName() == "matrices") {
				buffers[i]->write(0, transforms, currentTransform * sizeof(glm::mat4));
				//buffers[i]->bindUniformBlock(activeShader->getProgramId(), "matrices");
				buffers[i]->upload();
			}
		}
		vbo->upload();
		ibo->upload();

		CALL(glDrawElements(GL_TRIANGLES, currentIndex, GL_UNSIGNED_INT, nullptr));

	}

	uint32_t Renderer::fetchTextureSlot(uint32_t textureId) {
		for (int i = 0; i < currentTexture; i++) {
			if (textures[i] == textureId)
				return i;
		}

		return -1;
	}

	void Renderer::flushTextures() {
		for (int i = 0; i < GLRenderAPI::maxTextures; i++)
			textures[i] = 0;
	}

}