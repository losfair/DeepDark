#pragma once

#include <deepdark/ConfigParser.h>
#include <deepdark/StringUtils.h>

#include <fstream>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <sstream>

#include "Common.h"

namespace deepdark {

ServiceConfig::ServiceConfig() {
    autostart = false;
    autorestart = false;
    uid = 0;
    gid = 0;
}

std::unique_ptr<ServiceConfig> ServiceConfig::load(const std::string& config) {
    auto fields = must_parse_kv_config(config);

    std::unique_ptr<ServiceConfig> ret(new ServiceConfig());

    ret -> name = fields["name"];
    ret -> command = fields["command"];
    ret -> directory = fields["directory"];
    if(fields.find("autostart") != fields.end()) {
        ret -> autostart = must_parse_bool(fields["autostart"]);
    }
    if(fields.find("autorestart") != fields.end()) {
        ret -> autorestart = must_parse_bool(fields["autorestart"]);
    }

    return ret;
}

std::unique_ptr<ServiceConfig> ServiceConfig::load_from_file(const std::string& path) {
    return load(must_read_file(path));
}

} // namespace deepdark
