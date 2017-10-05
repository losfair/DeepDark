FLAGS := -O3
CXXFLAGS := $(FLAGS) -std=c++14 -I../include
LDFLAGS := $(FLAGS)
LINK_OBJS := Core.o Supervisor.o Network.o ../ConfigParser/ConfigParser.o ../Protocol/*.pb.o
DYNLIBS := -lpthread -lssl -lcrypto -lz
STATICLIBS := -l:libprotobuf.a -l:libgrpc++.a -l:libgrpc.a

Core: Core.o Supervisor.o Network.o
	$(CXX) -O3 -o ../../target/deepdark $(LINK_OBJS) $(STATICLIBS) $(DYNLIBS) -L/usr/local/lib
	strip ../../target/deepdark
