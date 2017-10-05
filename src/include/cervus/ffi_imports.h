#ifndef _CERVUS_FFI_IMPORTS_H_
#define _CERVUS_FFI_IMPORTS_H_

#ifdef __cplusplus
extern "C" {
#endif

struct vCervusFFICaller {
    char _[0];
};

typedef struct vCervusFFICaller * CervusFFICaller;

CervusFFICaller cervus_ffi_build_caller_with_context(
    void *target_fn,
    void *call_context,
    const char *ret_type,
    const char **param_types,
    unsigned int num_params
);

void cervus_ffi_destroy_caller(CervusFFICaller caller);
void * cervus_ffi_get_callable(CervusFFICaller caller);

#ifdef __cplusplus
}
#endif

#endif
