#!/bin/bash
python3 -m grpc_tools.protoc -I../Protocol --python_out=. --grpc_python_out=. ../Protocol/DeepDarkControl.proto
