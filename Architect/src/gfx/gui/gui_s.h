#pragma once


#pragma once

#include "guiwindow_s.h"
#include "../opengl/glwindow.h"

#include <imgui.h>
#include <imconfig.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>





namespace archt {

	class Gui_s {

		static Gui_s* instance;


		GuiWindow_s* styleWindow = nullptr;
		GuiWindow_s* focusedWindow = nullptr;

		std::vector<GuiWindow_s*> constantWindows;
		std::vector<GuiWindow_s*> perFrameWindows;

		static const std::string styleFile;

		ImVec4 defaultColors[ImGuiCol_COUNT];

		bool docked = true;

		Gui_s(glm::ivec2 windowSize);
		~Gui_s();
	public:

		static void init(GLWindow* window);
		static Gui_s* getInstance();
		static void setStyle();
		static void terminate();

		void render();

		
		// closable windows------------------------------------------------
		template <typename = std::function<void(const char*, bool*, GuiWindow_s*)>>
		GuiWindow_s* addGuiWindow_void(const char* name, std::function<void(const char*, bool*, GuiWindow_s*)> f, CloseCallback callback = nullptr) {

			constantWindows.push_back(new GuiWindow_s(name, f, callback));
			return constantWindows[constantWindows.size() - 1];
		}

		template <typename F, typename = CloseCallback, typename... Args>
		GuiWindow_s* addGuiWindow_args(const char* name, F&& f, CloseCallback callback,  Args&&... args) {

			constantWindows.push_back(createGuiWindow_args(name, f, callback, std::forward<Args>(args)...));
			return constantWindows[constantWindows.size() - 1];
		}

		void removeWindow(GuiWindow_s* window);

		void setDockingMode(bool mode);


		bool hasWindow(const char* name) const;

		void createStyleWindow();

		inline GuiWindow_s* getFocusedWindow() const { return focusedWindow; }

		void setFocusedWindow(GuiWindow_s* window);

	private:
		void renderDocked();
		void renderUndocked();


	};


}