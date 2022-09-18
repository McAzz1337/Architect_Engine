#pragma once

#include "glmesh.h"
#include "buffers.h"
#include "glvertexarray.h"
#include "camera.h"

#include <glm/mat4x4.hpp>

#include <vector>
#include <typeindex>

namespace archt {

	class GLRenderer2D {

		const static int MAX_OBJECTS;
		const static int MAX_VERTECES;
		const static int MAX_INDECES;


		static uint32_t currentVertex;
		static uint32_t currentIndex;
		static int currentTexture;
		static uint32_t currentMatrix;
		static uint32_t currentMesh;

		static GLShader* activeShader;

		static bool inScene;
		static VBO* vbo;
		static IBO* ibo;
		static GLVertexarray* vao;
		static Camera* cam;
		static GLMesh** meshes;
		static glm::mat4* matrices;
		static int* textures;

	public:
		GLRenderer2D() = delete;
		static void init();
		static void terminate();

		static void beginScene(Camera* camera);
		static void endScene();

		static void submit(GLMesh* meh);

		static void clear();
		static void startBatch();
		static void endBatch();

		static uint32_t* reorderIndeces(uint32_t* src, int size, int& out_size);
		static void sort();
		static void render();
		static void draw();
		static void flush();

		static void flushTextures();
		static int fetchTextureIndex(int tex);

	};


}