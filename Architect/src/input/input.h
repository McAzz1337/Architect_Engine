#pragma once

#include <typeindex>

#define MAX_BUTTONS 2
#define MAX_KEYS 400
#define KEY_RELEASED 0
#define KEY_PRESS 1
#define KEY_HOLD 2


namespace archt {


	class Input {

		static uint32_t keys[MAX_KEYS];
		static uint32_t buttons[MAX_BUTTONS];

	public:
		Input() = delete;

		static void init();
		static void terminate();

		static void release(int key);
		static void press(int key);
		static void hold(int key);

		static bool isRelease(int key);
		static bool isPress(int key);
		static bool isHeld(int key);


		static void pressMouse(int button);
		static void holdMouse(int button);
		static void releaseMouse(int button);

		static bool isMouseRelease(int button);
		static bool isMousePress(int button);
		static bool isMouseHeld(int button);
	};



}