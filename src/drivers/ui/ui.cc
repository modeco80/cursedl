
#include "ui.h"

namespace cursedl {
namespace ui { 

  Fl_Input* modpack_id = nullptr;
  Fl_Input* modpack_version = nullptr;
  Fl_Text_Buffer* dl_output_buf = nullptr;
  Fl_Text_Display* dl_output = nullptr;
  Fl_Button* go_btn = nullptr;

  std::string textbuf;

  void WriteOutput(const std::string& str) {
      textbuf += "[CurseDL] ";
      textbuf += str;
      textbuf += '\n';
      dl_output_buf->text(textbuf.c_str());
  }

  // creates the UI window
  Fl_Double_Window* Create() {
      Fl_Double_Window* w = new Fl_Double_Window(460, 410);
      w->label("Curse Modpack Downloader");
      modpack_id = new Fl_Input(149, 25, 305, 24, "Curse Modpack ID");
      modpack_version = new Fl_Input(149, 54, 305, 24, "Optional Version ID");
      dl_output_buf = new Fl_Text_Buffer();
      dl_output = new Fl_Text_Display(25, 110, 420, 245, "Download Output");
      dl_output->buffer(dl_output_buf);
      go_btn = new Fl_Button(385, 375, 70, 24, "Go!");
      w->end();
      return w;
  }

}
}
