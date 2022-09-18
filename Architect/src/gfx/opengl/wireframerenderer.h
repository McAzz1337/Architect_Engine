#pragma once
#include "buffers.h"
#include "glmesh.h"
#include "camera.h"
#include "glvertexarray.h"

namespace archt {

	class WireframeRenderer {

		static const int MAX_OBJECTS;
		static const int MAX_VERTECES;
		static const int MAX_INDECES;

		static Camera* cam;

		static int currentMesh;
		static GLMesh** meshes;
		static int currentMatrix;
		static glm::mat4* matrices;
		static glm::vec4* colors;

		static uint32_t currentVertex;
		static uint32_t currentIndex;

		static VBO* vbo;
		static IBO* ibo;
		static GLVertexarray* vao;

		static GLShader* shader;

	public:
		static void init();
		static void terminate();

		static void clear();

		static void submit(GLMesh* mesh, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f });

		static void beginScene(Camera* camera);
		static void endScene();

		static uint32_t* reorderIndeces(uint32_t* src, uint32_t size, uint32_t& out_size);

		static void beginBatch();
		static void endBatch();

		static void render();
		static void draw();

		static void flush();

	};
}