#include <deepdark/ConfigParser.h>
#include <iostream>
#include <string>

int main(int argc, const char *argv[]) {
    if(argc < 2) {
        std::cerr << "Config file path required as the first command-line argument" << std::endl;
        return 1;
    }
    std::string global_config_path(argv[1]);
    auto global_config = deepdark::GlobalConfig::load_from_file(global_config_path);

    return 0;
}
