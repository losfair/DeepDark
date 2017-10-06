#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <string>
#include <dirent.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>
#include <unistd.h>
#include <deepdark/ConfigParser.h>

namespace deepdark {

std::vector<std::unique_ptr<deepdark::ServiceConfig>> load_services_from_global_config(const deepdark::GlobalConfig& global_config);

}
