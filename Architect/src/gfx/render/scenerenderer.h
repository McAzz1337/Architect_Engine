#pragma once

#include "../../scene/scene.h"
#include "../../scene/entity_s.h"

#include <entt/entt.hpp>

#include "../../entity/component/camera_new.h"
#include "../opengl/glvertexarray.h"
#include "../opengl/glshader.h"
#include "../opengl/framebuffer.h"

#include <typeindex>

namespace archt {


	class SceneRenderer {

		static SceneRenderer* instance;


		static const int MAX_ENTITIES;
		static const int MAX_VERTECES;
		static const int MAX_INDECES;


		bool inScene = false;

		ptr<Camera_new> cam = nullptr;

		Framebuffer* fb = nullptr;

		Scene* scene = nullptr;
		Entity_s* entities = nullptr;
		glm::mat4* transforms = nullptr;
		uint32_t* textures = nullptr;

		uint32_t currentEntity = 0;
		uint32_t currentTransform = 0;
		uint32_t currentTexture = 0;
		uint32_t currentVertex = 0;
		uint32_t currentIndex = 0;

		ptr<GLShader> activeShader = nullptr;

		VBO* vbo = nullptr;
		IBO* ibo = nullptr;
		GLVertexarray* vao = nullptr;

	private:

		SceneRenderer();
		~SceneRenderer();
	public:

		static void createInstance();
		static SceneRenderer* getInstance();
		static void deleteInstance();

		void setRenderSettings();

		void clear();

		void beginScene(Scene* s = nullptr, ptr<Camera_new> camera = nullptr);
		void endScene();

		void submit(Entity_s entity);

		void render();
		void flush();

		void setRendertarget(Framebuffer* frameBuffer);

	private:
		void beginBatch();
		void endBatch();

		void sort();
		int findShaderMatch(int start, ptr<GLShader> shader);

		void draw();

		uint32_t fetchTextureSlot(uint32_t textureId);
		void flushTextures();
	};
}