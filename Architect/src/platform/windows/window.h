#pragma once

#include <Windows.h>

namespace archt {

	class Window {

		HINSTANCE instance;
		HWND window;
		const wchar_t* windowName = L"Architect Splash";

	public:
		Window();
		Window(const Window& window) = delete;
		~Window();

		Window operator=(const Window& other) = delete;




	};


}