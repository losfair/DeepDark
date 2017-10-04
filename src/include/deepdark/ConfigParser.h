#pragma once

#include <memory>
#include <string>
#include <sys/types.h>

namespace deepdark {

class ServiceConfig {
public:
    std::string command;
    std::string directory;
    bool autostart;
    bool autorestart;
    uid_t uid;
    gid_t gid;

    ServiceConfig();
    ServiceConfig(const ServiceConfig& other) = delete;
    ServiceConfig(ServiceConfig&& other) = delete;

    static std::unique_ptr<ServiceConfig> load(const std::string& config);
};

} // namespace deepdark
