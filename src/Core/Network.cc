#include "Network.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <stdio.h>

namespace deepdark {

class DeepDarkControlImpl final : public deepdark_proto::DeepDarkControl::Service {
public:
    NetworkServer& server;

    DeepDarkControlImpl(NetworkServer& _server) : server(_server) {}

    grpc::Status get_status(
        grpc::ServerContext *context,
        const deepdark_proto::GetStatusRequest *request,
        deepdark_proto::GetStatusResponse *response
    ) override {
        response -> set_description(server.supervisor.get_status());
        return grpc::Status::OK;
    }

    grpc::Status change_service_state(
        grpc::ServerContext *context,
        const deepdark_proto::ChangeServiceStateRequest *request,
        deepdark_proto::ChangeServiceStateResponse *response
    ) override {
        const std::string& name = request -> service_name();
        const std::string& op = request -> operation();

        if(name.empty()) {
            return grpc::Status(
                grpc::StatusCode::INVALID_ARGUMENT,
                "Service name must not be empty"
            );
        }

        bool ret = false;

        if(op == "start") {
            ret = server.supervisor.start_service(name);
        } else if(op == "stop") {
            ret = server.supervisor.stop_service(name);
        } else if(op == "restart") {
            server.supervisor.stop_service(name);
            ret = server.supervisor.start_service(name);
        } else {
            return grpc::Status(
                grpc::StatusCode::INVALID_ARGUMENT,
                "Unknown operation"
            );
        }

        if(ret) {
            response -> set_result("OK");
        } else {
            response -> set_result("Failed");
        }

        return grpc::Status::OK;
    }
};

NetworkServer::NetworkServer(Supervisor& _supervisor) : supervisor(_supervisor) {
}

NetworkServer::~NetworkServer() {
    std::cerr << "Fatal error: Network server should never get destroyed" << std::endl;
    std::terminate();
}

void NetworkServer::run(const std::string& listen_addr) {
    DeepDarkControlImpl service_impl(*this);
    grpc::ServerBuilder builder;
    builder.AddListeningPort(
        listen_addr,
        grpc::InsecureServerCredentials()
    );
    builder.RegisterService(&service_impl);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    if(server == nullptr) {
        throw std::runtime_error("Unable to start server");
    }

    std::cerr << "Server started" << std::endl;

    server -> Wait();
}

}
