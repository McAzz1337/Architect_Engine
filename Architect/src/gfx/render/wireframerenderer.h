#pragma once

#include <typeindex>

#include "../../scene/entity_s.h"
#include "../../scene/component_s.h"

#include "../opengl/buffers.h"
#include "../opengl/glvertexarray.h"
#include "../opengl/framebuffer.h"
#include "ptr.h"
#include "entity/component/camera_new.h"


namespace archt {
	class WireframeRenderer {

		static const uint32_t MAX_ENTITIES;
		static const uint32_t MAX_VERTECES;
		static const uint32_t MAX_INDECES;

		static WireframeRenderer* instance;

		uint32_t currentIndex = 0;
		uint32_t vertexOffset = 0;
		uint32_t indexOffset = 0;
		uint32_t currentTransform = 0;

		Entity_s** entities = nullptr;
		VBO* vbo = nullptr;
		IBO* ibo = nullptr;
		GLVertexarray* vao = nullptr;
		Transform_s* transforms = nullptr;

		Framebuffer* fb = nullptr;
		Camera_s* cam = nullptr;
		bool inScene = false;



	public:
		WireframeRenderer();
		~WireframeRenderer();

		static void init();
		static WireframeRenderer* getInstance();
		static void terminate();

		void setRenderSettings() const;

		void clear() const;

		void beginScene(Scene* scene = nullptr, Camera_s* camera = nullptr);
		void endScene();

		void beginBatch();
		void endBatch();

		void submit(Entity_s* entity);

		void render();

		void flush();

		void setRenderTarget(Framebuffer& fb);

	private:
		void draw() const;


	};
}