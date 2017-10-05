#pragma once

#include <vector>
#include <mutex>
#include <thread>
#include <string>
#include <deepdark/ConfigParser.h>
#include <sys/types.h>
#include <sys/wait.h>

namespace deepdark {

class Supervisor;
class ServiceState;

class Supervisor {
public:
    std::mutex m;
    std::vector<std::unique_ptr<ServiceState>> services;

    Supervisor(std::vector<std::unique_ptr<deepdark::ServiceConfig>>);
    ~Supervisor();
    void try_autostart_all();
    void try_autorestart_all();
    std::string get_status();
    bool start_service(const std::string& name);
    bool stop_service(const std::string& name);
};

class ServiceState {
private:
    bool running;
    bool should_autorestart;
    unsigned long update_time;
    unsigned long start_time;
    unsigned long stop_time;

public:
    std::recursive_mutex m;
    std::unique_ptr<deepdark::ServiceConfig> config;
    std::unique_ptr<std::thread> executor;
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
};

} // namespace deepdark
