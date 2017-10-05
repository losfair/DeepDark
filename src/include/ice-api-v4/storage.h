#ifndef _ICE_API_STORAGE_H_
#define _ICE_API_STORAGE_H_

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct vIceKVStorage {};
typedef struct vIceKVStorage * IceKVStorage;

struct vIceKVStorageHashMapExt {};
typedef struct vIceKVStorageHashMapExt * IceKVStorageHashMapExt;

typedef void (*IceKVStorageGetItemCallback) (void *data, const char *value);
typedef void (*IceKVStorageSetItemCallback) (void *data);
typedef void (*IceKVStorageRemoveItemCallback) (void *data);

IceKVStorage ice_storage_kv_create_with_redis_backend(
    const char *conn_str
);
void ice_storage_kv_destroy(IceKVStorage handle);
void ice_storage_kv_get(
    IceKVStorage handle,
    const char *k,
    IceKVStorageGetItemCallback cb,
    void *call_with
);
void ice_storage_kv_set(
    IceKVStorage handle,
    const char *k,
    const char *v,
    IceKVStorageSetItemCallback cb,
    void *call_with
);
void ice_storage_kv_remove(
    IceKVStorage handle,
    const char *k,
    IceKVStorageRemoveItemCallback cb,
    void *call_with
);
void ice_storage_kv_expire_sec(
    IceKVStorage handle,
    const char *k,
    ice_uint32_t t,
    IceKVStorageSetItemCallback cb,
    void *call_with
);

IceKVStorageHashMapExt ice_storage_kv_get_hash_map_ext(
    IceKVStorage handle
);

void ice_storage_kv_hash_map_ext_get(
    IceKVStorageHashMapExt hm,
    const char *k,
    const char *map_key,
    IceKVStorageGetItemCallback cb,
    void *call_with
);

void ice_storage_kv_hash_map_ext_set(
    IceKVStorageHashMapExt hm,
    const char *k,
    const char *map_key,
    const char *v,
    IceKVStorageSetItemCallback cb,
    void *call_with
);


void ice_storage_kv_hash_map_ext_remove(
    IceKVStorageHashMapExt hm,
    const char *k,
    const char *map_key,
    IceKVStorageRemoveItemCallback cb,
    void *call_with
);

#ifdef __cplusplus
}
#endif

#endif
