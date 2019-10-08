#include "common.h"
#include "cursedl.h"
#include "curse_api.h"
#include <zipper/unzipper.h>
#include <sstream>
#include <filesystem>
namespace fs = std::filesystem;


namespace cursedl {

    // Used to output information.
    // Can be basically anything, so that Cursedl's
    // downloader logic is mostly (if not completely)
    // seperate from the UI
    std::function<void(const std::string&)> WriteFunction;

    void SetWrite(std::function<void(const std::string&)> f) {
        WriteFunction = f;
    }

    bool DownloadModpack(const std::string& ModpackID, const std::string ModpackVersion) {
        nlohmann::json modVersions;

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

        WriteFunction("Download success");

        std::istringstream stream(std::get<1>(packZip));
        zipper::Unzipper zip(stream);
        
        std::stringstream manifest_stream;
        if(!zip.extractEntryToStream("manifest.json", manifest_stream)) {
            WriteFunction("Couldn't extract manifest");
            return false;
        }

        auto manifest = nlohmann::json::parse(manifest_stream);
        std::string overridesDir = manifest["overrides"].get<std::string>();
        
        WriteFunction("Creating directory tree");

        if(!fs::exists(fs::current_path() / ("modpack-" + ModpackID)))
            fs::create_directories("modpack-" + ModpackID + "/mods");
        else
            WriteFunction("Existing directory tree found, using that");

        fs::path modpackPath = fs::current_path() / ("modpack-" + ModpackID);
        fs::path modRoot = modpackPath / "mods";

        for(auto manifestFile : manifest["files"]) {
            std::string modID = std::to_string(manifestFile["projectID"].get<std::uint64_t>());
            std::string fileID = std::to_string(manifestFile["fileID"].get<std::uint64_t>());
            WriteFunction("Getting dependent mod " + modID);
            
            nlohmann::json versions;
            nlohmann::json applicableVersion;

            try {
                versions = api::GetCurseFiles(modID);
            } catch(api::Error& e) {
                WriteFunction("Error: " + std::string(e.what()));
                return false;
            }

            for(auto version : versions) {
                if(std::to_string(version["id"].get<std::uint64_t>()) == fileID)
                    applicableVersion = version;
            }

            if(applicableVersion.empty()) {
                WriteFunction("Mod version ID " + fileID + "doesn't exist in Curse anymore. Bailing");
                return false;
            }

            if(!applicableVersion["dependencies"].empty()) {
				std::function<bool(const nlohmann::json&)> GetDepends = [&](const nlohmann::json& dependencies) {
					if(dependencies.empty()) {
						WriteFunction("No dependencies given");
						return true;
					}
					for(auto depend : dependencies) {
						nlohmann::json files;
						try {
				            WriteFunction("Getting dependency of mod " + std::to_string(depend["addonId"].get<std::uint64_t>()));
				            files = api::GetCurseFiles(std::to_string(depend["addonId"].get<std::uint64_t>()));
				        } catch(api::Error& e) {
				            WriteFunction("Error: " + std::string(e.what()));
    	        			return false;
				        }
					
						for(auto file : files) {
							if(file["alternateFileId"].get<std::uint64_t>() == 0) {

								if(!file["dependencies"].empty()) {
									// handle dependencies (of) dependencies
									WriteFunction("Getting dependencies of depend");
									if(!GetDepends(file["dependencies"])) {
										return false;
									}
								}

					            WriteFunction("Getting file " + std::to_string(file["id"].get<std::uint64_t>()));
			   			        std::string name = file["downloadUrl"].get<std::string>();
   		    				    name = name.substr(name.find_last_of('/'), std::string::npos);
	
				            	if(fs::exists(modRoot.native() + name)) {
				        	        WriteFunction("Already found depend of mod " + std::to_string(depend["addonId"].get<std::uint64_t>()));
									break;
								}

								auto fileTuple = api::DownloadCurseFile(file);
								if(std::get<0>(fileTuple) == false) {
									WriteFunction("Error getting depend of mod " + std::to_string(depend["addonId"].get<std::uint64_t>()));
									return false;
								}

    				        	std::stringstream fs(std::get<1>(fileTuple));
				            	std::ofstream outs(modRoot.native() + name);
				            	outs << fs.rdbuf();
				            	outs.close();

				        	    fs.clear();
								break;
							}
						}
					}
					return true;
				};

				if(!GetDepends(applicableVersion["dependencies"])) {
					WriteFunction("Error getting dependencies.");
					return false;
				}
			}

            auto mod = api::DownloadCurseFile(applicableVersion);
            if(std::get<0>(mod) == false) {
                WriteFunction("Failed to download mod " + modID);
                return false;
            }



            std::stringstream file(std::get<1>(mod));
            std::string name = applicableVersion["downloadUrl"].get<std::string>();
            name = name.substr(name.find_last_of('/'), std::string::npos);

            if(!fs::exists(modRoot.native() + name)) {
                std::ofstream outs(modRoot.native() + name);
                outs << file.rdbuf();
                outs.close();
            } else {
                WriteFunction("Already found mod " + fileID);
            }
            // cleanly free up resources even though this will probably be done anyways
            file.clear();

            WriteFunction("Finished downloading mod " + modID);
        }

        WriteFunction("Extracting overrides");
        for(auto entry : zip.entries()) {

            if(entry.name.find(overridesDir + '/') != std::string::npos) {
                // something we care about
                std::string FixedUpDir = entry.name.substr(overridesDir.length() + 1);

                if(FixedUpDir.find_last_of('/') == FixedUpDir.length() - 1) {
                    if(!fs::exists(modpackPath / FixedUpDir)){
                        fs::create_directories(modpackPath / FixedUpDir);
                    }
                } else {
                        std::stringstream s;
                        if(!zip.extractEntryToStream(overridesDir + '/' + FixedUpDir, s)) {
                            WriteFunction("Error during extraction");
                            return false;
                        }
                        std::ofstream outs((modpackPath / FixedUpDir).native());
                        if(!outs.is_open()) {
                            WriteFunction("Error creating file for writing");
                            return false;
                        }
                            
                        outs << s.rdbuf();
                        outs.close();
                        s.clear();
                }

            }

        }

        // free all of the resources
        manifest_stream.clear();
        zip.close();
        stream.clear();
        WriteFunction("Finished downloading modpack " + ModpackID);
        return true;
    }

}
