import pyice
import asyncio
import argparse

async def run():
    parser = argparse.ArgumentParser(
        description = "Command-line interface for DeepDark"
    )
    parser.add_argument(
        "--remote",
        dest = "remote_addr",
        default = "127.0.0.1:8189",
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

    client = pyice.rpc.Client(args.remote_addr)
    pool = pyice.rpc.ClientConnectionPool(client)
    await pool.init()

    if args.action == "status":
        ret = await pool.call("get_status", [])
        print(ret.get_string())
    elif args.action == "start" or args.action == "stop":
        target = args.opt1
        if target == None:
            raise Exception("Service name required")

        if args.action == "start":
            op_name = "start_service"
        else:
            op_name = "stop_service"

        ret = await pool.call(op_name, [
            pyice.rpc.Param.build_string(target)
        ])
        if ret.get_bool():
            print("OK")
        else:
            print("Error")
    else:
        print("Unknown action: " + args.action)

loop = asyncio.get_event_loop()
loop.run_until_complete(run())
