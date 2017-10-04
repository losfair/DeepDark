#include <deepdark/ConfigParser.h>

namespace deepdark {

ServiceConfig::ServiceConfig() {
    uid = 0;
    gid = 0;
}
/*
std::unique_ptr<ServiceConfig> ServiceConfig::load(const std::string& config) {
}*/

} // namespace deepdark
