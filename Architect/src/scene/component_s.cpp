#include "component_s.h"

#include "../fileio.h"

#include "../audio/openal/aldebug.h"

#include <glm/gtx/transform.hpp>

#include "../filesystem/filemanager.h"

namespace archt {




	Transform_s::Transform_s() {}

	Transform_s::Transform_s(glm::mat4 transform) : transform(transform) {
	}

	Transform_s::Transform_s(const Transform_s& other) : transform(other.transform) {}
	
	//Transform_s::Transform_s(Transform_s&& other) noexcept : transform(other.transform)  {}


	Transform_s::~Transform_s() {
	}

	void Transform_s::translate(const glm::vec3& t) {
		transform = glm::translate(transform, t);
	}

	void Transform_s::rotate(float angle, const glm::vec3& axis) {
		transform = glm::rotate(transform, angle, axis);
	}

	void Transform_s::scale(const glm::vec3& s) {
		transform = glm::scale(transform, s);
	}




	Mesh_s::Mesh_s() {}

	Mesh_s::Mesh_s(const Mesh_s& other) {
		vbo = new VBO(other.vbo->getData(), other.vbo->getSize());
		ibo = new IBO(other.ibo->getData(), other.ibo->getSize());
	}

	//Mesh_s::Mesh_s(Mesh_s&& other) noexcept : vbo(other.vbo.getData(), other.vbo.getSize()) {}

	Mesh_s::Mesh_s(Vertex* verteces, uint32_t vSize, uint32_t* indeces, uint32_t iSize) {
		vbo = new VBO(verteces, vSize);
		ibo = new IBO(indeces, iSize);

	}

	Mesh_s::~Mesh_s() {
		delete vbo;
		delete ibo;
	}





	Material_s::Material_s() {}

	Material_s::Material_s(const Material_s& other)  {

		shader = other.shader;
		tex = other.tex;

		//tex = new GLTexture(other.tex->getFilePath());
		if (other.uniforms) {
			uniformCount = other.uniformCount;
			for (int i = 0; i < uniformCount; i++) {
				uniforms[i] = other.uniforms[i];
			}
		}
	}

	//Material_s::Material_s(Material_s&& other) noexcept : shader(other.shader.getFileName()) {
	//	uniformCount = other.uniformCount;
	//	if (other.uniforms) {
	//		for (int i = 0; i < uniformCount; i++) {
	//			uniforms[i] = other.uniforms[i];
	//		}
	//	}
	//	tex = new GLTexture(other.tex->getFilePath());
	//}

	Material_s::Material_s(const std::string& texturePath, const std::string& uniformsPath, const std::string& shaderPath) {

		//tex = new GLTexture(texturePath);
		
		Filemanager& fm = Filemanager::getInstance();

		shader = fm.loadFile<GLShader>(shaderPath);
		tex = fm.loadFile<GLTexture>(texturePath);
		
		std::vector<std::string> uniformsRaw;
		readFileSplit(uniformsPath, uniformsRaw);


		
		if (uniformsRaw.size() == 0)
			return;

		uniformCount = uniformsRaw.size();
		uniforms = new Uniform[uniformCount];

		for (int i = 0; i < uniformsRaw.size(); i++) {
			std::string components[2];
			split(uniformsRaw[i], '=', components, 2);
			std::vector<std::string> values;
			split(components[1], values, ',');

			switch (values.size()) {
				case 1:
					uniforms[i] = UniformFloat1(components[0], std::stof(values[0]));
					break;
				case 2: {
					glm::vec2 v = { std::stof(values[0]), std::stof(values[1]) };
					uniforms[i] = UniformFloat2(components[0], v);
					break;
				}
				case 3: {
					glm::vec3 v = { std::stof(values[0]), std::stof(values[1]), std::stof(values[2]) };
					uniforms[i] = UniformFloat3(components[0], v);
					break;
				}
				case 4: {
					glm::vec4 v = { std::stof(values[0]), std::stof(values[1]), std::stof(values[2]), std::stof(values[3]) };
					uniforms[i] = UniformFloat4(components[0], v);
					break;
				}
			}

		}

	}

	Material_s::~Material_s() {
		delete[] uniforms;
	}

	
	
	
	
	Tag::Tag() {}

	Tag::Tag(const Tag& other) : tag(other.tag) {}
	
	//Tag::Tag(Tag&& other) noexcept : tag(other.tag) {}


	Tag::Tag(const std::string& tag) : tag(tag) {}

	Tag::~Tag() {
	}





	AudioComponent::AudioComponent() {
	}

	AudioComponent::AudioComponent(const std::string& path) {
		
		Filemanager& fm = Filemanager::getInstance();
		buffers.push_back(fm.loadFile<AudioBuffer>(path));
	}

	AudioComponent::AudioComponent(const std::vector<std::string>& paths) {
		Filemanager& fm = Filemanager::getInstance();

		//buffers.reserve(paths.size());
		for (const auto& path : paths) {
			ptr<AudioBuffer> buf = fm.loadFile<AudioBuffer>(path);
			buffers.emplace_back(buf);
		}
	}

	AudioComponent::~AudioComponent() {
	
	}


}