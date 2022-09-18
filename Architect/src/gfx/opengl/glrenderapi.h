#pragma once
#include <typeindex>

#include "glrenderer2d.h"
#include "glwindow.h"

namespace archt {


	class GLRenderAPI {

		static std::string vendor;
		static std::string version;
		static std::string model;
		static std::string shaderLanguageVersion;

		static int availableMemory;
		static int totalMemory;
		static uint32_t clearMask;
		static uint32_t polygonMode;

		static GLWindow* window;

		static int maxTextures;
		static int maxMatrices;


	public:
		GLRenderAPI() = delete;

		friend class GLRenderer2D;
		friend class WireframeRenderer;
		friend class Renderer2D;
		friend class Renderer;
		friend class SceneRenderer;

		static GLWindow* init();
		static void terminate();
		
		static void setClearColor(const glm::vec4& color);

		static void enable(uint32_t feature);
		static void disable(uint32_t feature);

		static void blendFunc(uint32_t sFactor, uint32_t dFactor);
		static void setCullFace(uint32_t frontFace, uint32_t cullFace);
		static void setPolygonMode(uint32_t face, uint32_t mode);

		static void setClearMask(uint32_t mask);
		static void addToClearMask(uint32_t mask);
		static void removeFromClearMask(uint32_t mask);

		static void setViewport(const glm::vec4& v);


		static int queryAvailableMemory();
		static int queryTotalMemory();
		static int getMaxTextureCount();
		static int getMaxMatricesCount();

		static void createGuiInfoWindow();
	};
}