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
    bool has_uid;
    uid_t uid;
    bool has_gid;
    gid_t gid;
    std::string username;
    std::string groupname;

    ServiceConfig();
    ServiceConfig(const ServiceConfig& other) = delete;
    ServiceConfig(ServiceConfig&& other) = delete;
    inline bool operator == (const ServiceConfig& other) const {
        return (
            name == other.name
            && command == other.command
            && directory == other.directory
            && autostart == other.autostart
            && autorestart == other.autorestart
            && has_uid == other.has_uid && uid == other.uid
            && has_gid == other.has_gid && gid == other.gid
            && username == other.username && groupname == other.groupname
        );
    }

    static std::unique_ptr<ServiceConfig> load(const std::string& config);
    static std::unique_ptr<ServiceConfig> load_from_file(const std::string& path);
};

class GlobalConfig {
public:
    std::string service_config_directory;
    std::string listen_addr;

    GlobalConfig();
    GlobalConfig(const ServiceConfig& other) = delete;
    GlobalConfig(ServiceConfig&& other) = delete;

    static std::unique_ptr<GlobalConfig> load(const std::string& config);
    static std::unique_ptr<GlobalConfig> load_from_file(const std::string& path);
};

class ParseError : public std::exception {
public:
    std::string description;

    virtual const char *what() const noexcept {
        return description.c_str();
    }

    static ParseError with_description(const std::string& d) {
        ParseError ret;
        ret.description = d;
        return ret;
    }
};

} // namespace deepdark
