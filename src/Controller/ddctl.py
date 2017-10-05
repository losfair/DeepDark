import asyncio
import argparse
import grpc
import DeepDarkControl_pb2
import DeepDarkControl_pb2_grpc

def run():
    parser = argparse.ArgumentParser(
        description = "Command-line interface for DeepDark"
    )
    parser.add_argument(
        "--remote",
        dest = "remote_addr",
        default = "unix:/tmp/deepdark.sock",
        help = "Remote address to connect"
    )
    parser.add_argument(
        "action",
        type = str
    )
    parser.add_argument(
        "opt1",
        type = str,
        nargs = "?"
    )
    args = parser.parse_args()

    channel = grpc.insecure_channel(args.remote_addr)
    stub = DeepDarkControl_pb2_grpc.DeepDarkControlStub(channel)

    if args.action == "status":
        ret = stub.get_status(
            DeepDarkControl_pb2.GetStatusRequest()
        )
        print(ret.description)
    elif args.action == "start" or args.action == "stop" or args.action == "restart":
        target = args.opt1
        if target == None:
            print("Service name required")
        else:
            ret = stub.change_service_state(
                DeepDarkControl_pb2.ChangeServiceStateRequest(
                    service_name = target,
                    operation = args.action
                )
            )
            print(ret.result)
    else:
        print("Unknown action: " + args.action)

if __name__ == "__main__":
    run()
