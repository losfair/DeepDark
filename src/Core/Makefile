FLAGS := -O3
CXXFLAGS := $(FLAGS) -std=c++14 -I../include
LDFLAGS := $(FLAGS)

Core: Core.o Supervisor.o Network.o
	$(CXX) -o ../../target/Core Core.o Supervisor.o Network.o ../ConfigParser/ConfigParser.o ../Protocol/*.pb.o -lpthread -lprotobuf -lgrpc -lgrpc++
