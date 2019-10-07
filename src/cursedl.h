#pragma once

#include "common.h"


namespace cursedl {

    // Sets the write function CurseDL will use.
    void SetWrite(std::function<void(const std::string&)> f);

    //
    // Gets modpack (ModpackID) with the optional choice to download version
    // (ModpackVersion)
    //
    bool GetModpack(const std::string& ModpackID, const std::string ModpackVersion);

}