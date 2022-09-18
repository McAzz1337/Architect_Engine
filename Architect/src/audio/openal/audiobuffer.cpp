#include "audiobuffer.h"
#include <AL/al.h>


#include <iostream>



#include "aldebug.h"




#define WAVELOADER_RIFF "RIFF"
#define WAVELOADER_WAVE "WAVE"
#define WAVELOADER_FMT "fmt "
#define WAVELOADER_DATA "data"

namespace archt {

	WaveHeader::WaveHeader() {
		riff = "1234";
		size = 0;
		wave = "1234";
		fmt = "1234";
		chunkSize = 0;
		format = 0;
		channels = 0;
		samplerate = 0;
		avgBytesPerSecond = 0;
		bytesPerSample = 0;
		bitsPerSample = 0;
		data = "1234";
		dataSize = 0;
	}

	
	AudioBuffer::AudioBuffer() {

		
	}

	AudioBuffer::AudioBuffer(const std::string& path) :file(path) {

		WaveHeader header;
		uint16_t* buffer = loadWaveFile(path, header);

		AL_CALL(alGenBuffers(1, &id));
		AL_CALL(alBufferData(id, AL_FORMAT_STEREO16, buffer, header.dataSize, header.samplerate));


		delete[] buffer;
	}

	AudioBuffer::~AudioBuffer() {
		if (id)
			alDeleteBuffers(1 , &id);
	}

	uint16_t* AudioBuffer::loadWaveFile(const std::string& path, WaveHeader& header) {


		FILE* file = fopen(path.c_str(), "rb");
		if (!file) {
			printf("Failed to open Wave file: %s\n", path.c_str());
			__debugbreak();
		}


		fread(header.riff.data(), sizeof(uint8_t), 4, file);
		if (header.riff != WAVELOADER_RIFF) {
			printf("Error at riff in file: %s\nriff = %s\n", path.c_str(), header.riff.c_str());
			__debugbreak();
		}

		fread(&header.size, sizeof(DWORD), 1, file);

		fread(header.wave.data(), sizeof(uint8_t), 4, file);
		if (header.wave != WAVELOADER_WAVE) {
			printf("Error at wave in file: %s\nwave = %s\n", path.c_str(), header.wave.c_str());
			__debugbreak();
		}

		fread(header.fmt.data(), sizeof(uint8_t), 4, file);
		if (header.fmt != WAVELOADER_FMT) {
			printf("Error at fmt in file: %s\nfmt = %s\n", path.c_str(), header.fmt.c_str());
			__debugbreak();
		}

		fread(&header.chunkSize, sizeof(DWORD), 1, file);

		fread(&header.format, sizeof(int16_t), 1, file);
		fread(&header.channels, sizeof(int16_t), 1, file);

		fread(&header.samplerate, sizeof(DWORD), 1, file);
		fread(&header.avgBytesPerSecond, sizeof(DWORD), 1, file);

		fread(&header.bytesPerSample, sizeof(int16_t), 1, file);
		fread(&header.bitsPerSample, sizeof(int16_t), 1, file);

		fread(header.data.data(), sizeof(uint8_t), 4, file);
		if (header.data != WAVELOADER_DATA) {
			printf("Error at data in file: %s\data = %s\n", path.c_str(), header.data.c_str());
			__debugbreak();
		}

		fread(&header.dataSize, sizeof(DWORD), 1, file);

		uint16_t* data = new uint16_t[header.dataSize];

		fread(data, 2, header.dataSize, file);

#if 0
		printf("%s wave header:\n", path.c_str());
		printf("riff: %s\n", header.riff.c_str());
		printf("fileSize: %i\n", header.size);
		printf("wave: %s\n", header.wave.c_str());
		printf("fmt: %s\n", header.fmt.c_str());
		printf("chunkeSize: %i\n", header.chunkSize);
		printf("format: %i\n", header.format);
		printf("channels: %i\n", header.channels);
		printf("sampleRate: %i\n", header.samplerate);
		printf("avgBytesPerSecons: %i\n", header.avgBytesPerSecond);
		printf("bytesPerSample: %i\n", header.bytesPerSample);
		printf("bitsPerSample: %i\n", header.bitsPerSample);
		printf("data: %s\n", header.data.c_str());
		printf("dataSize: %i\n", header.dataSize);
		printf("-----------------------------------\n");
#endif

		return data;
	}


}