FLAGS := -O3
CXXFLAGS := $(FLAGS) -std=c++14 -I../include
LDFLAGS := $(FLAGS)

all: build_object

build_object: protoc DeepDarkControl.pb.o

protoc:
	protoc -I=. --cpp_out=. DeepDarkControl.proto
