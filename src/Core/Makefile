FLAGS := -O3
CXXFLAGS := $(FLAGS) -std=c++14 -I../include
LDFLAGS := $(FLAGS)

Core: Core.o
	$(CXX) -o ../../target/Core Core.o ../ConfigParser/ConfigParser.o
