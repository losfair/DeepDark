#include <deepdark/ConfigParser.h>
#include <deepdark/StringUtils.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <dirent.h>
#include <chrono>
#include <thread>
#include "Supervisor.h"

static void run(std::unique_ptr<deepdark::GlobalConfig> global_config);
static std::vector<std::unique_ptr<deepdark::ServiceConfig>> load_services(const deepdark::GlobalConfig& global_config);

int main(int argc, const char *argv[]) {
    if(argc < 2) {
        std::cerr << "Config file path required as the first command-line argument" << std::endl;
        return 1;
    }
    std::string global_config_path(argv[1]);
    auto global_config = deepdark::GlobalConfig::load_from_file(global_config_path);

    run(std::move(global_config));

    return 0;
}

static void run(std::unique_ptr<deepdark::GlobalConfig> global_config) {
    auto services = load_services(*global_config);
    deepdark::Supervisor supervisor(std::move(services));
    supervisor.start_all();

    using namespace std::chrono_literals;
    while(true) {
        std::this_thread::sleep_for(1000s);
    }
}

static std::vector<std::unique_ptr<deepdark::ServiceConfig>> load_services(const deepdark::GlobalConfig& global_config) {
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
        ret.push_back(
            deepdark::ServiceConfig::load_from_file(
                global_config.service_config_directory + "/" + p
            )
        );
    }
    return ret;
}
