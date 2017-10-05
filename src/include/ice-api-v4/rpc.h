#ifndef _ICE_API_RPC_H_
#define _ICE_API_RPC_H_

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct vIceRpcServerConfig {};
typedef struct vIceRpcServerConfig * IceRpcServerConfig;

struct vIceRpcServer {};
typedef struct vIceRpcServer * IceRpcServer;

struct vIceRpcCallContext {};
typedef struct vIceRpcCallContext * IceRpcCallContext;

struct vIceRpcParam {};
typedef struct vIceRpcParam * IceRpcParam;

struct vIceRpcClient {};
typedef struct vIceRpcClient * IceRpcClient;

struct vIceRpcClientConnection {};
typedef struct vIceRpcClientConnection * IceRpcClientConnection;

typedef void (*IceRpcMethodCallback)(
    IceRpcCallContext,
    void *
);

typedef void (*IceRpcClientConnectCallback)(
    IceRpcClientConnection,
    void *
);

typedef void (*IceRpcClientConnectionCallCallback)(
    const IceRpcParam,
    void *
);

IceRpcServerConfig ice_rpc_server_config_create();
void ice_rpc_server_config_destroy(IceRpcServerConfig config);
void ice_rpc_server_config_add_method(
    IceRpcServerConfig config,
    const char *name,
    IceRpcMethodCallback cb,
    void *call_with
);

IceRpcServer ice_rpc_server_create(IceRpcServerConfig config);
void ice_rpc_server_start(IceRpcServer server, const char *addr);

ice_uint32_t ice_rpc_call_context_get_num_params(IceRpcCallContext ctx);
IceRpcParam ice_rpc_call_context_get_param(IceRpcCallContext ctx, ice_uint32_t pos);
void ice_rpc_call_context_end(IceRpcCallContext ctx, IceRpcParam ret);

IceRpcParam ice_rpc_param_build_i32(ice_int32_t v);
IceRpcParam ice_rpc_param_build_f64(double v);
IceRpcParam ice_rpc_param_build_string(const char *v);
IceRpcParam ice_rpc_param_build_error(IceRpcParam from);
IceRpcParam ice_rpc_param_build_bool(ice_uint8_t v);
IceRpcParam ice_rpc_param_build_null();

ice_int32_t ice_rpc_param_get_i32(IceRpcParam p);
double ice_rpc_param_get_f64(IceRpcParam p);
ice_owned_string_t ice_rpc_param_get_string_to_owned(IceRpcParam p);
ice_uint8_t ice_rpc_param_get_bool(IceRpcParam p);
IceRpcParam ice_rpc_param_get_error(IceRpcParam p);
ice_uint8_t ice_rpc_param_is_null(IceRpcParam p);
void ice_rpc_param_destroy(IceRpcParam p);
IceRpcParam ice_rpc_param_clone(IceRpcParam p);

IceRpcClient ice_rpc_client_create(const char *addr);
void ice_rpc_client_destroy(IceRpcClient client);
void ice_rpc_client_connect(
    IceRpcClient client,
    IceRpcClientConnectCallback cb,
    void *call_with
);
void ice_rpc_client_connection_destroy(
    IceRpcClientConnection conn
);
void ice_rpc_client_connection_call(
    IceRpcClientConnection conn,
    const char *method_name,
    const IceRpcParam *params,
    ice_uint32_t num_params,
    IceRpcClientConnectionCallCallback cb,
    void *call_with
);

#ifdef __cplusplus
}
#endif

#endif
