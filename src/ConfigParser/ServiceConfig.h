#pragma once

#include <deepdark/ConfigParser.h>
#include <deepdark/StringUtils.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <stdlib.h>

#include "Common.h"

namespace deepdark {

ServiceConfig::ServiceConfig() {
    autostart = false;
    autorestart = false;
    has_uid = false;
    uid = 0;
    has_gid = false;
    gid = 0;
}

std::unique_ptr<ServiceConfig> ServiceConfig::load(const std::string& config) {
    auto fields = must_parse_kv_config(config);

    std::unique_ptr<ServiceConfig> ret(new ServiceConfig());

    ret -> name = fields["name"];
    if(ret -> name.size() == 0) {
        throw ParseError::with_description("Service name required as field `name`");
    }
    ret -> command = fields["command"];
    ret -> directory = fields["directory"];
    if(fields.find("autostart") != fields.end()) {
        ret -> autostart = must_parse_bool(fields["autostart"]);
    }
    if(fields.find("autorestart") != fields.end()) {
        ret -> autorestart = must_parse_bool(fields["autorestart"]);
    }

    if(fields.find("uid") != fields.end()) {
        ret -> uid = atoi(fields["uid"].c_str());
        ret -> has_uid = true;
    }
    if(fields.find("gid") != fields.end()) {
        ret -> gid = atoi(fields["gid"].c_str());
        ret -> has_gid = true;
    }

    ret -> username = fields["username"];
    ret -> groupname = fields["groupname"];

    return ret;
}

std::unique_ptr<ServiceConfig> ServiceConfig::load_from_file(const std::string& path) {
    return load(must_read_file(path));
}

} // namespace deepdark
