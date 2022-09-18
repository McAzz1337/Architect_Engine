#include "glrenderer2d.h"
#include "glinclude.h"

#include "glrenderapi.h"

#include "glshaderconstants.h"

#include "../../fileio.h"

namespace archt {

	int drawcalls = 0;

	const int GLRenderer2D::MAX_OBJECTS = 10000;
	const int GLRenderer2D::MAX_VERTECES = 4 * MAX_OBJECTS;
	const int GLRenderer2D::MAX_INDECES = 6 * MAX_OBJECTS;


	uint32_t GLRenderer2D::currentVertex = 0;
	uint32_t GLRenderer2D::currentIndex = 0;
	int GLRenderer2D::currentTexture = 0;
	uint32_t GLRenderer2D::currentMatrix = 0;
	uint32_t GLRenderer2D::currentMesh = 0;

	GLShader* GLRenderer2D::activeShader = nullptr;

	bool GLRenderer2D::inScene = false;

	VBO* GLRenderer2D::vbo = nullptr;
	IBO* GLRenderer2D::ibo = nullptr;
	GLVertexarray* GLRenderer2D::vao = nullptr;

	GLMesh** GLRenderer2D::meshes = nullptr;
	glm::mat4* GLRenderer2D::matrices = nullptr;
	int* GLRenderer2D::textures = nullptr;



	Camera* GLRenderer2D::cam = nullptr;

	void GLRenderer2D::init() {

		meshes = new GLMesh * [MAX_OBJECTS];


		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

		GLRenderAPI::enable(GL_DEPTH_TEST);
		GLRenderAPI::enable(GL_BLEND);

		GLRenderAPI::blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		GLRenderAPI::setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLRenderAPI::setCullFace(GL_CW, GL_BACK);

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


		matrices = new glm::mat4[GLRenderAPI::maxMatrices];

		textures = new int[GLRenderAPI::maxTextures];
		for (int i = 0; i < GLRenderAPI::maxTextures; i++) {
			textures[i] = 0;
		}
	}

	void GLRenderer2D::terminate() {
		delete ibo;
		delete vbo;
		delete vao;
	}

	void GLRenderer2D::beginScene(Camera* camera) {
		inScene = true;
		cam = camera;
	}
	void GLRenderer2D::endScene() {
		inScene = false;
		//printf("%i drawcalls per scene\n", drawcalls);
		currentMesh = 0;
		drawcalls = 0;
	}

	void GLRenderer2D::submit(GLMesh* mesh) {
		if (!inScene)
			return;

		if (currentMesh == MAX_OBJECTS) {
			render();
			endScene();
			beginScene(cam);
		}

		meshes[currentMesh] = mesh;
		currentMesh++;
	}


	void GLRenderer2D::clear() {
		glClear(GLRenderAPI::clearMask);
	}

	void GLRenderer2D::startBatch() {

		activeShader = meshes[0]->getShader();
		const glm::mat4 projectionView = cam->getProjectionView();

		for (int i = 0; i < currentMesh; i++) {

			GLMesh* mesh = meshes[i];

			VBO* vb = mesh->getVBO();
			IBO* ib = mesh->getIBO();

			uint32_t vSize = vb->getSize();
			uint32_t iSize = ib->getSize();

			GLTexture* tex = mesh->getTexture();
			int texIndex = fetchTextureIndex(tex->getId());

			if (currentVertex + vSize >= MAX_VERTECES ||
				currentIndex + iSize >= MAX_INDECES ||
				(texIndex == -1 && currentTexture == GLRenderAPI::maxTextures) ||
				currentMatrix == GLRenderAPI::maxMatrices ||
				mesh->getShader() != activeShader) {

				draw();
				flush();
				endBatch();

				activeShader = mesh->getShader();
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
			matrices[currentMatrix] = (projectionView * mesh->getModelMatrix());

			vbo->write(currentVertex, vb->getData(), vSize);
			ibo->write(currentIndex, ib->getData(), iSize, currentVertex);

			currentVertex += vSize;
			currentIndex += iSize;
			currentMatrix++;
		}
	}

	void GLRenderer2D::endBatch() {
		currentTexture = 0;
	}



	void GLRenderer2D::sort() {

		int index = 0;
		for (int i = 0; i < currentMesh - 1; i++) {
			GLShader* currentShader = meshes[i]->getShader();
			GLShader* nextShader = meshes[i + 1]->getShader();
			if (nextShader != currentShader) {
				index = i + 1;
				for (int j = i + 2; j < currentMesh; j++) {
					nextShader = meshes[j]->getShader();
					if (nextShader == currentShader) {
						GLMesh* mesh = meshes[j];
						meshes[j] = meshes[index];
						meshes[index] = mesh;
						index++;
					}
				}
				i = index;
			}
		}
	}


	void GLRenderer2D::render() {
		if (!inScene || currentMesh == 0)
			return;

		sort();
		startBatch();
		draw();
		flush();
		endBatch();
	}

	void GLRenderer2D::draw() {

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
				}
				buffers[i]->upload();
			}
		}
		else {
			activeShader->setMatrixf4v("mvp", matrices, currentMatrix);
		}
		glDrawElements(GL_TRIANGLES, currentIndex, GL_UNSIGNED_INT, nullptr);
		//printf("%i meshesdrawn in drawcall %i\n", currentMatrix, drawcalls);
		drawcalls++;
	}

	void GLRenderer2D::flush() {

		//for (int i = 0; i < textures.size(); i++) {
		//	textures[i] = nullptr; 
		//}

		currentVertex = 0;
		currentIndex = 0;
		currentTexture = 0;
		currentMatrix = 0;
	}


	void GLRenderer2D::flushTextures() {
		for (int i = 0; i < GLRenderAPI::getMaxTextureCount(); i++)
			textures[i] = 0;

		currentTexture = 0;
	}

	int GLRenderer2D::fetchTextureIndex(int tex) {

		for (int i = 0; i < GLRenderAPI::getMaxTextureCount(); i++) {

			if (textures[i] == tex)
				return i;
		}

		return -1;
	}



}