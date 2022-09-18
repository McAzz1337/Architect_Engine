#pragma once
#include "glinclude.h"

#include <sstream>


#ifdef ARCHT_DEBUG

#define GL_ASSERT(x) if (!x) { __debugbreak(); }

#define CALL(x) archt::clearError(); x; GL_ASSERT(archt::checkError(#x, __FILE__, __LINE__))
#define CHECK_ERROR(x) checkError(#x, __FILE__, __LINE__)

#else

#define CALL(x) x;
#define CHECK_ERROR(x)

#endif

namespace archt {

	static void clearError() {
		while (glGetError() != GL_NO_ERROR) {}
	}


	static bool checkError(const char* func, const char* file, int line) {

		bool noError = true;

		GLenum error;
		while ((error = glGetError()) != GL_NO_ERROR) {
			std::stringstream ss;
			ss << "0x" << std::hex << error;
			printf("GL_ERROR [%s]\n%s\n%s\nLine : %i\nERROR : %s\n\n", ss.str().c_str(), func, file, line, glewGetErrorString(error));
			noError = false;
		}

		return noError;


	}


}