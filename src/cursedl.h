#pragma once

#include "common.h"


namespace cursedl {

    // Sets the write function CurseDL will use.
    void SetWrite(std::function<void(const std::string&)> f);

    //
    // Downloads modpack (ModpackID) with the optional choice to download version
    // (ModpackVersion)
    //
    bool DownloadModpack(const std::string& ModpackID, const std::string ModpackVersion);

}