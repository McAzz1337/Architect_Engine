#pragma once

#include <AL/al.h>
#include <sstream>
#include <iostream>

#ifdef ARCHT_DEBUG

#define AL_ASSERT(x) if (x) {__debugbreak();}
#define AL_CALL(x) archt::clearAlError(); x; AL_ASSERT(archt::checkAlError(#x, __FILE__, __LINE__))

#else

#define AL_CALL(x) x;

#endif

namespace archt {

	static void clearAlError() {
		while (alGetError() != AL_NO_ERROR) {
		}
	}

	static bool checkAlError(const char* func, const char* file, int line) {

		ALenum error;
		bool hasError = false;

		while ((error = alGetError()) != AL_NO_ERROR) {

			std::stringstream ss;
			ss << std::hex << error;
			printf("OpenAl error [%i]\n%s\nFile: %s\nLine: %i\n%s\n", error, func, file, line, alGetString(error));
			hasError = true;
		}

		return hasError;
	}




}