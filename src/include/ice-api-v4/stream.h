#ifndef _ICE_API_STREAM_H_
#define _ICE_API_STREAM_H_

#include "types.h"
#include "wstream.h"
#include "rstream.h"

#ifdef __cplusplus
extern "C" {
#endif

struct IceStreamTxRxPair {
    IceWriteStream tx;
    IceReadStream rx;
};

void ice_stream_create_pair(struct IceStreamTxRxPair *out);

#ifdef __cplusplus
}
#endif

#endif
