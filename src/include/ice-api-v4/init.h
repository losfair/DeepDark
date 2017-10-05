#ifndef _ICE_API_INIT_H_
#define _ICE_API_INIT_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "metadata.h"

#ifndef ICE_NO_VERSION_CHECK
static void __attribute__((constructor)) __check_ice_core_version() {
    const char *current_version = ice_metadata_get_version();
    fprintf(stderr, "Core version: %s\n", current_version);

    if(strncmp(current_version, "0.4.", 4) != 0) {
        fprintf(stderr, "Error: Core version mismatch\n");
        assert(0);
    }
}
#endif

#endif
