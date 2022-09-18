#pragma once


#include "../audio/openal/audiobuffer.h"
#include "../audio/openal/audiosource.h"

#include "../gfx/opengl/buffers.h"
#include "../gfx/opengl/gltexture.h"
#include "../gfx/opengl/glshader.h"

#include "../ptr.h"

#include "../entity/component/material/uniform.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <string>

namespace archt {

	struct Transform_s {

		glm::mat4 transform = glm::mat4(1.0f);

		Transform_s();
		Transform_s(glm::mat4 transform);
		Transform_s(const Transform_s& other);
		//Transform_s(Transform_s&& other) noexcept;
		~Transform_s();

		void translate(const glm::vec3& t);
		void rotate(float angle, const glm::vec3& axis);
		void scale(const glm::vec3& s);

		inline Transform_s operator*(const Transform_s& other) {
			return Transform_s(transform * other.transform);
		}

		inline operator glm::mat4() {
			return transform;
		}

		inline operator glm::mat4& () {
			return transform;
		}

	};

	struct Mesh_s {

		VBO* vbo = nullptr;
		IBO* ibo = nullptr;

		Mesh_s();
		Mesh_s(const Mesh_s& other);
		//Mesh_s(Mesh_s&& other) noexcept;
		Mesh_s(Vertex* verteces, uint32_t vSize, uint32_t* indeces, uint32_t iSize);
		~Mesh_s();
	};



	struct Material_s {

		ptr<GLTexture>tex;
		Uniform* uniforms = nullptr;
		int uniformCount = 0;
		ptr<GLShader> shader;

		Material_s();
		Material_s(const Material_s& other);
		//Material_s(Material_s&& other) noexcept;
		Material_s(const std::string& texturePath, const std::string& uniformsPath, const std::string& shaderPath);
		~Material_s();

	};

	struct Tag {

		std::string tag;

		Tag();
		Tag(const Tag& other);
		//Tag(Tag&& other) noexcept;
		Tag(const std::string& tag);
		~Tag();

	};


	struct AudioComponent {

		AudioSource source;
		std::vector<ptr<AudioBuffer>> buffers;

		AudioComponent();
		AudioComponent(const std::string& path);
		AudioComponent(const std::vector<std::string>& paths);

		~AudioComponent();


	};

	

}