#include "glrenderapi.h"

#include "glinclude.h"
#include "gldebug.h"

#include "glshaderconstants.h"

#include "../../vendor/imgui/imgui.h"

#include "../gui/gui_s.h"

#include "../../stb/stb/stb_image.h"

#ifdef ARCHT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace archt {

	std::string GLRenderAPI::vendor = "N/A";
	std::string GLRenderAPI::version = "N/A";
	std::string GLRenderAPI::model = "N/A";
	std::string GLRenderAPI::shaderLanguageVersion = "N/A";

	
	int GLRenderAPI::availableMemory = 0;
	int GLRenderAPI::totalMemory = 0;
	int GLRenderAPI::maxTextures = 32;
	int GLRenderAPI::maxMatrices = 256;
	GLWindow* GLRenderAPI::window = nullptr;


	uint32_t GLRenderAPI::clearMask = 0;
	uint32_t GLRenderAPI::polygonMode = GL_TRIANGLES;


	GLWindow* GLRenderAPI::init() {

		
		if (glfwInit() != GLFW_TRUE) {
			printf("Failed to initialize GLFW!\n");
			__debugbreak();
		}

		{
			RECT desktop;
			HWND screen = GetDesktopWindow();
			GetWindowRect(screen, &desktop);

			int w = (int) (desktop.right * 0.66666f);
			int h = (int) (desktop.bottom * 0.66666f);


			int x = (int) (desktop.right * 0.166666f);
			int y = (int) (desktop.bottom  * 0.166666f);


			window = new GLWindow("Architect", x, y, w, h);
		}

		if (glewInit() != GLEW_OK) {
			printf("Failed to initialize GLEW!\n");
			__debugbreak();
		}

		glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &availableMemory);
		glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalMemory);
		

		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextures);
		GLShaderConstants::setConstant(GLShaderConstants::MAX_TEXTURES, &maxTextures);
		int maxVec4 = 0;
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &maxVec4);
		maxMatrices = 1000;
		
		int temp = 256;
		GLShaderConstants::setConstant(GLShaderConstants::MAX_MATRICES, &temp);


		vendor = std::string((char*) glGetString(GL_VENDOR));
		version = std::string((char*) glGetString(GL_VERSION));
		model = std::string((char*) glGetString(GL_RENDERER));
		shaderLanguageVersion = std::string((char*) glGetString(GL_SHADING_LANGUAGE_VERSION));

		int w;
		int h;
		int comp;
		unsigned char* data = stbi_load("src/assets/img/A.png", &w, &h, &comp, STBI_rgb_alpha);

		GLFWimage image[1];
		image[0].width = w;
		image[0].height = h;
		image[0].pixels = data;

		glfwSetWindowIcon(window->getHandle(), 1, image);

		stbi_image_free(data);

		return window;
	}

	void GLRenderAPI::terminate() {
		CALL(glfwTerminate());
	}

	void GLRenderAPI::enable(uint32_t feature) {
		glEnable(feature);
	}

	void GLRenderAPI::disable(uint32_t feature) {
		glDisable(feature);
	}

	void GLRenderAPI::blendFunc(uint32_t sFactor, uint32_t dFactor) {
		CALL(glBlendEquation(GL_FUNC_ADD));
		glBlendFunc(sFactor, dFactor);
	}

	void GLRenderAPI::setCullFace(uint32_t frontFace, uint32_t cullFace) {
		glEnable(GL_CULL_FACE);
		glFrontFace(frontFace);
		glCullFace(cullFace);
	}

	void GLRenderAPI::setPolygonMode(uint32_t face, uint32_t mode) {
		switch (mode) {
			case GL_LINE: 
				polygonMode = GL_LINES; 
				break;
			case GL_FILL: 
				polygonMode = GL_TRIANGLES; 
				break;
			default: 
				polygonMode = GL_TRIANGLES; 
				break;
		}
		glPolygonMode(face, mode);
	}


	void GLRenderAPI::setClearMask(uint32_t mask) {
		clearMask = mask;
	}

	void GLRenderAPI::addToClearMask(uint32_t mask) {
		clearMask |= mask;
	}

	void GLRenderAPI::removeFromClearMask(uint32_t mask) {
		clearMask &= ~mask;
	}

	int GLRenderAPI::queryAvailableMemory() {
		glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &availableMemory);
		return availableMemory;
	}

	void GLRenderAPI::createGuiInfoWindow() {
	
		auto window = [](bool* open, GuiWindow_s* handle) {
			ImGui::Begin("Video card info");

			ImVec4 green = { 0.0f, 1.0f, 0.0f, 1.0f };
			ImVec4 white = { 1.0f, 1.0f, 1.0f, 1.0f };
			ImVec4 turqoise = { 0.0f, 0.8f, 1.0f, 1.0f };

			ImGui::TextColored(white, "Vendor:\t\t");
			ImGui::SameLine();
			ImGui::TextColored(green, vendor.c_str());

			ImGui::TextColored(white, "Version:\t\t");
			ImGui::SameLine();
			ImGui::TextColored(green, version.c_str());

			ImGui::TextColored(white, "Model:\t\t");
			ImGui::SameLine();
			ImGui::TextColored(green, model.c_str());

			ImGui::TextColored(white, "GLSL version:\t\t");
			ImGui::SameLine();
			ImGui::TextColored(green, shaderLanguageVersion.c_str());
			
			ImGui::TextColored(white, "Total memory:\t");
			ImGui::SameLine();
			ImGui::TextColored(turqoise, std::to_string(totalMemory).c_str());
			

			ImGui::TextColored(white, "Available memory: ");
			ImGui::SameLine();
			ImGui::TextColored(turqoise, std::to_string(queryAvailableMemory()).c_str());


			ImGui::TextColored(white, "Texture slots:\t");
			ImGui::SameLine();
			ImGui::TextColored(turqoise, std::to_string(maxTextures).c_str());

			ImGui::End();
		};

		Gui_s::getInstance()->addGuiWindow_void(window);
	}

	void GLRenderAPI::setViewport(const glm::vec4& v) {
		glViewport(v.x, v.y, v.z, v.w);
	}

	int GLRenderAPI::queryTotalMemory() {
		return totalMemory;
	}

	void GLRenderAPI::setClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	int GLRenderAPI::getMaxTextureCount() {
		return maxTextures;
	}

	int GLRenderAPI::getMaxMatricesCount() {
		return maxMatrices;
	}

}