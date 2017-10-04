#pragma once

#include <vector>
#include <mutex>
#include <thread>
#include <deepdark/ConfigParser.h>
#include <sys/types.h>
#include <sys/wait.h>

namespace deepdark {

class Supervisor;
class ServiceState;

class Supervisor {
public:
    std::vector<std::unique_ptr<ServiceState>> services;

    Supervisor(std::vector<std::unique_ptr<deepdark::ServiceConfig>>);
};

class ServiceState {
private:
    bool running;

public:
    std::mutex m;
    std::unique_ptr<deepdark::ServiceConfig> config;
    std::unique_ptr<std::thread> executor;
    pid_t pid;
    int exit_status;

    ServiceState(std::unique_ptr<deepdark::ServiceConfig> cfg);
    ~ServiceState();
    inline bool is_running() { return running; }
    bool start();
    bool stop();
};

} // namespace deepdark
