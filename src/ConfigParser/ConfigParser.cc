#include <deepdark/ConfigParser.h>
#include <deepdark/StringUtils.h>

#include <fstream>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <sstream>

namespace deepdark {

static bool must_parse_bool(const std::string& v);

ServiceConfig::ServiceConfig() {
    autostart = false;
    autorestart = false;
    uid = 0;
    gid = 0;
}

std::unique_ptr<ServiceConfig> ServiceConfig::load(const std::string& config) {
    const std::vector<std::string> lines = string_utils::split(config, "\n");
    std::unordered_map<std::string, std::string> fields;

    unsigned long line_id = 0;
    for(auto& _line : lines) {
        line_id++;

        auto line = string_utils::trim(_line);
        if(line.size() == 0) {
            continue;
        }

        auto parts = string_utils::split(line, "=", 2);
        if(parts.size() != 2) {
            throw ParseError::with_description(
                string_utils::append("Parse error at line ", line_id)
            );
        }
        std::string k = string_utils::trim(parts[0]);
        std::string v = string_utils::trim(parts[1]);
        fields[k] = v;
    }

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
    std::ifstream f(path, std::ios::binary);
    if(!f.is_open()) {
        throw std::runtime_error("Unable to open config file");
    }

    std::stringstream content_ss;
    content_ss << f.rdbuf();

    return load(content_ss.str());
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

} // namespace deepdark
