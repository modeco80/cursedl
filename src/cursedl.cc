#include "common.h"
#include "cursedl.h"
#include "curse_api.h"
#include <zipper/unzipper.h>
#include <sstream>


namespace cursedl {

    // Used to output information.
    // Can be basically anything, so that Cursedl's
    // downloader logic is mostly (if not completely)
    // seperate from the UI
    std::function<void(const std::string&)> WriteFunction;

    void SetWrite(std::function<void(const std::string&)> f) {
        WriteFunction = f;
    }

    bool GetModpack(const std::string& ModpackID, const std::string ModpackVersion) {
        nlohmann::json modInfo;
        nlohmann::json modVersions;

        try {
            WriteFunction("Getting information for Modpack ID " + ModpackID);
            modInfo = api::GetCurseFileInfo(ModpackID);
        } catch(api::Error& e) {
            WriteFunction("Error: " + std::string(e.what()));
            return false;
        }

        try {
            WriteFunction("Getting all files for Modpack ID " + ModpackID);
            modVersions = api::GetCurseFiles(ModpackID);
        } catch(api::Error& e) {
            WriteFunction("Error: " + std::string(e.what()));
            return false;
        }

        nlohmann::json file;

        if(ModpackVersion == "") {
            // just use the latest version
            WriteFunction("Using latest version");
            for(auto fileObj : modVersions) {
                if(fileObj["alternateFileId"] == 0) // is 0 = latest
                    file = fileObj;
            }
        } else {
            WriteFunction("Using version file ID " + ModpackVersion);
            std::uint64_t version{};

            try {
                 version = std::stoi(ModpackVersion);
            } catch(std::invalid_argument& e) {
                WriteFunction("Invalid file ID.");
                return false;
            }

            for(auto fileObj : modVersions) {
                if(fileObj["id"] == version)
                    file = fileObj;
            }
            if(file.empty()) {
                WriteFunction("Invalid file ID given.");
                return false;
            }
        }

        WriteFunction("Downloading modpack ZIP");
        auto packZip = api::DownloadCurseFile(file);

        if(std::get<0>(packZip) == false) {
            WriteFunction("Error downloading modpack ZIP");
            return false;
        }
        WriteFunction("ZIP download success");

        auto stream = std::istringstream(std::get<1>(packZip));
        auto zip = zipper::Unzipper(stream);
        // complete once commit finished

        return true;
    }

}