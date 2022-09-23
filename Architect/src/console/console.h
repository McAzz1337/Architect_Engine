#pragma once

#include "consolemsg.h"
#include "../containers/linkedlist.h"

namespace archt {

	class Console {

		char buffer[256];
		LinkedList<std::string> log;
		LinkedList<std::string> history;
		int historyPos = -1;

	public:
		Console();
		~Console();


		void operator<<(std::string* msg);
		
		static void Strtrim(char* s);

		void createGuiWindow();

		int textEditCallback(ImGuiInputTextCallbackData* data);


	};


}