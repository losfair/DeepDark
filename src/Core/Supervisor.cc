#include "Supervisor.h"
#include <iostream>
#include <exception>
#include <stdexcept>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

namespace deepdark {

Supervisor::Supervisor(std::vector<std::unique_ptr<deepdark::ServiceConfig>> cfgs) {
    for(auto& cfg : cfgs) {
        services.push_back(std::unique_ptr<ServiceState>(new ServiceState(std::move(cfg))));
    }
}

Supervisor::~Supervisor() {
    std::lock_guard<std::mutex> _lg(m);
}

void Supervisor::try_autostart_all() {
    std::lock_guard<std::mutex> _lg(m);

    for(auto& svc : services) {
        svc -> try_autostart();
    }
}

void Supervisor::try_autorestart_all() {
    std::lock_guard<std::mutex> _lg(m);

    for(auto& svc : services) {
        svc -> try_autorestart();
    }
}

ServiceState::ServiceState(std::unique_ptr<deepdark::ServiceConfig> cfg) {
    config = std::move(cfg);
    running = false;
    should_autorestart = false;
    pid = 0;
    exit_status = 0;
    update_time = 0;
    start_time = 0;
}

ServiceState::~ServiceState() {
    std::lock_guard<std::recursive_mutex> _lg(m);

    if(is_running()) {
        std::cerr << "Fatal error: Attempting to destroy a service while it is still running" << std::endl;
        std::terminate();
    }
}

bool ServiceState::try_autostart() {
    std::lock_guard<std::recursive_mutex> _lg(m);
    
    if(is_running() || !config -> autostart) {
        return false;
    }

    return start();
}

bool ServiceState::try_autorestart() {
    std::lock_guard<std::recursive_mutex> _lg(m);

    if(is_running() || !config -> autorestart || !should_autorestart) {
        return false;
    }

    if(time(0) - start_time < 5) {
        return false;
    }

    return start();
}

bool ServiceState::start() {
    std::lock_guard<std::recursive_mutex> _lg(m);

    if(is_running()) {
        return false;
    }

    assert(executor == nullptr);

    pid_t new_pid;

    if((new_pid = fork()) == 0) {
        execl("/bin/sh", "/bin/sh", "-c", config -> command.c_str(), NULL);
        std::cerr << "Error: Unable to execute /bin/sh" << std::endl;
        exit(1);
    }

    pid = new_pid;
    running = true;
    update_time = time(0);
    start_time = update_time;
    should_autorestart = true;

    // We assume that `this` is valid during the whole lifetime of the closure.
    executor = std::unique_ptr<std::thread>(new std::thread([this, new_pid]() {
        int exit_status = 0;
        int ret = waitpid(new_pid, &exit_status, 0);

        if(ret == -1) exit_status = -1;
        else exit_status = WEXITSTATUS(exit_status);

        std::lock_guard<std::recursive_mutex> _lg(this -> m);
        std::clog << "[*] Service `" << this -> config -> name << "` stopped with status " << exit_status << std::endl;
        this -> running = false;
        this -> exit_status = exit_status;
        this -> pid = 0;
        this -> update_time = time(0);
        this -> stop_time = this -> update_time;
        this -> executor -> detach();
        this -> executor.reset(nullptr);
    }));

    std::clog << "[*] Service `" << config -> name << "` started" << std::endl;

    return true;
}


// This function should NOT modify any fields of `this` except `should_autorestart`.
bool ServiceState::stop() {

    {
        std::lock_guard<std::recursive_mutex> _lg(m);
        
        if(!is_running()) {
            return false;
        }

        should_autorestart = false;

        assert(executor != nullptr);
        assert(pid > 0);

        kill(pid, SIGTERM);
        // The executor will stop after the managed process gets killed.
    }

    sleep(1);

    {
        std::lock_guard<std::recursive_mutex> _lg(m);
        if(!is_running()) {
            return true;
        }

        std::cerr << "Process is not stopped after 1 second. Waiting for another 5 seconds." << std::endl;
    }

    sleep(5);

    {
        std::lock_guard<std::recursive_mutex> _lg(m);
        if(!is_running()) {
            return true;
        }

        std::cerr << "Process is still not stopped after another 5 seconds. Killing." << std::endl;
        kill(pid, SIGKILL);
    }

    return true;
}

} // namespace deepdark
