FLAGS := -O3
CXXFLAGS := $(FLAGS) -std=c++14 -I../include
LDFLAGS := $(FLAGS)

StringUtils_test: StringUtils_test.o
	$(CXX) -o StringUtils_test StringUtils_test.o
	./StringUtils_test
	rm StringUtils_test
