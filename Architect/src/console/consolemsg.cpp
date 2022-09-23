#include "consolemsg.h"

#include <imgui.h>


namespace archt {

	const std::string ConsoleMsg::colorIndicator = "#";


	ConsoleMsg::ConsoleMsg() {
	
	}
	
	ConsoleMsg::ConsoleMsg(const std::string& msg) {

		size = 1;

		int i = msg.find(colorIndicator);
		while (i != std::string::npos) {
			
			size++;
			i = msg.find(colorIndicator);
		}

		tokens = new std::string[size];
		colors = new ImVec4[size];
		
		colors[0].x = 1.0f;
		colors[0].y = 1.0f;
		colors[0].z = 1.0f;
		colors[0].x = 1.0f;


		int start = 0;
		

		i = 0;
		bool notEnd = true;
		do {
			
			int end = msg.find(colorIndicator, start);


			notEnd = end != std::string::npos;

			if (notEnd) {
				
				colors[i].x = stof(msg.substr(end + 1, 3));
				colors[i].y = stof(msg.substr(end + 4, 3));
				colors[i].z = stof(msg.substr(end + 7, 3));
				colors[i].w = 1.0f;
			}
			
			tokens[i] = msg.substr(start, notEnd ? end : msg.length() - start);
			

			start = end + 10;
			i++;
		} while (i < size && notEnd);
	}

	ConsoleMsg::~ConsoleMsg() {

		delete[] tokens;
		delete[] colors;
	}


	void ConsoleMsg::operator()() const {

		for (int i = 0; i < size; i++) {
		
			ImGui::TextColored(colors[i], tokens[i].c_str());
			
			if (i < size - 1)
				ImGui::SameLine();
		}
	}


}