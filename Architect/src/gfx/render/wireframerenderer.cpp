#include "wireframerenderer.h"

#include "../../scene/component_s.h"
#include "../opengl/gldebug.h"
#include "../opengl/glrenderapi.h"


namespace archt {


	const uint32_t WireframeRenderer::MAX_ENTITIES = 1000;
	const uint32_t WireframeRenderer::MAX_VERTECES = 4 * MAX_ENTITIES;
	const uint32_t WireframeRenderer::MAX_INDECES = 8 * MAX_ENTITIES;

	WireframeRenderer* WireframeRenderer::instance = nullptr;


	WireframeRenderer::WireframeRenderer() {

		entities = new Entity_s * [MAX_ENTITIES];

		vbo = new VBO(nullptr, MAX_VERTECES);
		vbo->allocateOnGPU();

		ibo = new IBO(nullptr, MAX_INDECES);
		ibo->allocateOnGPU();

		vao = new GLVertexarray(vbo, ibo);

		transforms = new Transform_s[MAX_ENTITIES];
	}

	WireframeRenderer::~WireframeRenderer() {

		delete vbo;
		delete ibo;
		delete vao;
		delete[] transforms;
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

	void WireframeRenderer::clear() const {

		CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}


	void WireframeRenderer::beginScene(Scene* scene, Camera_s* camera) {

		if (camera)
			cam = camera;

		inScene = true;
	}

	void WireframeRenderer::endScene() {

		if (fb) {

			fb->unbind();
		}

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

			transforms[currentTransform] = viewProjection * entity->getComponent<Transform_s>();
			vb->setMatrixId((float) currentTransform);
			currentTransform++;

			vbo->write(vertexOffset, vb->getData(), vb->getSize());
			vertexOffset += vb->getSize();

			std::vector<uint32_t> indeces;
			ib->asLineIndeces(indeces);

			ibo->write(indexOffset, indeces.data(), indeces.size(), vertexOffset);
			indexOffset += iCount;
		}
	}

	void WireframeRenderer::endBatch() {

		currentIndex = 0;
		vertexOffset = 0;
		indexOffset = 0;
		currentTransform = 0;
	}

	void WireframeRenderer::submit(Entity_s* entity) {

		if (!inScene)
			return;

		if (currentIndex == MAX_ENTITIES) {
			render();
			endScene();
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

	void WireframeRenderer::flush() {


	}


	void WireframeRenderer::setRenderSettings() const {

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

	void WireframeRenderer::draw() const {

		vao->bind();
		
		vbo->bind();
		vbo->upload(0, vertexOffset);
		
		ibo->bind();
		ibo->upload(0, currentIndex);

		if (fb) {
			fb->bind();
		}

		GLShader* shader = GLRenderAPI::getDefaultShader();
		shader->bind();
		std::vector<Uniformbuffer*> ubs = shader->getUniformBuffers();

		for (Uniformbuffer* ub : ubs) {

			ub->bind();
			if (ub->getName() == "matrices") {
				ub->write(0, transforms, currentTransform * sizeof(glm::mat4));
				//buffers[i]->bindUniformBlock(activeShader->getProgramId(), "matrices");
				ub->upload();
			}
		}


		// TODO: create and bind shader and upload matrices to uniform buffer

		CALL(glDrawElements(GL_LINES, ibo->getSize(), GL_UNSIGNED_INT, nullptr));


	}

	void WireframeRenderer::setRenderTarget(Framebuffer& fb) {

		this->fb = &fb;

		if (this->fb)
			this->fb->bind();
	}

}