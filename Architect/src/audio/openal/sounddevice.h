#pragma once
#include <AL/alc.h>



namespace archt {

	class SoundDevice {
	
	public:
		static SoundDevice* instance;

	private:


		ALCdevice* device = nullptr;
		ALCcontext* context = nullptr;

	public:
		SoundDevice();
		~SoundDevice();


		static void init();
		static void terminate();
	
	
	};

}
