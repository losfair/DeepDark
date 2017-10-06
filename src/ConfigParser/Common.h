#pragma once

#include <deepdark/ConfigParser.h>
#include <deepdark/StringUtils.h>

#include <fstream>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <sstream>

namespace deepdark {

static std::unordered_map<std::string, std::string> must_parse_kv_config(const std::string& config) {
    const std::vector<std::string> lines = string_utils::split(config, "\n");
    std::unordered_map<std::string, std::string> fields;

    unsigned long line_id = 0;
    for(auto& _line : lines) {
        line_id++;

        auto line = string_utils::trim(string_utils::split(_line, "#", 2)[0]);
        if(line.size() == 0) {
            continue;
        }

        auto parts = string_utils::split(line, "=", 2);
        if(parts.size() != 2) {
            throw ParseError::with_description(
                string_utils::append("Parse error at line ", line_id) + ": " + line
            );
        }
        std::string k = string_utils::trim(parts[0]);
        std::string v = string_utils::trim(parts[1]);
        fields[k] = v;
    }

    return fields;
}

static bool must_parse_bool(const std::string& v) {
    if(v == "1" || v == "true") {
        return true;
    } else if(v == "0" || v == "false") {
        return false;
    } else {
        throw ParseError::with_description(
            std::string("Unable to parse value as bool: ") + v
        );
    }
}

static std::string must_read_file(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    if(!f.is_open()) {
        throw std::runtime_error("Unable to open config file");
    }

    std::stringstream content_ss;
    content_ss << f.rdbuf();

    return content_ss.str();
}

} // namespace deepdark
