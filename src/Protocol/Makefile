FLAGS := -O3
CXXFLAGS := $(FLAGS) -std=c++14 -I../include
LDFLAGS := $(FLAGS)

all:
	make protoc
	make build_objects

build_objects: DeepDarkControl.pb.o DeepDarkControl.grpc.pb.o

protoc:
	protoc -I=. --cpp_out=. DeepDarkControl.proto
	protoc -I=. --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` DeepDarkControl.proto
