#pragma once

#include "../../ptr.h"

#include "../../entity/entity.h"
#include "../../entity/component/camera_new.h"
#include "../../entity/component/material/material.h"
#include "../../entity/component/transform.h"

#include "../opengl/buffers.h"
#include "../opengl/glvertexarray.h"
#include "../opengl/glshader.h"

#include <glm/mat4x4.hpp>
#include <vector>
#include <typeindex>

namespace archt {

	class Renderer2D {

	public:

		static Renderer2D* instance;

	private:

		static const  int MAX_OBJECTS;
		static const  int MAX_VERTECES;
		static const  int MAX_INDECES;


		uint32_t currentVertex = 0;
		uint32_t currentIndex = 0;
		int currentTexture = 0;
		uint32_t currentMatrix = 0;
		uint32_t currentEntity = 0;

		GLShader* activeShader = nullptr;

		bool inScene = false;
		VBO* vbo = nullptr;
		IBO* ibo = nullptr;
		GLVertexarray* vao = nullptr;
		ptr<Camera_new> cam = nullptr;
		ptr<Entity>* entities = nullptr;
		Transform* matrices = nullptr;
		int* textures = nullptr;


		Transform* t = nullptr;
		VBO* v = nullptr;
		IBO* ibo_ptr = nullptr;

		Renderer2D();
		~Renderer2D();
	
	public:
		void beginScene(ptr<Camera_new> camera);
		void endScene();

		void submit(ptr<Entity> entity);

		void clear();
		void render();
		void flush();
		
	private:
		void beginBatch();
		void endBatch();

		uint32_t* reorderIndeces(uint32_t* src, int size, int& out_size);
		void sort();
		void draw();

		void flushTextures();
		int fetchTextureIndex(int tex);

	public:
		static void createInstance();
		static void deleteInstance();

	};


}