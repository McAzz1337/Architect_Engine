#include "gui_s.h"

#include "../../filesystem/filemanager.h"

#include "../../fileio.h"

#include <sstream>


namespace archt {

	std::string symbols[ImGuiCol_COUNT] = {
		 "ImGuiCol_Text",
		 "ImGuiCol_TextDisabled",
		 "ImGuiCol_WindowBg",
		 "ImGuiCol_ChildBg",
		 "ImGuiCol_PopupBg",
		 "ImGuiCol_Border",
		 "ImGuiCol_BorderShadow",
		 "ImGuiCol_FrameBg",
		 "ImGuiCol_FrameBgHovered",
		 "ImGuiCol_FrameBgActive",
		 "ImGuiCol_TitleBg",
		 "ImGuiCol_TitleBgActive",
		 "ImGuiCol_TitleBgCollapsed",
		 "ImGuiCol_MenuBarBg",
		 "ImGuiCol_ScrollbarBg",
		 "ImGuiCol_ScrollbarGrab",
		 "ImGuiCol_ScrollbarGrabHovered",
		 "ImGuiCol_ScrollbarGrabActive",
		 "ImGuiCol_CheckMark",
		 "ImGuiCol_SliderGrab",
		 "ImGuiCol_SliderGrabActive",
		 "ImGuiCol_Button",
		 "ImGuiCol_ButtonHovered",
		 "ImGuiCol_ButtonActive",
		 "ImGuiCol_Header",
		 "ImGuiCol_HeaderHovered",
		 "ImGuiCol_HeaderActive",
		 "ImGuiCol_Separator",
		 "ImGuiCol_SeparatorHovered",
		 "ImGuiCol_SeparatorActive",
		 "ImGuiCol_ResizeGrip",
		 "ImGuiCol_ResizeGripHovered",
		 "ImGuiCol_ResizeGripActive",
		 "ImGuiCol_Tab",
		 "ImGuiCol_TabHovered",
		 "ImGuiCol_TabActive",
		 "ImGuiCol_TabUnfocused",
		 "ImGuiCol_TabUnfocusedActive",
		 "ImGuiCol_DockingPreview",
		 "ImGuiCol_DockingEmptyBg",
		 "ImGuiCol_PlotLines",
		 "ImGuiCol_PlotLinesHovered",
		 "ImGuiCol_PlotHistogram",
		 "ImGuiCol_PlotHistogramHovered",
		 "ImGuiCol_TableHeaderBg",
		 "ImGuiCol_TableBorderStrong",
		 "ImGuiCol_TableBorderLight",
		 "ImGuiCol_TableRowBg",
		 "ImGuiCol_TableRowBgAlt",
		 "ImGuiCol_TextSelectedBg",
		 "ImGuiCol_DragDropTarget",
		 "ImGuiCol_NavHighlight",
		 "ImGuiCol_NavWindowingHighlight",
		 "ImGuiCol_NavWindowingDimBg",
		 "ImGuiCol_ModalWindowDimBg"
	};




	Gui_s* Gui_s::instance = nullptr;

	const std::string Gui_s::styleFile = "src/assets/styles/style.txt";


	Gui_s::Gui_s(glm::ivec2 windowSize) {
		ImGuiIO& io = ImGui::GetIO();
		(void) io;
		io.DisplaySize.x = windowSize.x;
		io.DisplaySize.y = windowSize.y;



		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			ImGuiStyle& style = ImGui::GetStyle();
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		//createStyleWindow();
	}

	Gui_s::~Gui_s() {
		for (GuiWindow_s* w : constantWindows)
			delete w;

		for (GuiWindow_s* w : perFrameWindows)
			delete w;

		if (styleWindow)
			delete styleWindow;
	}

	void Gui_s::init(GLWindow* window) {

		if (instance)
			return;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		(void) io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui_ImplOpenGL3_Init("#version 450 core");
		ImGui_ImplGlfw_InitForOpenGL(window->getHandle(), true);
		ImGui::StyleColorsDark();
		glm::ivec2 windowSize = window->getSize();
		instance = new Gui_s(windowSize);

		setStyle();
	}

	Gui_s* Gui_s::getInstance() {
		return instance;
	}

	void Gui_s::setStyle() {


		ImGuiStyle& style = ImGui::GetStyle();

		std::vector<std::string> values;
		readFileSplit(styleFile, values);

		for (int i = 0; i < values.size(); i++) {

			std::string tokens[2];
			split(values[i], '=', tokens, 2);

			int index = std::stoi(tokens[0]);
			ImVec4 components;
			std::string comps[4];
			split(tokens[1], ',', comps, 4);

			components.x = std::stof(comps[0]);
			components.y = std::stof(comps[1]);
			components.z = std::stof(comps[2]);
			components.w = std::stof(comps[3]);

			style.Colors[i] = components;
			instance->defaultColors[i] = components;
		}




	}

	void Gui_s::terminate() {

		if (instance) {
			delete instance;
			instance = nullptr;
		}

		ImGui_ImplGlfw_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();
	}

	void Gui_s::render() {

		if (docked)
			renderDocked();
		else
			renderUndocked();
	}


	void Gui_s::removeWindow(GuiWindow_s* window) {
		for (int i = 0; i < constantWindows.size(); i++) {

			if (constantWindows[i] == window) {

				if (constantWindows[i] == focusedWindow) {
					focusedWindow = nullptr;
				}

				constantWindows.erase(constantWindows.begin() + i);
				constantWindows.shrink_to_fit();
				break;
			}
		}
	}

	void Gui_s::setDockingMode(bool mode) {
		docked = mode;
	}

	bool Gui_s::hasWindow(const char* name) const {
		return false;
	}

	void Gui_s::createStyleWindow() {

		

		auto lambda = [this](bool* open, GuiWindow_s* handle) {



			ImGuiStyle& style = ImGui::GetStyle();
			ImGui::Begin("Style", open);

			if (!(*open)) {
				styleWindow = nullptr;
			}

			for (int i = 0; i < ImGuiCol_COUNT; i++) {
				//ImGui::SliderFloat4(symbols[i].c_str(), &style.Colors[i].x, 0.0f, 1.0f);

				ImVec2 buttonSize = ImGui::CalcTextSize(symbols[i].c_str());
				buttonSize.x += 10;
				buttonSize.y += 10;
				if (ImGui::Button(symbols[i].c_str(), buttonSize)) {
					

					auto colorPicker = [this](bool* open, GuiWindow_s* handle, int index) {

						if (ImGui::Begin(symbols[index].c_str(), open)) {

							ImGuiStyle& style = ImGui::GetStyle();
							ImGui::ColorPicker4(symbols[index].c_str(), &style.Colors[index].x);

						}

						ImGui::End();
					};
					addGuiWindow_args(colorPicker, nullptr, i);
				}
				ImGui::SameLine();
				ImGui::ColorButton(symbols[i].c_str(), style.Colors[i], 0, buttonSize);
				ImGui::SameLine();
				std::string s = "Reset " + symbols[i];
				buttonSize = ImGui::CalcTextSize(s.c_str());
				buttonSize.y += 10;
				if (ImGui::Button(s.c_str(), buttonSize)) {
					style.Colors[i] = defaultColors[i];
				}
			}

			ImVec2 buttonSize = ImGui::CalcTextSize("Reset to default");
			buttonSize.x += 10;
			buttonSize.y += 10;
			if (ImGui::Button("Reset to default", buttonSize)) {
				setStyle();
			}

			buttonSize = ImGui::CalcTextSize("Save to file");
			buttonSize.x += 10;
			buttonSize.y += 10;
			if (ImGui::Button("Save to file", buttonSize)) {


				std::stringstream ss;
				for (int i = 0; i < ImGuiCol_COUNT; i++) {

					ImVec4 v = style.Colors[i];
					ss << i << "=" << v.x << "," << v.y << "," << v.z << "," << v.w << std::endl;
				}

				writeFile(styleFile, ss.str());
			}


			ImGui::End();
		};

		if (!styleWindow) {
			styleWindow = addGuiWindow_void(lambda);
		}
	}

	void Gui_s::setFocusedWindow(GuiWindow_s* window) {

		focusedWindow = window;
	}

	void Gui_s::renderDocked() {


		ImGuiIO& io = ImGui::GetIO();
		(void) io;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static bool* p_open = new bool(true);
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen) {
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else {
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", p_open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		else {
			//ShowDockingDisabledMessage();
		}

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("Options")) {
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				ImGui::MenuItem("Padding", NULL, &opt_padding);
				ImGui::Separator();

				if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
				if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
				ImGui::Separator();

				if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
					*p_open = false;
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Settings")) {
				if (ImGui::MenuItem("Style Window", "", nullptr)) {
					createStyleWindow();
				}
				if (ImGui::MenuItem("Filemanager", "", nullptr)) {
					Filemanager::getInstance().createGuiWindow();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();

			for (int i = 0; i < constantWindows.size(); i++) {
				if (constantWindows[i])
					(*constantWindows[i])();
			}

			for (int i = constantWindows.size() - 1; i > -1; i--) {
				if (!(*constantWindows[i])) {	// operator bool() : returns if the window is open
					delete constantWindows[i];
					constantWindows.erase(constantWindows.begin() + i);
				}
			}
			constantWindows.shrink_to_fit();

			//ImGui::ShowDemoWindow();


			for (GuiWindow_s* w : perFrameWindows) {
				(*w)();
			}
			perFrameWindows.erase(perFrameWindows.begin(), perFrameWindows.end());
			perFrameWindows.shrink_to_fit();

		}
		ImGui::End(); // ImGui::DockSpace();

		ImGui::Render();

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable
			&& io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {

			GLFWwindow* window = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(window);
		}
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Gui_s::renderUndocked() {

		ImGuiIO& io = ImGui::GetIO();
		(void) io;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		for (GuiWindow_s* w : constantWindows)
			(*w)();

		for (GuiWindow_s* w : perFrameWindows) {
			(*w)();
		}
		perFrameWindows.erase(perFrameWindows.begin(), perFrameWindows.end());
		perFrameWindows.shrink_to_fit();

		ImGui::Render();

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable
			&& io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {

			GLFWwindow* window = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(window);
		}
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

}