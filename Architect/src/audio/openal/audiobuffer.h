#pragma once
#include <string>
#include <Windows.h>
#include <typeindex>

namespace archt {

	struct WaveHeader {

		std::string riff;
		DWORD size;
		std::string wave;
		std::string fmt;
		DWORD chunkSize;
		int16_t format;
		int16_t channels;
		DWORD samplerate;
		DWORD avgBytesPerSecond;
		int16_t bytesPerSample;
		int16_t bitsPerSample;
		std::string data;
		DWORD dataSize;

		WaveHeader();
	};



	class AudioBuffer {

		std::string file;
		uint32_t id = 0;

	public:
		AudioBuffer();
		AudioBuffer(const std::string& path);
		~AudioBuffer();



		inline uint32_t getId() const { return id; }
		inline std::string getFilePath() const { return file; }

	private:
		static uint16_t* loadWaveFile(const std::string& path, WaveHeader& header);

	};


}