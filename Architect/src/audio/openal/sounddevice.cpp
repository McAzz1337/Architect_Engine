#include "sounddevice.h"
#include <iostream>

namespace archt {

	SoundDevice* SoundDevice::instance = nullptr;


	SoundDevice::SoundDevice() {

		device = alcOpenDevice(nullptr);
		if (!device) {
			printf("Failed to open audio device\n");
			__debugbreak();
		}

		context = alcCreateContext(device, nullptr);
		if (!context) {
			printf("Failed to create audio context\n");
			__debugbreak();
		}

		if (!alcMakeContextCurrent(context)) {
			printf("Failed to make context current\n");
			__debugbreak();
		}


	}

	SoundDevice::~SoundDevice() {
		if (device)
			alcCloseDevice(device);
	}

	void SoundDevice::init() {
		if (instance)
			return;

		instance = new SoundDevice();
	}

	void SoundDevice::terminate() {
		delete instance;
		instance = nullptr;
	}
}