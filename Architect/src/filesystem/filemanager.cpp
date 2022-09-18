#include "filemanager.h"

#include "../fileio.h"

#include "../gfx/gui/gui_s.h"

namespace archt {
	

	Filemanager Filemanager::instance;


	Filemanager::Filemanager() {
	
	}
	
	Filemanager::~Filemanager() {
		
	}

	void Filemanager::deleteUnusedFiles() {
		
		for (auto& shader : shaderFiles) {
			if (shader.second.use_count() == 1)
				shaderFiles.erase(shader.first);
		}

		for (auto& texture : textureFiles) {
			if (texture.second.use_count() == 1)
				textureFiles.erase(texture.first);
		}

		for (auto& buffer : audioFiles) {
			if (buffer.second.use_count() == 1)
				audioFiles.erase(buffer.first);
		}

	}

	void Filemanager::createGuiWindow() {
		
		if (guiWindow) {
			return;
		}

		auto lambda = [this](bool* open, GuiWindow_s* window) {

			ImGui::Begin("Filemanager", open);

			ImGui::BeginTabBar("Tabs");

			
			
			if (ImGui::BeginTabItem("Shaders")) {
				
				ImGui::Text("Count = %i", shaderFiles.size());

				for (auto& shader : shaderFiles) {
					std::string header = "";
					extractFileName(shader.second->getFilePath(), header, '/');

					if (ImGui::CollapsingHeader(header.c_str())) {
						if (ImGui::CollapsingHeader("Vertex source")) {
							ImGui::Text(shader.second->getVertexSource().c_str());
						}
						const std::string& gsrc = shader.second->getGeometrySource();
						if (gsrc.size() > 0) {
							if (ImGui::CollapsingHeader("Geometry source")) {
								ImGui::Text(gsrc.c_str());
							}
						}
						if (ImGui::CollapsingHeader("Fragment source")) {
							ImGui::Text(shader.second->getFragmentSource().c_str());
						}
					}

				}

				ImGui::EndTabItem();
			}
			
			if (ImGui::BeginTabItem("Textures")) {

				ImGui::Text("Count = %i", textureFiles.size());

				for (auto& tex : textureFiles) {
					std::string header = "";
					extractFileName(tex.second->getFilePath(), header, '/');
					if (ImGui::CollapsingHeader(header.c_str())) {
						glm::vec2 s = tex.second->getSize();
						ImVec2 size = { s.x, s.y };
						ImGui::Image((ImTextureID) tex.second->getId(), size, { 0, 1 }, { 1, 0 });
					}
				}

				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();

			ImGui::End();

		};

		guiWindow = Gui_s::getInstance()->addGuiWindow_void(lambda);
	}
	
	Filemanager& Filemanager::getInstance() {
		return instance;
	}
}