#pragma once


#include "../audio/openal/audiobuffer.h"
#include "../audio/openal/audiosource.h"

#include "../gfx/opengl/buffers.h"
#include "../gfx/opengl/gltexture.h"
#include "../gfx/opengl/glshader.h"

#include "../math/ray.h"

#include "../ptr.h"

#include "../entity/component/material/uniform.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <string>

namespace archt {

	struct Transform_s {

		glm::mat4 transform = glm::mat4(1.0f);

		Transform_s();
		Transform_s(float value);
		Transform_s(const Transform_s& other);
		//Transform_s(Transform_s&& other) noexcept;
		~Transform_s();

		void translate(const glm::vec3& t);
		void rotate(float angle, const glm::vec3& axis);
		void scale(const glm::vec3& s);

		glm::vec3 getPosition() const;

		Transform_s inverse() const;


		glm::vec4 operator[](int i) const;


		inline Transform_s operator*(const Transform_s& other) const {
			
			Transform_s t;
			t.transform = transform * other.transform;

			return t;
		}

		//inline glm::mat4 operator*(const glm::mat4& mat) {
		//	return transform * mat;
		//}

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

	struct Camera_s {

		Transform_s view;
		Transform_s projection;

		Camera_s();
		~Camera_s();

		static Camera_s makePerspective(float fov, float aspect, float nearClip, float farClip);
		static Camera_s makeOrthographic(float left, float right, float top, float bottom, float nearClip, float farClip);

		void translate(const glm::vec3& t);
		void rotate(float angle, const glm::vec3& axis);
		void scale(const glm::vec3& s);

		void castRay(Ray& ray) const;
		
		inline glm::vec3 getPosition() const { return (glm::vec3) view[3]; }

		inline const Transform_s& getView() const { return view; }
		inline const Transform_s& getProjection() const { return projection; }
		inline Transform_s getViewProjection() const { return projection * view; }

	};

}