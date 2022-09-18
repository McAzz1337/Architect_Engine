#include "window.h"

#include "../../stb/stb/stb_image.h"
#include "../../gfx/opengl/gltexture.h"

LRESULT CALLBACK windowProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {

	return DefWindowProc(window, msg, wParam, lParam);
}

namespace archt {

	Window::Window() : instance(GetModuleHandle(nullptr)) {
	
		WNDCLASS wndClass = {};
		wndClass.lpszClassName = windowName;
		wndClass.hInstance = instance;
		wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.lpfnWndProc = windowProc;

		RegisterClass(&wndClass);

		DWORD style = WS_POPUP;

		int w = 0;
		int h = 0;
		int comp;
		unsigned char* data = nullptr;
		{
			unsigned char* _data = stbi_load("src/assets/img/archt.png", &w, &h, &comp, STBI_rgb_alpha);
			data = new unsigned char[w * h * comp];
			GLTexture::flipImage(_data, w, h, comp, data);
			stbi_image_free(_data);
		}

		RECT screen;
		HWND screenWindow = GetDesktopWindow();
		GetWindowRect(screenWindow, &screen);


		RECT rect;
		rect.left = (screen.right - w) / 2;
		rect.top = (screen.bottom - h) / 2;
		rect.right = rect.left + w / 2 + 400;
		rect.bottom = rect.top + h / 2 - 20;

		AdjustWindowRect(&rect, style, false);

		window = CreateWindowEx(
			0,
			windowName,
			L"Architecht Splash",
			style,
			rect.left,
			rect.top,
			rect.right,
			rect.bottom,
			NULL,
			NULL,
			instance,
			NULL
		);
		ShowWindow(window, SW_SHOW);

		COLORREF bkcolor = RGB(0, 0, 0);
		PAINTSTRUCT ps;
		RECT rc;
		HDC hdc = BeginPaint(window, &ps);
		GetClientRect(window, &rc);
		SetDCBrushColor(hdc, bkcolor);
		FillRect(hdc, &rc, (HBRUSH) GetStockObject(DC_BRUSH));

		//or use ps.rcPaint to repaint only the section which requires update
		//FillRect(hdc, &ps.rcPaint, (HBRUSH)GetStockObject(DC_BRUSH));

		EndPaint(window, &ps);
		
		


		BITMAPINFO bitmapInfo;
		bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
		bitmapInfo.bmiHeader.biWidth = w;
		bitmapInfo.bmiHeader.biHeight = h;
		bitmapInfo.bmiHeader.biPlanes = 1;
		bitmapInfo.bmiHeader.biBitCount = 32;
		bitmapInfo.bmiHeader.biCompression = BI_RGB;

		HDC dc = GetDC(window);
		StretchDIBits(
			dc,
			0,
			0,
			w,
			h,
			0,
			0,
			w,
			h,
			data, 
			&bitmapInfo, 
			DIB_RGB_COLORS, 
			SRCCOPY
		);
		InvalidateRect(window, NULL, FALSE);
		UpdateWindow(window);

		delete data;
	}

	Window::~Window() {
		DestroyWindow(window);
		UnregisterClass(windowName, instance);
	}


}
