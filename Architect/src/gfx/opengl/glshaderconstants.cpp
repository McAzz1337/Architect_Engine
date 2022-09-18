#include "glshaderconstants.h"
#include <iostream>

namespace archt {

#define MODIFY_SHADER_CONSTANT(index, name, value)	index = src.find(name); \
													if (index != std::string::npos) { \
													index += name.length() + 1; \
													int end = src.find("\n", index); \
													src.replace(src.begin() + index, src.begin() + end, std::to_string(value).c_str()); \
													}\



	uint32_t GLShaderConstants::v_maxTextures = 0;
	uint32_t GLShaderConstants::v_maxMatrices = 0;

	const std::string GLShaderConstants::HASH_DEFINE = "#define ";


	const std::string GLShaderConstants::MAX_TEXTURES = HASH_DEFINE + "MAX_TEXTURES";
	const std::string GLShaderConstants::MAX_MATRICES = HASH_DEFINE + "MAX_MATRICES";


	
	
	
	
	void GLShaderConstants::setConstant(const std::string& name, void* value) {
		
		if (name == MAX_TEXTURES) {
			v_maxTextures = *((uint32_t*) value);
		}
		else if (name == MAX_MATRICES) {
			v_maxMatrices = *((uint32_t*) value);
		}
	}

	void GLShaderConstants::modifySahderSource(std::string& src) {
		
		int index = std::string::npos;

		MODIFY_SHADER_CONSTANT(index, MAX_TEXTURES, v_maxTextures);
		MODIFY_SHADER_CONSTANT(index, MAX_MATRICES, v_maxMatrices);
	
	}

}