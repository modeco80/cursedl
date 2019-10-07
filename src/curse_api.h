#pragma once

#include "common.h"
#include <nlohmann/json.hpp>


namespace cursedl {
namespace api {

    // generic error
    struct Error: public std::exception {
        std::string what_;
        Error(std::string what) : what_(what) {}

        const char* what() throw() {
            return what_.c_str();
        } 
    };

    // document later

    nlohmann::json GetCurseFileInfo(const std::string& CurseProjectID);

    // gets ALL the files,
    // not just what latestFiles contains
    nlohmann::json GetCurseFiles(const std::string& CurseProjectID);

    std::tuple<bool, std::string> DownloadCurseFile(const nlohmann::json& CurseFileInfo);
}
}