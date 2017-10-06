#include <deepdark/ConfigParser.h>
#include <deepdark/StringUtils.h>
#include <deepdark/Version.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <dirent.h>
#include <chrono>
#include <thread>
#include <memory>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>
#include <unistd.h>
#include "Supervisor.h"
#include "Network.h"

static void run(std::unique_ptr<deepdark::GlobalConfig> global_config);
static std::vector<std::unique_ptr<deepdark::ServiceConfig>> load_services(const deepdark::GlobalConfig& global_config);
static std::unique_ptr<deepdark::Supervisor> supervisor;

void gracefully_exit_on_signal(int signo) {
    // We do immediate exit here to prevent destructors from being called.
    // FIXME: Make supervisor cleanup correctly.

    if(supervisor == nullptr) {
        _exit(0);
    }

    // We are in a signal handler. Start a thread to avoid deadlock.
    std::thread t([]() {
        supervisor -> prepare_for_shutdown();
        _exit(0);
    });

    t.detach();
}

int main(int argc, const char *argv[]) {
    std::cerr << "DeepDark Core v" << deepdark::VERSION << std::endl;

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

    assert(supervisor == nullptr);
    supervisor.reset(new deepdark::Supervisor(std::move(services)));

    signal(SIGINT, gracefully_exit_on_signal);
    signal(SIGTERM, gracefully_exit_on_signal);

    supervisor -> try_autostart_all();

    using namespace std::chrono_literals;
    std::thread monitor_thread([]() {
        while(true) {
            std::this_thread::sleep_for(1s);
            supervisor -> try_autorestart_all();
        }
    });

    deepdark::NetworkServer server(*supervisor);
    server.run(global_config -> listen_addr);

    std::cerr << "Fatal error: Server exited unexpectedly" << std::endl;
    std::terminate();
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
