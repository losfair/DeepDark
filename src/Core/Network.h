#pragma once

#include <memory>
#include <ice-api-v4/rpc.h>
#include <ice-api-v4/glue.h>
#include "Supervisor.h"

namespace deepdark {

class NetworkServer {
private:
    IceRpcServer rpc_server;
    Supervisor& supervisor;

public:
    NetworkServer(Supervisor& _supervisor);
    NetworkServer(const NetworkServer& other) = delete;
    NetworkServer(NetworkServer&& other) = delete;
    ~NetworkServer();

    void run(const std::string& listen_addr);
    void on_get_status(IceRpcCallContext ctx);
    void on_start_service(IceRpcCallContext ctx);
    void on_stop_service(IceRpcCallContext ctx);
};

}
