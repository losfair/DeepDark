#pragma once

#include <memory>
#include <string>
#include <exception>
#include <sys/types.h>

namespace deepdark {

class ServiceConfig {
public:
    std::string name;
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
    static std::unique_ptr<ServiceConfig> load_from_file(const std::string& path);
};

class ParseError : public std::exception {
public:
    std::string description;

    virtual const char *what() const throw() {
        return description.c_str();
    }

    static ParseError with_description(const std::string& d) {
        ParseError ret;
        ret.description = d;
        return ret;
    }
};

} // namespace deepdark
