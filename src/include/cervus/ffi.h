#ifndef _CERVUS_FFI_H_
#define _CERVUS_FFI_H_

#include "ffi_imports.h"
#include <vector>
#include <string>
#include <assert.h>

namespace Cervus {

class FFICaller {
private:
    CervusFFICaller inst;

public:
    FFICaller(
        void *target_fn,
        void *call_context,
        std::string ret_type,
        std::vector<std::string>& param_types
    ) {
        const char *ret_type_c = ret_type.c_str();
        unsigned int num_params = param_types.size();

        const char **param_types_c = new const char * [num_params];
        for(unsigned int i = 0; i < num_params; i++) {
            param_types_c[i] = param_types[i].c_str();
        }

        inst = cervus_ffi_build_caller_with_context(
            target_fn,
            call_context,
            ret_type_c,
            param_types_c,
            num_params
        );

        delete[] param_types_c;
    }

    ~FFICaller() {
        if(inst) {
            cervus_ffi_destroy_caller(inst);
        }
    }

    void * get_callable() {
        assert(inst);
        return cervus_ffi_get_callable(inst);
    }
};

}

#endif
