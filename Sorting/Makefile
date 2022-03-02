CXX := g++
CXXFLAGS := -std=c++17 -Wall

.PHONY: all clean

TESTS := $(patsubst %.cpp, %, $(wildcard *.cpp))

STRING_SORTS := $(patsubst %.cc, %, $(wildcard *.cc))

all: $(TESTS) $(STRING_SORTS)

$(TESTS): %: %.cpp mySort.h
	$(CXX) $(CXXFLAGS) -g -o $@ $<

$(STRING_SORTS): %: %.cc string_sort.cc mySort.h
	$(CXX) $(CXXFLAGS) -O3 -o $@ $<

clean:
	rm -f $(TESTS) $(STRING_SORTS)
