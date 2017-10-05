#include "Network.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <stdio.h>

namespace deepdark {

NetworkServer::NetworkServer(Supervisor& _supervisor) : supervisor(_supervisor) {
    auto rpc_config = ice_rpc_server_config_create();

    ice_rpc_server_config_add_method(
        rpc_config,
        "get_status",
        [](IceRpcCallContext ctx, void *_this) { ((NetworkServer *) _this) -> on_get_status(ctx); },
        (void *) this
    );
    ice_rpc_server_config_add_method(
        rpc_config,
        "start_service",
        [](IceRpcCallContext ctx, void *_this) { ((NetworkServer *) _this) -> on_start_service(ctx); },
        (void *) this
    );
    ice_rpc_server_config_add_method(
        rpc_config,
        "stop_service",
        [](IceRpcCallContext ctx, void *_this) { ((NetworkServer *) _this) -> on_stop_service(ctx); },
        (void *) this
    );

    rpc_server = ice_rpc_server_create(rpc_config);
}

NetworkServer::~NetworkServer() {
    std::cerr << "Fatal error: Network server should never get destroyed" << std::endl;
    std::terminate();
}

void NetworkServer::run(const std::string& listen_addr) {
    ice_rpc_server_start(rpc_server, listen_addr.c_str());

    while(true) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1000s);
    }
}

void NetworkServer::on_get_status(IceRpcCallContext ctx) {
    std::string ret = supervisor.get_status();
    ice_rpc_call_context_end(ctx, ice_rpc_param_build_string(ret.c_str()));
}

void NetworkServer::on_start_service(IceRpcCallContext ctx) {
    int num_params = ice_rpc_call_context_get_num_params(ctx);
    if(num_params != 1) {
        ice_rpc_call_context_end(
            ctx,
            ice_rpc_param_build_error(
                ice_rpc_param_build_string("Invalid params")
            )
        );
        return;
    }

    IceRpcParam _name_p = ice_rpc_call_context_get_param(ctx, 0);
    ice_owned_string_t _name = ice_rpc_param_get_string_to_owned(_name_p);

    if(_name == NULL) {
        ice_rpc_call_context_end(
            ctx,
            ice_rpc_param_build_error(
                ice_rpc_param_build_string("String required as the first argument")
            )
        );
        return;
    }

    std::string name(_name);
    ice_glue_destroy_cstring(_name);

    bool ret = supervisor.start_service(name);
    ice_rpc_call_context_end(
        ctx,
        ice_rpc_param_build_bool(ret)
    );
}

void NetworkServer::on_stop_service(IceRpcCallContext ctx) {
    int num_params = ice_rpc_call_context_get_num_params(ctx);
    if(num_params != 1) {
        ice_rpc_call_context_end(
            ctx,
            ice_rpc_param_build_error(
                ice_rpc_param_build_string("Invalid params")
            )
        );
        return;
    }

    IceRpcParam _name_p = ice_rpc_call_context_get_param(ctx, 0);
    ice_owned_string_t _name = ice_rpc_param_get_string_to_owned(_name_p);

    if(_name == NULL) {
        ice_rpc_call_context_end(
            ctx,
            ice_rpc_param_build_error(
                ice_rpc_param_build_string("String required as the first argument")
            )
        );
        return;
    }

    std::string name(_name);
    ice_glue_destroy_cstring(_name);

    bool ret = supervisor.stop_service(name);
    ice_rpc_call_context_end(
        ctx,
        ice_rpc_param_build_bool(ret)
    );
}

}
