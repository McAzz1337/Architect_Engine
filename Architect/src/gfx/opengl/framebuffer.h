#pragma once

#include "glwindow.h"
#include "gltexture.h"
#include "buffers.h"

#include <glm/vec2.hpp>
#include <typeindex>

#include "../gui/guiwindow_s.h"

namespace archt {


	class Framebuffer {

		uint32_t id = 0;
		uint32_t rbo = 0;
		GLTexture* tex = nullptr;

		int w = 0;
		int h = 0;

		VBO vbo;
		IBO ibo;

		GuiWindow_s* guiWindow = nullptr;

	public:
		Framebuffer() = delete;
		Framebuffer(const glm::ivec2& windowSize);
		~Framebuffer();


		void bind() const;
		void unbind() const;

		void installViewPortCallback(GLWindow* window); 
		void viewportCallback(int width, int height);

		inline  operator const VBO& () const { return vbo; }
		inline  operator const IBO& () const { return ibo; }

		inline const VBO& getVBO() const { return vbo; }
		inline const IBO& getIBO() const { return ibo; }
		inline GLTexture* getTexture() const { return tex; }
		inline GuiWindow_s* getGuiWindow() const { return guiWindow; }

	};


}