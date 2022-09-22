#include "guiwindow.h"


namespace archt {




	GuiWindow::GuiWindow(CloseCallback callback) : callback(callback) {}

	GuiWindow::GuiWindow(const char* name, std::function<void(const char*, bool*, GuiWindow*)> func, CloseCallback callback)
		: name(name), func(func), callback(callback) {
	
	}

	GuiWindow::~GuiWindow() {
		if (callback)
			callback();
	}

	GuiWindow::operator bool() {
		return open;
	}

	void GuiWindow::operator()() {
		func(name, &open, this);
	}


}