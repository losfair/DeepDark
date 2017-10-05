#ifndef _ICE_API_RSTREAM_H_
#define _ICE_API_RSTREAM_H_

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct vIceReadStream {};
typedef struct vIceReadStream * IceReadStream;

typedef ice_uint8_t (*IceReadStreamRecvCallbackOnData) (
    void *call_with,
    const ice_uint8_t *data,
    ice_uint32_t data_len
);

typedef void (*IceReadStreamRecvCallbackOnEnd) (
    void *call_with
);

typedef void (*IceReadStreamRecvCallbackOnError) (
    void *call_with
);

void ice_stream_rstream_begin_recv(
    IceReadStream target,
    IceReadStreamRecvCallbackOnData cb_on_data,
    IceReadStreamRecvCallbackOnEnd cb_on_end,
    IceReadStreamRecvCallbackOnError cb_on_error,
    void *call_with
);

void ice_stream_rstream_destroy(
    IceReadStream target
);

#ifdef __cplusplus
}
#endif

#endif
