#pragma once

#include "uniformbuffer.h"

#include <typeindex>
#include <string>

#include <glm/mat4x4.hpp>

#include <unordered_map>

namespace archt {

	class GLShader {

		static const std::string VS_EXT;
		static const std::string GS_EXT;
		static const std::string FS_EXT;


		uint32_t id = 0;
		std::string file;
		std::string vsrc;
		std::string gsrc;
		std::string fsrc;

		mutable std::unordered_map<std::string, int> uniforms;
		std::vector<std::string> uniformBufferNames;

		//Uniformbuffer* uniformBuffer = nullptr;
		std::vector<Uniformbuffer*> uniformBuffers;

	public:
		GLShader();
		GLShader(const GLShader& other);
		GLShader(GLShader&& other) noexcept;
		GLShader(const std::string& path);
		GLShader(const std::string& vsrc, const std::string& gsrc, const std::string& fsrc);
		~GLShader();
		
		void bind() const;
	
		int getUniformLocation(const std::string& name) const;


		void setUniformfv(const std::string& name, float* uniform, int count) const;
		void setUniform1f(const std::string& name, float uniform) const;
		void setUniform2f(const std::string& name, float* uniform) const;
		void setUniform3f(const std::string& name, float* uniform) const;
		void setUniform4f(const std::string& name, float* uniform) const;
		void setUniform4fv(const std::string& name, float* uniforms, int count) const;


		void setUniform1i(const std::string& name, int uniform) const;
		void setUniform1iv(const std::string& name, int count, int* uniforms) const;
		void setUniform1ui(const std::string& name, unsigned int uniform) const;


		void setMat4(const std::string& name, const glm::mat4& matrix) const;
		void setMatrixf4v(const std::string& name, glm::mat4* matrices, int count) const;

		void registerUniformBuffer(Uniformbuffer* buffer);

		void logShaderSource() const;

		inline uint32_t getProgramId() const { return id; }
		inline const std::string& getVertexSource() const { return vsrc; }
		inline const std::string& getGeometrySource() const { return gsrc; }
		inline const std::string& getFragmentSource() const { return fsrc; }

		inline const std::vector<Uniformbuffer*>& getUniformBuffers() const { return uniformBuffers; }
		inline const std::string& getFilePath() const { return file; }

		GLShader operator=(const GLShader& ohter) {
			return GLShader(ohter);
		}

	private:
		static void compileShader(const std::string& src, int type, int& id);
		static void createProgram(uint32_t& id, uint32_t vertex, uint32_t geometry, uint32_t fragment);
	};
}