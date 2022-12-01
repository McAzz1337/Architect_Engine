#include "wireframerenderer.h"

#include "../../scene/component_s.h"
#include "../opengl/gldebug.h"

namespace archt {


	const uint32_t WireframeRenderer::MAX_ENTITIES = 1000;
	const uint32_t WireframeRenderer::MAX_VERTECES = 4 * MAX_ENTITIES;
	const uint32_t WireframeRenderer::MAX_INDECES = 8 * MAX_ENTITIES;

	WireframeRenderer::WireframeRenderer() {
	
		entities = new Entity_s*[MAX_ENTITIES];

		vbo = new VBO(nullptr, MAX_VERTECES);
		vbo->allocateOnGPU();

		ibo = new IBO(nullptr, MAX_INDECES);
		ibo->allocateOnGPU();

		vao = new GLVertexarray(vbo, ibo);
	
		matrices = new Transform_s[MAX_ENTITIES];
	}
	
	WireframeRenderer::~WireframeRenderer() {
		
		delete vbo;
		delete ibo;
		delete vao;
		delete[] matrices;
		delete[] entities;
	}

	void WireframeRenderer::init() {
		if (!instance)
			instance = new WireframeRenderer();
	}
	
	WireframeRenderer* WireframeRenderer::getInstance() {
	
		return instance;
	}
	
	void WireframeRenderer::terminate() {
		
		delete instance;
	}

	void WireframeRenderer::beginScene(Camera_s* camera, Framebuffer* frameBuffer) {
		
		if (frameBuffer)
			fb = frameBuffer;

		if (camera)
			cam = camera;

		inScene = true;
	}
	
	void WireframeRenderer::endSCene() {
	
		inScene = false;
	}

	void WireframeRenderer::beginBatch() {

		vbo->bind();
		ibo->bind();

		const Transform_s viewProjection = cam->getViewProjection();
		
		for (int i = 0; i < currentIndex; i++) {

			Entity_s* entity = entities[i];
			VBO* vb = entity->getComponent<Mesh_s>().vbo;
			uint32_t vCount = vb->getSize();

			IBO* ib = entity->getComponent<Mesh_s>().ibo;
			uint32_t iCount = ib->getSize();



			if (vertexOffset + vCount > MAX_VERTECES ||
				indexOffset + iCount > MAX_INDECES) {

				draw();
				endBatch();
			}

			matrices[currentMatrix] = viewProjection * entity->getComponent<Transform_s>();
			vb->setMatrixId((float) currentMatrix);
			currentMatrix++;

			vbo->write(vertexOffset, vb->getData(), vb->getSize());
			vertexOffset += vb->getSize();

			ibo->write(indexOffset, ib->getData(), iCount, vertexOffset);
			indexOffset += iCount;
		}
	}
	
	void WireframeRenderer::endBatch() {
		
		currentIndex = 0;
		vertexOffset = 0;
		indexOffset = 0;
		currentMatrix = 0;
	}

	void WireframeRenderer::submit(Entity_s* entity) {
		
		if (!inScene)
			return;

		if (currentIndex == MAX_ENTITIES) {
			render();
			endSCene();
			beginScene();
		}

		entities[currentIndex] = entity;
		currentIndex++;

	}

	void WireframeRenderer::render() {
		
		if (!inScene)
			return;

		beginBatch();

		draw();

		endBatch();

	}

	void WireframeRenderer::draw() const {
		
		vao->bind();
		ibo->bind();

		// TODO: create and bind shader and upload matrices to uniform buffer

		CALL(glDrawElements(GL_LINES, ibo->getSize(), GL_UNSIGNED_INT, nullptr));

	}
}