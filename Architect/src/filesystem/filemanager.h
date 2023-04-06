#pragma once
#include <string>
#include <unordered_map>

#include "../gfx/opengl/glshader.h"
#include "../gfx/opengl/gltexture.h"

#include "../audio/openal/audiobuffer.h"

#include "../ptr.h"

#include "../gfx/gui/guiwindow.h"


namespace archt {


	class Filemanager {


		static Filemanager instance;


		bool doCleanup = false;

		std::unordered_map<std::string, ptr<GLShader>> shaderFiles;
		std::unordered_map<std::string, ptr<GLTexture>> textureFiles;
		std::unordered_map<std::string, ptr<AudioBuffer>> audioFiles;

		GuiWindow* guiWindow = nullptr;

		Filemanager();
		~Filemanager();

	public:


		void deleteUnusedFiles();

		void createGuiWindow();


		inline void issueCleanup() { doCleanup = true; }
		inline bool cleanupIssued() const { return doCleanup; }

		static Filemanager& getInstance();



		template<typename T>
		ptr<T> loadFile(const std::string& path) {
			printf("Requested unsupported file type from filemanager\n File type = %s", typeid(T).name());
			return nullptr;
		}

		template<>
		ptr<GLShader> loadFile<GLShader>(const std::string& path) {
			
			if (shaderFiles.find(path) == shaderFiles.end())
				shaderFiles[path] = make_ptr<GLShader>(path);
			
			return shaderFiles[path];
		}

		template<>
		ptr<GLTexture> loadFile<GLTexture>(const std::string& path) {
			
			if (textureFiles.find(path) == textureFiles.end())
				textureFiles[path] = make_ptr<GLTexture>(path);

			return textureFiles[path];
		}

		template<>
		ptr<AudioBuffer> loadFile<AudioBuffer>(const std::string& path) {

			if (audioFiles.find(path) == audioFiles.end())
				audioFiles[path] = make_ptr<AudioBuffer>(path);

			return audioFiles[path];
		}

		template<typename T>
		void deleteReference(const std::string& file) {
			printf("Called unspecialised function\n");
			__debugbreak();
		}

		void deleteReference<GLShader>(const std::string& file) {
			
		}

		

		
	};

}