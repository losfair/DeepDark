#ifndef _ICE_API_WSTREAM_H_
#define _ICE_API_WSTREAM_H_

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct vIceWriteStream {};
typedef struct vIceWriteStream * IceWriteStream;

void ice_stream_wstream_write(
    IceWriteStream target,
    const ice_uint8_t *data,
    ice_uint32_t data_len
);

void ice_stream_wstream_destroy(
    IceWriteStream target
);

#ifdef __cplusplus
}
#endif

#endif
