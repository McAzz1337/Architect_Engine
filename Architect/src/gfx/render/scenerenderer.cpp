#include "scenerenderer.h"

#include "../opengl/glrenderapi.h"
#include "../opengl/gldebug.h"

#include "../../scene/component_s.h"

#include "../gui/gui_s.h"

namespace archt {

#define RENDERER_NONE_FOUND -1

	SceneRenderer* SceneRenderer::instance = nullptr;


	const int SceneRenderer::MAX_ENTITIES = 1000;
	const int SceneRenderer::MAX_VERTECES = MAX_ENTITIES * 4;
	const int SceneRenderer::MAX_INDECES = MAX_ENTITIES * 6;

	SceneRenderer::SceneRenderer() {
		entities = new Entity_s[MAX_ENTITIES];
		transforms = new glm::mat4[MAX_ENTITIES];
		textures = new uint32_t[GLRenderAPI::maxTextures];

		vbo = new VBO(nullptr, MAX_VERTECES);
		vbo->allocateOnGPU();

		ibo = new IBO(nullptr, MAX_INDECES);
		ibo->allocateOnGPU();

		vao = new GLVertexarray(vbo, ibo);
	}

	SceneRenderer::~SceneRenderer() {
		delete[] entities;
		delete[] transforms;
		delete[] textures;

		delete vbo;
		delete ibo;
		delete vao;
	}

	void SceneRenderer::createInstance() {
		if (!instance) instance = new SceneRenderer();
	}

	SceneRenderer* SceneRenderer::getInstance() {
		return instance;
	}

	void SceneRenderer::deleteInstance() {
		if (instance) {
			delete instance;
			instance = nullptr;
		}
	}

	void SceneRenderer::setRenderSettings() {
	
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

	void SceneRenderer::clear() {
		glClear(GLRenderAPI::clearMask);
	}


	void SceneRenderer::beginScene(Scene* s, ptr<Camera_new> camera) {
		inScene = true;
		
		if (scene) 
			scene = s;
		
		if (camera)
			cam = camera;
		
		//entities = scene->getRenderables();
	}

	void SceneRenderer::endScene() {

		inScene = false;

		fb->unbind();
		fb = nullptr;

		currentEntity = 0;
	}

	void SceneRenderer::submit(Entity_s entity) {
		if (!inScene)
			return;

		if (currentEntity == MAX_ENTITIES) {
			render();
			endScene();
			beginScene();
		}

		entities[currentEntity] = entity;
		currentEntity++;
	}

	void SceneRenderer::render() {
	
		if (currentEntity == 0)
			return;

		sort();
		beginBatch();
		draw();
		endBatch();
	}


	void SceneRenderer::flush() {}

	void SceneRenderer::setRendertarget(Framebuffer* frameBuffer) {
	
		fb = frameBuffer;

		if (fb)
			fb->bind();
	}

	void SceneRenderer::beginBatch() {


	
		activeShader = entities[0].getComponent<Material_s>().shader;
		Transform_s projectionView = cam->getProjectionView().getMatrix(); // todo switch camera
		
		for (int i = 0; i < currentEntity; i++) {
		
			Entity_s* entity = &entities[i];
			Mesh_s& mesh = entity->getComponent<Mesh_s>();
			Material_s& material = entity->getComponent<Material_s>();
			ptr<GLTexture> tex = material.tex;
		
			VBO* vb = mesh.vbo;
			IBO* ib = mesh.ibo;
		
			uint32_t vSize = vb->getSize();
			uint32_t iSize = ib->getSize();
		
			int texSlot = fetchTextureSlot(tex->getId());
		
			if (activeShader != material.shader ||
				(texSlot == RENDERER_NONE_FOUND && currentTexture == GLRenderAPI::maxTextures) ||
				currentVertex + vSize >= MAX_VERTECES ||
				currentIndex + iSize >= MAX_INDECES) {
		
				draw();
				endBatch();
				activeShader = material.shader;
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

			transforms[currentTransform] = (projectionView * entity->getComponent<Transform_s>());
			vb->setMatrixId((float) currentTransform);
			currentTransform++;
		
			vbo->write(currentVertex, vb->getData(), vSize);
			ibo->write(currentIndex, ib->getData(), iSize, currentVertex);
		
			currentVertex += vSize;
			currentIndex += iSize;
			
		
		}
	
	}

	void SceneRenderer::endBatch() {
		currentTransform = 0;
		currentVertex = 0;
		currentIndex = 0;
	}

	void SceneRenderer::sort() {
		

		for (int i = 0; i < currentEntity - 1; i++) {
			ptr<GLShader> shader = entities[i].getComponent<Material_s>().shader;
			ptr<GLShader> nextShader = entities[i].getComponent<Material_s>().shader;

			if (nextShader == shader)
				continue;

			int match = findShaderMatch(i + 1, shader);
			if (match == RENDERER_NONE_FOUND)
				continue;

			Entity_s temp = entities[i + 1];
			entities[i + 1] = entities[match];
			entities[match] = temp;
		}
	}

	int SceneRenderer::findShaderMatch(int start, ptr<GLShader> shader) {
		
		for (int i = start; i < currentEntity; i++) {
			ptr<GLShader> nextShader = entities[i].getComponent<Material_s>().shader;
			if (nextShader == shader) {
				return i;
			}
		}

		return -1;
	}

	void SceneRenderer::draw() {
		
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
		vbo->upload(0, currentVertex);
		ibo->upload(0, currentIndex);

		


		CALL(glDrawElements(GL_TRIANGLES, currentIndex, GL_UNSIGNED_INT, nullptr));

	}

	uint32_t SceneRenderer::fetchTextureSlot(uint32_t textureId) {
	
		for (int i = 0; i < currentTexture; i++) {
			if (textures[i] == textureId)
				return i;
		}

		return -1;
	}

	void SceneRenderer::flushTextures() {
	
		for (int i = 0; i < GLRenderAPI::maxTextures; i++)
			textures[i] = 0;
	}

}