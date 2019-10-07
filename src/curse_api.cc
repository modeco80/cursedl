#include "common.h"
#include "curse_api.h"
#include <cpr/cpr.h>

namespace cursedl {
namespace api {

    // The API base for all Curse launcher stuff
    const std::string CURSE_API_BASE = "https://addons-ecs.forgesvc.net/api/v2";

    nlohmann::json GetCurseFileInfo(const std::string& CurseProjectID) {
        auto req = cpr::Get(cpr::Url { CURSE_API_BASE + "/addon/" + CurseProjectID });
        if(req.status_code != 200) {
            throw Error("Could not get file info for Curse project " + CurseProjectID);
        }

        return nlohmann::json::parse(req.text);
    }

    nlohmann::json GetCurseFiles(const std::string& CurseProjectID) {
        auto req = cpr::Get(cpr::Url { CURSE_API_BASE + "/addon/" + CurseProjectID + "/files" });
        if(req.status_code != 200) {
            throw Error("Could not get files for Curse project " + CurseProjectID);
        }
        return nlohmann::json::parse(req.text);
    }


    std::tuple<bool, std::string> DownloadCurseFile(const nlohmann::json& CurseFileInfo) {
        if(CurseFileInfo.empty())
            return {false, ""};

        if(CurseFileInfo.find("downloadUrl") == CurseFileInfo.end())
            return {false, ""};

        auto request = cpr::Get(cpr::Url{CurseFileInfo["downloadUrl"]});
        if(request.status_code != 200) {
            return {false, ""};
        }
        return {true, request.text};
    }

}
}