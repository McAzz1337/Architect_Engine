#include "uniform.h"

#include <typeinfo>

namespace archt {

	static void test() {
		Uniform* u = new UniformFloat4("vec", { 1.0f });
		
		UniformFloat4* vec = uniform_cast<UniformFloat4>(u);
	}

	Uniform::Uniform() {}

	Uniform::Uniform(const std::string& name, int size) : name(name), size(size) {}

	Uniform::~Uniform() {}




	UniformFloat1::UniformFloat1() {}

	UniformFloat1::UniformFloat1(const std::string& name, float uniform) : Uniform(name, 1) {}

	UniformFloat1::~UniformFloat1() {}






	UniformFloat2::UniformFloat2() {}

	UniformFloat2::UniformFloat2(const std::string& name, const glm::vec2& uniform) : Uniform(name, 2) {}

	UniformFloat2::~UniformFloat2() {}





	UniformFloat3::UniformFloat3() {}

	UniformFloat3::UniformFloat3(const std::string& name, const glm::vec3& uniform) : Uniform(name, 3) {}

	UniformFloat3::~UniformFloat3() {}








	UniformFloat4::UniformFloat4() {}

	UniformFloat4::UniformFloat4(const std::string& name, const glm::vec4& uniform) : Uniform(name, 4) {}

	UniformFloat4::~UniformFloat4() {}





	UniformMatrix4::UniformMatrix4() {}

	UniformMatrix4::UniformMatrix4(const std::string& name, const glm::mat4& uniform) : Uniform(name, 1) {}

	UniformMatrix4::~UniformMatrix4() {}
}