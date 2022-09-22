#pragma once


#pragma once

#include "guiwindow.h"
#include "../opengl/glwindow.h"

#include <imgui.h>
#include <imconfig.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>





namespace archt {

	class Gui {

		static Gui* instance;


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
		static Gui* getInstance();
		static void setStyle();
		static void terminate();

		void render();

		
		// closable windows------------------------------------------------
		template <typename = std::function<void(const char*, bool*, GuiWindow*)>>
		GuiWindow* addGuiWindow_void(const char* name, std::function<void(const char*, bool*, GuiWindow*)> f, CloseCallback callback = nullptr) {

			constantWindows.push_back(new GuiWindow(name, f, callback));
			return constantWindows[constantWindows.size() - 1];
		}

		template <typename F, typename = CloseCallback, typename... Args>
		GuiWindow* addGuiWindow_args(const char* name, F&& f, CloseCallback callback,  Args&&... args) {

			constantWindows.push_back(createGuiWindow_args(name, f, callback, std::forward<Args>(args)...));
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