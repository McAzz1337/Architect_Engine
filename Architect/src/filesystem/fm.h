#pragma once

#include "../gfx/opengl/glshader.h"
#include "../gfx/opengl/gltexture.h"
#include "../audio/openal/audiobuffer.h"


#include <unordered_map>
#include <string>

namespace archt {
	class FM {

	public:

		static FM& instance;

	private:

		std::unordered_map<std::string, std::pair<GLShader, uint32_t>> shaders;
		std::unordered_map<std::string, std::pair<AudioBuffer, uint32_t>> audioBuffers;
		std::unordered_map<std::string, std::pair<GLTexture, uint32_t>> textures;

		

	public:
		FM();
		~FM();

		void clearUnused(const std::vector<void*>& retained);

		template<typename T>
		T& loadFile(const std::string& file) {
			printf("Called unspecialised function\n");
			__debugbreak();
		}

		template<>
		GLShader& loadFile<GLShader>(const std::string& file) {
			
			if (!shaders.contains(file)) {
				shaders[file] = std::make_pair(GLShader(file), 1);
			}
			else {
				shaders[file].second++;
			}

			return shaders[file].first;
		}

		template<>
		AudioBuffer& loadFile<AudioBuffer>(const std::string& file) {

			if (!audioBuffers.contains(file)) {
				audioBuffers[file] = std::make_pair(AudioBuffer(file), 1);
			}
			else {
				audioBuffers[file].second++;
			}

			return audioBuffers[file].first;
		}

		template<>
		GLTexture& loadFile<GLTexture>(const std::string& file) {

			if (!textures.contains(file)) {
				textures[file] = std::make_pair(GLTexture(file), 1);
			}
			else {
				textures[file].second++;
			}

			return textures[file].first;
		}

		template<typename T>
		void deleteReference(const std::string& file) {
			
			printf("Called unspecialised function\n");
			__debugbreak();
		}

		template<>
		void deleteReference<GLShader>(const std::string& file) {	
			
			shaders[file].second--;
		}

		template<>
		void deleteReference<AudioBuffer>(const std::string& file) {

			audioBuffers[file].second--;
		}

		template<>
		void deleteReference<GLTexture>(const std::string& file) {

			textures[file].second--;
		}
	};
}