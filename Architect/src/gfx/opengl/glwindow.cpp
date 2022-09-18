#include "glwindow.h"

#include "glrenderapi.h"
#include "../../input/input.h"

#include <iostream>
#include <functional>
#include <memory>
#include <sstream>
#include <thread>
#include <chrono>

#include "../../thread/mythread.h"

#include <Windows.h>

namespace archt {

	void keyCallback(GLFWwindow* window, int key, int scan, int action, int mod) {

		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, 1);
		}
		if (key == GLFW_KEY_P) {
			static bool press = false;
			if ((action == GLFW_PRESS || action == GLFW_REPEAT) && !press) {
				((GLWindow*) glfwGetWindowUserPointer(window))->toggleFullscreen();
				press = true;
			}
			else if (action == GLFW_RELEASE && press) {
				press = false;
			}

		}
		else {
			switch (action) {
				case GLFW_PRESS:
					Input::press(key);
					break;
				case GLFW_REPEAT:
					Input::hold(key);
					break;
				case GLFW_RELEASE:
					Input::release(key);
					break;
				default:
					printf("Unimplemented key action : %i\n", action);
					break;
			}
		}
	}

	void resizeCallback(GLFWwindow* window, int width, int height) {
		GLWindow* w = (GLWindow*) glfwGetWindowUserPointer(window);
		w->setSize(width, height);
	}


	void errorCallback(int error_code, const char* description) {

		std::stringstream ss;
		ss << "Error code [" << error_code << "][0x" << std::hex << error_code << "]\n";
		ss << "Description : \n" << description;

		__debugbreak();
	}



	GLWindow::GLWindow() {
	
	}

	

	GLWindow::GLWindow(const char* title, int x, int y, int w, int h)
		: title(title), x(x), y(y), w(w), h(h) {

		/* // doesnt work as intended
		int aElements[2] = { COLOR_WINDOWFRAME};
		DWORD aNewColors[2];
		aNewColors[0] = RGB(0x30, 0x30, 0x30);  // light gray 
		SetSysColors(2, aElements, aNewColors);
		*/

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		
		float xScale, yScale;
		glfwGetMonitorContentScale(monitor, &xScale, &yScale);
		this->x *= xScale;
		this->y *= yScale;

		this->w *= xScale;
		this->h *= yScale;


		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		refreshRate = mode->refreshRate;


		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		window = glfwCreateWindow(w, h, title, nullptr, nullptr);
		glfwMakeContextCurrent(window);


		glfwSetWindowPos(window, x, y);

		glfwSwapInterval(0);

		//toggleFullscreen();

		glfwSetKeyCallback(window, keyCallback);
		glfwSetWindowSizeCallback(window, resizeCallback);
		glfwSetWindowUserPointer(window, this);
		glfwSetErrorCallback(errorCallback);

	}



	GLWindow::~GLWindow() {
		if (window) {
			glfwDestroyWindow(window);
		}
	}

	void GLWindow::makeContextCurrent() const {
		glfwMakeContextCurrent(window);
	}


	void GLWindow::pollEvents() {
		glfwPollEvents();
	}

	bool GLWindow::shouldClose() {
		return glfwWindowShouldClose(window) == 1;
	}

	void GLWindow::swapBuffer() {
		if (dispatchResizeEvent) {
			//invokeResizeCallbacks();
			dispatchResizeEvent = false;
		}
		glfwSwapBuffers(window);
	}


	void GLWindow::setTitle(const char* name) {
		glfwSetWindowTitle(window, name);
	}

	void GLWindow::setSize(int width, int height) {
		if (fullScreen) {
			return;
		}

		w = width;
		h = height;

		glViewport(0, 0, w, h);
		dispatchResizeEvent = true;
	}

	void GLWindow::toggleFullscreen() const {
		if (fullScreen) {
			fullScreen = false;
			glfwWindowHint(GLFW_DECORATED, GL_TRUE);
			glfwSetWindowMonitor(window, nullptr, x, y, w, h, refreshRate);
			glfwSetWindowSize(window, w, h);
			centerOnScreen();
		}
		else {
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);

			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			glfwWindowHint(GLFW_DECORATED, GL_FALSE);
			fullScreen = true;
			glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, refreshRate);
		}
	}



	void GLWindow::centerOnScreen() const {

		RECT screen;
		HWND screenWindow = GetDesktopWindow();
		GetWindowRect(screenWindow, &screen);
		int x = (screen.right - w) / 2;
		int y = (screen.bottom - h) / 2;

		glfwSetWindowPos(window, x, y);
	}

	void GLWindow::hide() const {
		glfwHideWindow(window);
	}

	void GLWindow::show() const {
		glfwShowWindow(window);
	}

	void GLWindow::addViewportCallback(ViewportCallback callback) {
		viewportCallbacks.push_back(callback);
	}

	void GLWindow::invokeResizeCallbacks() {
	
		printf("resize\n");
		for (int i = 0; i < viewportCallbacks.size(); i++) {
			viewportCallbacks[i](w, h);
		}
	}


}