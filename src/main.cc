
#include "common.h"
#include "cursedl.h"
#include "ui.h"


void GoCallback(Fl_Widget* widget, void* UserData) {
    std::string modpackId = cursedl::ui::modpack_id->value();
    std::string modpackVersion = cursedl::ui::modpack_version->value();

    if(modpackId == "") {
        cursedl::ui::WriteOutput("You need to specify a modpack ID!");
        return;
    }

    cursedl::ui::WriteOutput("Downloading " + modpackId + ( modpackVersion == "" ? "" : " (Version ID " + modpackVersion + ")"));
    // Run all of the modpack downloading stuff
    // somewhere (other) than the UI thread.
    // Shouldn't be as curse(d). HA. very funny.
    std::thread t([=]() {
        cursedl::ui::go_btn->deactivate();
        if(!cursedl::DownloadModpack(modpackId, modpackVersion)) {
            cursedl::ui::WriteOutput("Error getting modpack.");
        }
        cursedl::ui::go_btn->activate();
    });
    t.detach();
}

// TODO: 
// Offer a TUI only version?
int main(int argc, char** argv) {
    // Set the "library" write function
    // to our UI's write function
    cursedl::SetWrite(&cursedl::ui::WriteOutput);

    auto window = cursedl::ui::Create();
    cursedl::ui::go_btn->callback(GoCallback);
    window->show(argc, argv); 
    return Fl::run();
}