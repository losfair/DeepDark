#include "Supervisor.h"
#include <iostream>
#include <exception>
#include <stdexcept>
#include <string>
#include <condition_variable>
#include <algorithm>
#include <set>
#include <map>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

namespace deepdark {

Supervisor::Supervisor(std::unique_ptr<GlobalConfig> gcfg) {
    load_global_config(std::move(gcfg));
}

Supervisor::~Supervisor() {
    std::lock_guard<std::mutex> _lg(m);
}

void Supervisor::load_global_config(std::unique_ptr<GlobalConfig> gcfg) {
    global_config = std::move(gcfg);
    update_services(load_services_from_global_config(*global_config));
}

void Supervisor::reload_global_config() {
    load_global_config(std::move(global_config));
}

// TODO: Write unit test to verify correctness
void Supervisor::update_services(std::vector<std::unique_ptr<ServiceConfig>> _cfgs) {
    std::lock_guard<std::mutex> _lg(m);

    std::set<std::string> current_names, new_names, name_intersection;
    std::map<std::string, std::unique_ptr<ServiceConfig>> cfgs;

    for(auto& cfg : _cfgs) {
        std::string name = cfg -> name;
        cfgs[name] = std::move(cfg);
    }

    for(auto& s : services) {
        current_names.insert(s -> get_name());
    }

    for(auto& cfg : cfgs) {
        new_names.insert(cfg.first);
    }

    std::set_intersection(
        current_names.begin(), current_names.end(),
        new_names.begin(), new_names.end(),
        std::inserter(name_intersection, name_intersection.begin())
    );

    std::vector<std::unique_ptr<ServiceState>> new_services;

    for(auto& s : services) {
        std::string name = s -> get_name();
        // Service still exists...
        if(name_intersection.find(name) != name_intersection.end()) {
            s -> update_config(std::move(cfgs[name]));
            new_services.push_back(std::move(s));
        }
    }

    for(auto& cfg : cfgs) {
        // This is a new service...
        if(name_intersection.find(cfg.first) == name_intersection.end()) {
            std::unique_ptr<ServiceState> state(new ServiceState(std::move(cfg.second)));

            // Try autostart because this is a new service
            state -> try_autostart();

            new_services.push_back(std::move(state));
        }
    }

    services = std::move(new_services);
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

std::string Supervisor::get_status() {
    std::lock_guard<std::mutex> _lg(m);

    std::string ret;
    for(auto& svc : services) {
        ret += "Service: ";
        ret += svc -> get_name();
        ret += "\n";
        ret += "Running: ";
        ret += svc -> is_running() ? "Yes" : "No";
        ret + "\n";

        ret += "\n";
    }

    return ret;
}

bool Supervisor::start_service(const std::string& name) {
    std::lock_guard<std::mutex> _lg(m);

    for(auto& svc : services) {
        if(svc -> get_name() == name) {
            return svc -> start();
        }
    }

    return false;
}

bool Supervisor::stop_service(const std::string& name) {
    std::lock_guard<std::mutex> _lg(m);
    
    for(auto& svc : services) {
        if(svc -> get_name() == name) {
            return svc -> stop();
        }
    }

    return false;
}

void Supervisor::prepare_for_shutdown() {
    std::lock_guard<std::mutex> _lg(m);

    for(auto& svc : services) {
        svc -> stop();
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
    stop();
}

void ServiceState::update_config(std::unique_ptr<ServiceConfig> cfg) {
    bool require_restart = false;
    {
        std::lock_guard<std::recursive_mutex> _lg(m);

        if(cfg -> name != config -> name) {
            throw std::runtime_error("Attempting to update a service with a different name");
        }

        if(*cfg == *config) {
            std::cerr << "[*] Not updating config for service " << config -> name << std::endl;
        } else {
            std::cerr << "[*] Updating config for service " << config -> name << std::endl;
            config = std::move(cfg);

            if(is_running()) {
                require_restart = true;
            }
        }
    }

    // Restart the service for the new config to take effect
    // We must release the lock here because `stop()` depends on another thread requiring this lock.
    if(require_restart) {
        stop();
        start();
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
        // Only one thread is running here. Things now become easy.
        if(config -> has_gid) {
            assert(setgid(config -> gid) == 0);
        } else if(config -> groupname.size() > 0) {
            group *entry = getgrnam(config -> groupname.c_str());
            if(entry) {
                assert(setgid(entry -> gr_gid) == 0);
            } else {
                std::cerr << "Error: Unable to find group entry" << std::endl;
                std::terminate();
            }
        }

        if(config -> has_uid) {
            assert(setuid(config -> uid) == 0);
        } else if(config -> username.size() > 0) {
            passwd *entry = getpwnam(config -> username.c_str());
            if(entry) {
                assert(setuid(entry -> pw_uid) == 0);
            } else {
                std::cerr << "Error: Unable to find passwd entry" << std::endl;
                std::terminate();
            }
        }

        if(config -> directory.size() > 0) {
            assert(chdir(config -> directory.c_str()) == 0);
        }

        execl("/bin/sh", "/bin/sh", "-c", config -> command.c_str(), NULL);
        std::cerr << "Error: Unable to execute /bin/sh" << std::endl;
        std::terminate();
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

        {
            std::lock_guard<std::recursive_mutex> _lg(this -> m);
            std::clog << "[*] Service `" << this -> config -> name << "` stopped with status " << exit_status << std::endl;
            this -> running = false;
            this -> exit_status = exit_status;
            this -> pid = 0;
            this -> update_time = time(0);
            this -> stop_time = this -> update_time;
            this -> executor -> detach();
            this -> executor.reset(nullptr);
        }

        // TODO: Does the lifetime of `this` still hold here?
        this -> executor_release.notify_all();
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

    {
        std::unique_lock<std::recursive_mutex> _ul(m);
        if(is_running()) {
            executor_release.wait(_ul);
            assert(is_running() == false);
        }
    }

    return true;
}

std::string ServiceState::get_name() {
    std::lock_guard<std::recursive_mutex> _lg(m);

    std::string name = config -> name;
    return name;
}

} // namespace deepdark
