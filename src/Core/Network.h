#pragma once

#include <memory>
#include <grpc++/grpc++.h>
#include "Supervisor.h"
#include "../Protocol/DeepDarkControl.grpc.pb.h"

namespace deepdark {

class NetworkServer {
public:
    Supervisor& supervisor;

    NetworkServer(Supervisor& _supervisor);
    NetworkServer(const NetworkServer& other) = delete;
    NetworkServer(NetworkServer&& other) = delete;
    ~NetworkServer();

    void run(const std::string& listen_addr);
    /*
    void on_get_status(IceRpcCallContext ctx);
    void on_start_service(IceRpcCallContext ctx);
    void on_stop_service(IceRpcCallContext ctx);
    */
};

}
