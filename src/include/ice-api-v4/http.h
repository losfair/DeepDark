#ifndef _ICE_API_HTTP_H_
#define _ICE_API_HTTP_H_

#include "types.h"
#include "rstream.h"

#ifdef __cplusplus
extern "C" {
#endif

struct vIceHttpServerConfig {};
typedef struct vIceHttpServerConfig * IceHttpServerConfig;

struct vIceHttpServer {};
typedef struct vIceHttpServer * IceHttpServer;

struct vIceHttpServerExecutionContext {};
typedef struct vIceHttpServerExecutionContext * IceHttpServerExecutionContext;

struct vIceHttpRouteInfo {};
typedef struct vIceHttpRouteInfo * IceHttpRouteInfo;

struct vIceHttpEndpointContext {};
typedef struct vIceHttpEndpointContext * IceHttpEndpointContext;

struct vIceHttpRequest {};
typedef struct vIceHttpRequest * IceHttpRequest;

struct vIceHttpResponse {};
typedef struct vIceHttpResponse * IceHttpResponse;

typedef void (*IceHttpRouteCallback) (
    IceHttpEndpointContext,
    IceHttpRequest,
    void *
);

typedef ice_uint8_t (*IceHttpReadBodyCallbackOnData) (
    const ice_uint8_t *data,
    ice_uint32_t len,
    void *call_with
);

typedef void (*IceHttpReadBodyCallbackOnEnd) (
    ice_uint8_t ok,
    void *call_with
);

typedef void (*IceHttpKeyValueIterInstantCallback) (
    const char *key,
    const char *value,
    void *call_with
);

IceHttpServerConfig ice_http_server_config_create();
void ice_http_server_config_destroy(IceHttpServerConfig cfg);
void ice_http_server_config_set_listen_addr(
    IceHttpServerConfig cfg,
    const char *addr
);
void ice_http_server_config_set_num_executors(
    IceHttpServerConfig cfg,
    ice_uint32_t n
);
IceHttpServer ice_http_server_create(
    IceHttpServerConfig cfg
);
IceHttpServerExecutionContext ice_http_server_start(
    IceHttpServer server
);
IceHttpRouteInfo ice_http_server_route_create(
    const char *path,
    IceHttpRouteCallback cb,
    void *call_with
);
void ice_http_server_route_destroy(
    IceHttpRouteInfo rt
);
void ice_http_server_add_route(
    IceHttpServer server,
    IceHttpRouteInfo rt
);
void ice_http_server_set_default_route(
    IceHttpServer server,
    IceHttpRouteInfo rt
);
IceHttpResponse ice_http_response_create();
void ice_http_response_destroy(
    IceHttpResponse resp
);
void ice_http_response_set_body(
    IceHttpResponse resp,
    const ice_uint8_t *data,
    ice_uint32_t len
);
void ice_http_response_set_status(
    IceHttpResponse resp,
    ice_uint16_t status
);
void ice_http_response_set_header(
    IceHttpResponse resp,
    const char *k,
    const char *v
);
void ice_http_response_append_header(
    IceHttpResponse resp,
    const char *k,
    const char *v
);
void ice_http_response_attach_rstream(
    IceHttpResponse resp,
    IceReadStream stream
);
void ice_http_server_endpoint_context_end_with_response(
    IceHttpEndpointContext ctx,
    IceHttpResponse resp
);
IceHttpRequest ice_http_server_endpoint_context_take_request(
    IceHttpEndpointContext ctx
);
void ice_http_request_destroy(
    IceHttpRequest req
);
ice_owned_string_t ice_http_request_get_uri_to_owned(
    IceHttpRequest req
);
ice_owned_string_t ice_http_request_get_method_to_owned(
    IceHttpRequest req
);
ice_owned_string_t ice_http_request_get_remote_addr_to_owned(
    IceHttpRequest req
);
ice_owned_string_t ice_http_request_get_header_to_owned(
    IceHttpRequest req,
    const char *k
);
void ice_http_request_iter_headers(
    IceHttpRequest req,
    IceHttpKeyValueIterInstantCallback cb,
    void *call_with
);
void ice_http_request_take_and_read_body(
    IceHttpRequest req,
    IceHttpReadBodyCallbackOnData cb_on_data,
    IceHttpReadBodyCallbackOnEnd cb_on_end,
    void *call_with
);
ice_uint8_t ice_storage_file_http_response_begin_send(
    IceHttpRequest req,
    IceHttpResponse resp,
    const char *path
);

#ifdef __cplusplus
}
#endif

#endif
