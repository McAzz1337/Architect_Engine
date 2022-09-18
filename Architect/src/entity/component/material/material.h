#pragma once

#include "../../component.h"
#include "../../../gfx/opengl/glshader.h"
#include "../../../gfx/opengl/gltexture.h"

namespace archt {


	class Material : public Component {

		GLShader shader;
		GLTexture* tex = nullptr;


	public:
		Material();
		Material(const std::string& shaderPath, const std::string& texturePath);
		~Material();



		inline GLShader& getShader() { return shader; }
		inline GLTexture* getTexture() const { return tex; }




	};
}