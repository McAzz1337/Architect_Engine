#include "input.h"



namespace archt {


	uint32_t Input::keys[MAX_KEYS];
	uint32_t Input::buttons[MAX_BUTTONS];


	void Input::init() {
		for (int i = 0; i < MAX_KEYS; i++)
			keys[i] = KEY_RELEASED;
	}
	
	void Input::terminate() {
	}
	
	void Input::release(int key) {
		keys[key] = KEY_RELEASED;
	}
	
	void Input::press(int key) {
		if (keys[key] == KEY_PRESS || keys[key] == KEY_HOLD)
			return;

		keys[key] = KEY_PRESS;
	}

	void Input::hold(int key) {
		keys[key] = KEY_HOLD;
	}

	bool Input::isRelease(int key) {
		return keys[key] == KEY_RELEASED;
	}
	
	bool Input::isPress(int key) {
		return keys[key] == KEY_PRESS;
	}
	
	bool Input::isHeld(int key) {
		return keys[key] == KEY_HOLD;
	}

	void Input::pressMouse(int button) {
	
		buttons[button] = KEY_PRESS;
	}

	void Input::holdMouse(int button) {
		
		buttons[button] = KEY_HOLD;
	}

	void Input::releaseMouse(int button) {
	
		buttons[button] = KEY_RELEASED;
	}

	bool Input::isMouseRelease(int button) {
		
		return buttons[button] == KEY_RELEASED;
	}

	bool Input::isMousePress(int button) {

		return buttons[button] == KEY_PRESS;
	}

	bool Input::isMouseHeld(int button) {

		return buttons[button] == KEY_HOLD;
	}

}