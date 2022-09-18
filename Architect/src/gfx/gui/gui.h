#pragma once

#include "guiwindow.h"
#include "../opengl/glwindow.h"

#include "../../vendor/imgui/imconfig.h"
#include "../../vendor/imgui/imgui_impl_glfw.h"
#include "../../vendor/imgui/imgui_impl_opengl3.h"
#include "../../vendor/imgui/imgui.h"


namespace archt {

	class Gui {

	public:
		static Gui* instance;

	private:

		GuiWindow* styleWindow = nullptr;
		GuiWindow* focusedWindow = nullptr;

		std::vector<GuiWindow*> constantWindows;
		std::vector<GuiWindow*> perFrameWindows;

		static const std::string styleFile;
		
		ImVec4 defaultColors[ImGuiCol_COUNT];

		bool docked = true;

		Gui(glm::ivec2 windowSize);
		~Gui();
	public:

		static void init(GLWindow* window);
		static void setStyle();
		static void terminate();

		void render();

		//void submitWindowVoid(std::function<void()> renderFunc);
		//GuiWindowBase* addGuiWindowVoid(std::function<void()> renderFunc);
		template <typename F, typename = void>
		void submitWindow(F&& f) {
			perFrameWindows.push_back(new GuiWindowVoid(((std::function<void()>) f)));
		}

		template <typename F, typename... Args>
		void submitWindow(F&& f, Args&&... args) {
			perFrameWindows.push_back(createGuiWindowArgs(f, std::forward<Args>(args)...));
		}

		template <typename F, typename = void>
		GuiWindow* addGuiWindow(F&& f) {
			
			constantWindows.push_back(new GuiWindowVoid(((std::function<void()>) f)));
			return constantWindows[constantWindows.size() - 1];
		}

		template <typename F, typename... Args>
		GuiWindow* addGuiWindow(F&& f, Args&&... args) {

			constantWindows.push_back(createGuiWindowArgs(f, std::forward<Args>(args)...));
			return constantWindows[constantWindows.size() - 1];
		}

		// closable windows------------------------------------------------
		template <typename F, typename = void, typename = bool*, typename = GuiWindow*>
		GuiWindow* addGuiWindow_s(F&& f) {

			constantWindows.push_back(new GuiWindowVoid_s(((std::function<void(bool*, GuiWindow*)>) f)));
			return constantWindows[constantWindows.size() - 1];
		}

		template <typename F, typename... Args>
		GuiWindow* addGuiWindow_s(F&& f, Args&&... args) {

			constantWindows.push_back(createGuiWindowArgs_s(f,  std::forward<Args>(args)...));
			return constantWindows[constantWindows.size() - 1];
		}

		void removeWindow(GuiWindow* window);

		void setDockingMode(bool mode);


		bool hasWindow(const char* name) const;

		void createStyleWindow();

		inline GuiWindow* getFocusedWindow() const { return focusedWindow; }

		void setFocusedWindow(GuiWindow* window);

	private:
		void renderDocked();
		void renderUndocked();


	};


}