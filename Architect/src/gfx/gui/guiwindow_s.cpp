#include "guiwindow_s.h"


namespace archt {




	GuiWindow_s::GuiWindow_s(CloseCallback callback) : callback(callback) {}

	GuiWindow_s::GuiWindow_s(std::function<void(bool*, GuiWindow_s*)> func, CloseCallback callback)
		: func(func), callback(callback) {
	
	}

	GuiWindow_s::~GuiWindow_s() {
		if (callback)
			callback();
	}

	GuiWindow_s::operator bool() {
		return open;
	}

	void GuiWindow_s::operator()() {
		func(&open, this);
	}


}