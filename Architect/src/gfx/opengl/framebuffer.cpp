#include "framebuffer.h"

#include "glinclude.h"
#include "gldebug.h"
#include "glrenderapi.h"
#include "../gui/gui_s.h"

namespace archt {

	Framebuffer::Framebuffer(const glm::ivec2& windowSize) : w(windowSize.x), h(windowSize.y) {

		CALL(glGenFramebuffers(1, &id));



		tex = GLTexture::createEmptyTexture(w,h);

		CALL(glGenRenderbuffers(1, &rbo));
		CALL(glBindRenderbuffer(GL_RENDERBUFFER, rbo));

		CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h));

		CALL(glBindFramebuffer(GL_FRAMEBUFFER, id));

		CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->getId(), 0));

		CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));


		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			printf("Failed to create framebuffer\n");
			__debugbreak();
		}

		uint32_t vSize = 4;
		float aspect = (float) w / (float) h;
		Vertex* verteces = new Vertex[4]{
			Vertex({ -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f }, 0.0f, 0.0f),
			Vertex({  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, 0.0f, 0.0f),
			Vertex({  0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f }, 0.0f, 0.0f),
			Vertex({ -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }, 0.0f, 0.0f)
		};

		uint32_t iSize = 6;
		uint32_t* indeces = new uint32_t[iSize]{
			0, 1, 2,
			0, 2, 3
		};


		vbo.setVerteces(verteces, vSize);
		ibo.setIndeces(indeces, iSize);

		auto lambda = [this](bool* open, GuiWindow_s* handle) {


			std::string windowName = "Framebuffer " + std::to_string(id);
			ImGui::Begin(windowName.c_str());

			if (ImGui::IsWindowFocused()) {
				Gui_s::getInstance()->setFocusedWindow(handle);
			}

			ImVec2 textureSize(w, h);
			ImVec2 initialCursorPos = ImGui::GetCursorPos();
			ImVec2 windowSize = ImGui::GetWindowSize();
			
			ImVec2 centralizedCursorpos;
			bool isCenter = true;
			if (w < windowSize.x) {
				centralizedCursorpos.x = (windowSize.x - (float) w) * 0.5f;
				isCenter = false;
			}
			if (h < windowSize.y) {
				centralizedCursorpos.y = (windowSize.y - (float) h) * 0.5f;
				isCenter = false;
			}
			if (!isCenter) {
				ImGui::SetCursorPos(centralizedCursorpos);
			}

			
			ImGui::Image((ImTextureID) tex->getId(), textureSize, { 0, 1 }, { 1, 0});

			ImGui::End();


		};
		guiWindow = Gui_s::getInstance()->addGuiWindow_void(lambda);
	}

	Framebuffer::~Framebuffer() {
		//if (Gui::instance)
		//	Gui::instance->removeWindow(guiWindow);

		delete tex;

		glDeleteFramebuffers(1, &id);
	}

	void Framebuffer::bind() const {
		CALL(glBindFramebuffer(GL_FRAMEBUFFER, id));
	}
	void Framebuffer::unbind() const {
		CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
	
	
	void Framebuffer::installViewPortCallback(GLWindow* window) {
		//std::function<void(int, int)> s = std::bind(&Framebuffer::viewportCallback, this, std::placeholders::_1, std::placeholders::_2);
		window->addViewportCallback(std::bind(&Framebuffer::viewportCallback, this, std::placeholders::_1, std::placeholders::_2));
	}

	void Framebuffer::viewportCallback(int width, int height) {

		w = width;
		h = height;

		CALL(glBindFramebuffer(GL_FRAMEBUFFER, id));

	
		CALL(glDeleteRenderbuffers(1, &rbo));
		delete tex;

		tex = GLTexture::createEmptyTexture(w, h);


		CALL(glGenRenderbuffers(1, &rbo));
		CALL(glBindRenderbuffer(GL_RENDERBUFFER, rbo));

		CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h));

		CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->getId(), 0));

		CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));
	}
}