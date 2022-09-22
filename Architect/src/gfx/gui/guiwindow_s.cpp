#include "guiwindow_s.h"


namespace archt {




	GuiWindow_s::GuiWindow_s(CloseCallback callback) : callback(callback) {}

	GuiWindow_s::GuiWindow_s(const char* name, std::function<void(const char*, bool*, GuiWindow_s*)> func, CloseCallback callback)
		: name(name), func(func), callback(callback) {
	
	}

	GuiWindow_s::~GuiWindow_s() {
		if (callback)
			callback();
	}

	GuiWindow_s::operator bool() {
		return open;
	}

	void GuiWindow_s::operator()() {
		func(name, &open, this);
	}


}