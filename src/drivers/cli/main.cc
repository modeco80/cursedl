#include "../../common.h"
#include "../../cursedl.h"

// write function for CurseDL CLI
void CliWrite(const std::string& str) {
    std::cout << str << '\n';
}

void CliUsage(const char* prog) {
    std::cout << "cursedl-cli : CLI interface to cursedl Curse Minecraft modpack downloader\n";
    std::cout << "(C) 2019 modeco80 under the MIT License\n";
    std::cout << "\t\b\b\b\b\b\b\b\bUsage:\n";
    std::cout << "\t\b\b\b\b" << prog << " [Curse modpack ID] <OPTIONAL: modpack file version ID>" << "\n";
}

int main(int argc, char** argv) {
    cursedl::SetWrite(&CliWrite);
    if(argc > 1) {
        std::string modpackID = "";
        std::string modpackVersion = "";
        modpackID = std::string(argv[1]);
        if(argc == 3) {
            modpackVersion = std::string(argv[2]);
        }

        if(!cursedl::DownloadModpack(modpackID, modpackVersion)) {
            std::cout << "Failure downloading modpack.\n";
            return 1;
        }
    } else {
        CliUsage(argv[0]);
        return 1;
    }
}
