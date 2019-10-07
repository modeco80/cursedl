// cursedl UI

#pragma once

#include "common.h"

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Button.H>

namespace cursedl {
namespace ui {
    extern Fl_Input* modpack_id;
    extern Fl_Input* modpack_version;
    extern Fl_Text_Display* dl_output;
    extern Fl_Button* go_btn;

    void WriteOutput(const std::string& str); 
    Fl_Double_Window* Create();
}
}
