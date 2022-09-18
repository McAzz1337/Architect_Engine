#pragma once

#include "../../ptr.h"
#include "../../entity/entity.h"
#include "../../entity/component/camera_new.h"

#include "../opengl/glvertexarray.h"
#include "../opengl/glshader.h"

#include "../opengl/framebuffer.h"


namespace archt {

	class Renderer{

	public:
		
		static Renderer* instance;

	private:
		
		static const int MAX_ENTITIES;
		static const int MAX_VERTECES;
		static const int MAX_INDECES;


		bool inScene = false;

		ptr<Camera_new> cam = nullptr;

		Framebuffer* fb = nullptr;

		
		ptr<Entity>* entities = nullptr;
		glm::mat4* transforms = nullptr;
		uint32_t* textures = nullptr;

		uint32_t currentEntity = 0;
		uint32_t currentTransform = 0;
		uint32_t currentTexture = 0;
		uint32_t currentVertex = 0;
		uint32_t currentIndex = 0;

		GLShader* activeShader = nullptr;

		VBO* vbo = nullptr;
		IBO* ibo = nullptr;
		GLVertexarray* vao = nullptr;

		Renderer();
		~Renderer();

	public:

		static void createInstance();
		static void deleteInstance();

		void setRenderSettings();

		void clear();
		void submit(ptr<Entity> entity);

		void beginScene(ptr<Camera_new> camera = nullptr);
		void endScene();

		void render();
		void render(ptr<Entity> entity, ptr<Camera_new> cam);
		void flush();

		void setRendertarget(Framebuffer* frameBuffer);

	private:
		void beginBatch();
		void endBatch();

		void sort();
		int findShaderMatch(int start, GLShader* shader);

		void draw();

		uint32_t fetchTextureSlot(uint32_t textureId);
		void flushTextures();

	};

}