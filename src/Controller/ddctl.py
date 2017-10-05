import pyice
import asyncio

async def run():
    client = pyice.rpc.Client("127.0.0.1:8189")
    pool = pyice.rpc.ClientConnectionPool(client)
    await pool.init()

    ret = await pool.call("get_status", [])
    print(ret.get_string())

    ret = await pool.call("stop_service", [
        pyice.rpc.Param.build_string("test_service")
    ]);
    print(ret.get_bool())

loop = asyncio.get_event_loop()
loop.run_until_complete(run())
