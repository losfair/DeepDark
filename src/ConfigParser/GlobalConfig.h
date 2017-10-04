#pragma once

#include "Common.h"

namespace deepdark {

GlobalConfig::GlobalConfig() {
}

std::unique_ptr<GlobalConfig> GlobalConfig::load(const std::string& config) {
    auto fields = must_parse_kv_config(config);
    std::unique_ptr<GlobalConfig> ret(new GlobalConfig());

    ret -> service_config_directory = fields["service_config_directory"];

    return ret;
}

std::unique_ptr<GlobalConfig> GlobalConfig::load_from_file(const std::string& path) {
    return load(must_read_file(path));
}

} // namespace deepdark
