#include "console.h"

#include "../gfx/gui/gui.h"

namespace archt {




	Console::Console() {
		
		strcpy(buffer, "");
	}

	Console::~Console() {
		delete[] buffer;
	}

	


	void Console::operator<<(std::string* msg) {
		log << msg;
	}

	void Console::Strtrim(char* s) { 
		
		char* str_end = s + strlen(s); 
	
		while (str_end > s && str_end[-1] == ' ') 
			str_end--; 
	
		*str_end = 0; 
	}


	void Console::createGuiWindow() {

		auto lambda = [this](const char* name, bool* open, GuiWindow* hanlde) {
			
			ImGui::Begin(name);

		

			Node<std::string>* msg = log[0];
			while (msg) {
				ImGui::Text(msg->data->c_str());
				msg = msg->next;
			}
			auto textEditCallback = [](ImGuiInputTextCallbackData* data) {
				Console* console = (Console*) data->UserData;
				return console->textEditCallback(data);
			};

			ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
			if (ImGui::InputText("cmd", buffer, 256, input_text_flags, textEditCallback, (void*) this)) {
				Strtrim(buffer);
				if (buffer[0]) {
					log >> new std::string(buffer);
					history >> new std::string(buffer);
				}
				printf("reset buf\n");
				strcpy(buffer, "");
				historyPos = -1;
			}



			ImGui::End();
		};
		Gui::getInstance()->addGuiWindow_void("Console", lambda);
	}



    int Console::textEditCallback(ImGuiInputTextCallbackData* data) {
        //AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
        switch (data->EventFlag) {
            case ImGuiInputTextFlags_CallbackCompletion:
            {
				
                break;
            }
            case ImGuiInputTextFlags_CallbackHistory:
            {
                
                const int prev_history_pos = historyPos;
                if (data->EventKey == ImGuiKey_UpArrow) {
                    if (historyPos == -1)
                        historyPos = history.size() - 1;
                    else if (historyPos > 0)
                        historyPos--;
                }
                else if (data->EventKey == ImGuiKey_DownArrow) {
                    if (historyPos != -1)
                        if (++historyPos == history.size())
                            historyPos = -1;
                }

                if (prev_history_pos != historyPos) {
                    const char* history_str = (historyPos > -1) ? history[historyPos]->data->c_str() : "";
					printf("historypos = %i\n", historyPos);
					printf("historystr = %s\n", history_str);


					strcpy(buffer, history_str);
					printf("buf = %s\n", buffer);
					
                }
            }
        }
        return 0;
    }

}