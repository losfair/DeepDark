FLAGS := -O3
CXXFLAGS := $(FLAGS) -std=c++14 -I../include
LDFLAGS := $(FLAGS)

Core: Core.o Supervisor.o
	$(CXX) -o ../../target/Core Core.o Supervisor.o ../ConfigParser/ConfigParser.o -lpthread
