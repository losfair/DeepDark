#pragma once

#include <vector>
#include <mutex>
#include <thread>
#include <string>
#include <condition_variable>
#include <deepdark/ConfigParser.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "Loader.h"

namespace deepdark {

class Supervisor;
class ServiceState;

class Supervisor {
public:
    std::mutex m;
    std::vector<std::unique_ptr<ServiceState>> services;
    std::unique_ptr<GlobalConfig> global_config;

    Supervisor(std::unique_ptr<GlobalConfig> gcfg);
    ~Supervisor();
    void load_global_config(std::unique_ptr<GlobalConfig> gcfg);
    void reload_global_config();
    void try_autostart_all();
    void try_autorestart_all();
    std::string get_status();
    bool start_service(const std::string& name);
    bool stop_service(const std::string& name);
    void prepare_for_shutdown();
    void update_services(std::vector<std::unique_ptr<ServiceConfig>> _cfgs);
};

class ServiceState {
private:
    bool running;
    bool should_autorestart;
    unsigned long update_time;
    unsigned long start_time;
    unsigned long stop_time;
    std::recursive_mutex m;
    std::unique_ptr<deepdark::ServiceConfig> config;
    std::unique_ptr<std::thread> executor;
    std::condition_variable_any executor_release;

public:
    pid_t pid;
    int exit_status;

    ServiceState(std::unique_ptr<deepdark::ServiceConfig> cfg);
    ~ServiceState();
    inline bool is_running() const { return running; }
    inline unsigned long get_update_time() const { return update_time; }
    inline pid_t get_pid() const { return pid; }
    inline int get_exit_status() const { return exit_status; }
    bool start();
    bool stop();
    bool try_autostart();
    bool try_autorestart();
    void update_config(std::unique_ptr<ServiceConfig> cfg);
    std::string get_name();
};

} // namespace deepdark
