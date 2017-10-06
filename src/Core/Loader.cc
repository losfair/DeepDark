#include "Loader.h"

namespace deepdark {

std::vector<std::unique_ptr<deepdark::ServiceConfig>> load_services_from_global_config(const deepdark::GlobalConfig& global_config) {
    DIR *dir = opendir(global_config.service_config_directory.c_str());
    if(!dir) {
        throw std::runtime_error(
            std::string("Unable to open service config directory: ") + global_config.service_config_directory
        );
    }

    std::vector<std::string> sc_paths;
    dirent *ent = NULL;
    while(ent = readdir(dir)) {
        if(ent -> d_type != DT_REG) continue;
        sc_paths.push_back(std::string(ent -> d_name));
    }

    closedir(dir);

    std::vector<std::unique_ptr<deepdark::ServiceConfig>> ret;
    for(auto& p : sc_paths) {
        std::cerr << "-> " << p << std::endl;
        try {
            ret.push_back(
                deepdark::ServiceConfig::load_from_file(
                    global_config.service_config_directory + "/" + p
                )
            );
        } catch(deepdark::ParseError& e) {
            std::cerr << "Error while parsing config file `" << p << "`: " << e.what() << std::endl;
        }
    }
    return ret;
}

} // namespace deepdark
