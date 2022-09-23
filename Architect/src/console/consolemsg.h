#pragma once

#include <glm/vec3.hpp>
#include <imgui.h>

#include <string>


namespace archt {


	class ConsoleMsg {

		
		static const std::string colorIndicator;

		int size = 1;
		std::string* tokens = nullptr;
		ImVec4* colors = nullptr;


	public:
		ConsoleMsg();
		ConsoleMsg(const std::string& msg);
		~ConsoleMsg();


		void operator()() const;




	};


}