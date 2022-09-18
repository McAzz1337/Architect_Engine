#include "waveloader.h"


/*
#define WAVELOADER_RIFF "RIFF"
#define WAVELOADER_WAVE "WAVE"
#define WAVELOADER_FMT "fmt "
#define WAVELOADER_DATA "data"

namespace archt {





	uint16_t* loadWaveFile(const std::string& path, WaveHeader& header) {


		FILE* file = fopen(path.c_str(), "r");
		if (!file) {
			printf("Failed to open Wave file: %s\n", path.c_str());
			__debugbreak();
		}



		fread(header.riff, sizeof(int8_t), 4, file);
		if (!strcmp((const char*) header.riff, WAVELOADER_RIFF)) {
			printf("Error at riff in file: %s\nriff = %s\n", path.c_str(), header.riff);
			__debugbreak();
		}

		fread(&header.size, sizeof(DWORD), 1, file);

		fread(header.wave, sizeof(int8_t), 4, file);
		if (!strcmp((const char*) header.wave, WAVELOADER_WAVE)) {
			printf("Error at wave in file: %s\nwave = %s\n", path.c_str(), header.wave);
			__debugbreak();
		}

		fread(header.fmt, sizeof(int8_t), 4, file);
		if (!strcmp((const char*) header.fmt, WAVELOADER_FMT)) {
			printf("Error at fmt in file: %s\nfmt = %s\n", path.c_str(), header.fmt);
			__debugbreak();
		}

		fread(&header.chunkSize, sizeof(DWORD), 1, file);
		
		fread(&header.format, sizeof(int16_t), 1, file);
		fread(&header.channels, sizeof(int16_t), 1, file);
		
		fread(&header.samplerate, sizeof(DWORD), 1, file);
		fread(&header.avgBytesPerSecond, sizeof(DWORD), 1, file);

		fread(&header.bytesPerSample, sizeof(int16_t), 1, file);
		fread(&header.bitsPerSample, sizeof(int16_t), 1, file);

		fread(header.data, sizeof(int8_t), 4, file);
		if (!strcmp((const char*) header.fmt, WAVELOADER_DATA)) {
			printf("Error at data in file: %s\data = %s\n", path.c_str(), header.data);
			__debugbreak();
		}

		fread(&header.dataSize, sizeof(DWORD), 1, file);

		uint16_t* data = new uint16_t[header.dataSize];

		fread(data, 2, header.dataSize, file);

#if 0
		printf("%s wave header:\n", path.c_str());
		printf("riff: %s\n", header.riff);
		printf("fileSize: %i\n", header.size);
		printf("wave: %s\n", header.wave);
		printf("fmt: %s\n", header.fmt);
		printf("chunkeSize: %i\n", header.chunkSize);
		printf("format: %i\n", header.format);
		printf("channels: %i\n", header.channels);
		printf("sampleRate: %i\n", header.samplerate);
		printf("avgBytesPerSecons: %i\n", header.avgBytesPerSecond);
		printf("bytesPerSample: %i\n", header.bytesPerSample);
		printf("bitsPerSample: %i\n", header.bitsPerSample);
		printf("data: %s\n", header.data);
		printf("dataSize: %i\n", header.dataSize);
		printf("-----------------------------------\n");
#endif


		return data;
	}

}*/