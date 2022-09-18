#pragma once

#include <string>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <typeinfo>

namespace archt {



	struct Uniform {

		std::string name;
		int size;

		Uniform();
		Uniform(const std::string& name, int size);
		virtual ~Uniform();

	

	};

	struct UniformFloat1 : public Uniform {

		float uniform = 0.0f;

		UniformFloat1();
		UniformFloat1(const std::string& name, float uniform);
		~UniformFloat1();

	};

	struct UniformFloat2 : public Uniform {

		glm::vec2 uniform = glm::vec2(0.0f);

		UniformFloat2();
		UniformFloat2(const std::string& name, const glm::vec2& uniform);
		~UniformFloat2();

	};

	struct UniformFloat3 : public Uniform {

		glm::vec3 uniform = glm::vec3(0.0f);

		UniformFloat3();
		UniformFloat3(const std::string& name, const glm::vec3& uniform);
		~UniformFloat3();

	};

	struct UniformFloat4 : public Uniform {

		glm::vec4 uniform = glm::vec4(0.0f);

		UniformFloat4();
		UniformFloat4(const std::string& name, const glm::vec4& uniform);
		~UniformFloat4();

	};

	struct UniformMatrix4 : public Uniform {

		glm::mat4 uniform = glm::mat4(1.0f);

		UniformMatrix4();
		UniformMatrix4(const std::string& name, const glm::mat4& uniform);
		~UniformMatrix4();

	};


	template<class T>
	T* uniform_cast(Uniform* uniform) {
		return dynamic_cast<T*>(uniform);
	}

	
}