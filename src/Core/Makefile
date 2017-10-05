FLAGS := -O3
CXXFLAGS := $(FLAGS) -std=c++14 -I../include
LDFLAGS := $(FLAGS)
LINK_OBJS := Core.o Supervisor.o Network.o ../ConfigParser/ConfigParser.o ../Protocol/*.pb.o
LIBS := -lprotobuf -lgrpc++ -lgrpc -lpthread -lz -lssl -lcrypto -ldl -lrt

Core: Core.o Supervisor.o Network.o
	$(CXX) -O3 -o ../../target/Core $(LINK_OBJS) $(LIBS) -L/usr/local/lib
	$(CXX) -O3 -o ../../target/Core_static $(LINK_OBJS) $(LIBS) -L/usr/local/lib --static
	strip ../../target/Core ../../target/Core_static
