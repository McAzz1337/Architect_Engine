#pragma once

#include <typeindex>

#define MAX_KEYS 400
#define KEY_RELEASED 0
#define KEY_PRESS 1
#define KEY_HOLD 2


namespace archt {


	class Input {

		static uint32_t keys[MAX_KEYS];

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
	};



}