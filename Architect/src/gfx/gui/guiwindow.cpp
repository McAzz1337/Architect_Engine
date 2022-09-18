#include "guiwindow.h"


namespace archt {
    
    
    GuiWindow::~GuiWindow() {
    }

   

    GuiWindowVoid::GuiWindowVoid(std::function<void()> func) : f(func) {}

    void GuiWindowVoid::render() {
        f();
    }



    GuiWindowVoid_s::GuiWindowVoid_s(std::function<void(bool*, GuiWindow*)> func) 
        : f(func) {}

    void GuiWindowVoid_s::render() {
        f(&open, this);
    }
}