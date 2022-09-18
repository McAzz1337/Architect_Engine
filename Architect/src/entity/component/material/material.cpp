#include "material.h"


namespace archt {
	


	Material::Material() {}

	Material::Material(const std::string& shaderPath, const std::string& texturePath) : shader(shaderPath) {
		tex = new GLTexture(texturePath);
	}

	Material::~Material() {}

}