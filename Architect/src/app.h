#pragma once

#include <string>
#include "gfx/opengl/glwindow.h"

namespace archt {


	class App {

		GLFWwindow* window = nullptr;


	public:
		App();
		App(const char* title, int x, int y, int w, int h);
		App(const char* title, int x, int y, int w, int h, const std::string& iconPath);
		virtual ~App();

		void init();
		void terminate();

		virtual void update() = 0;
		virtual void render() const = 0;
		


	};


}