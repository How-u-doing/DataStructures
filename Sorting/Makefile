CXX := g++
CXXFLAGS := -std=c++17 -Wall -g

.PHONY: all clean

TESTS := $(patsubst %.cpp, %, $(wildcard *.cpp))

all: $(TESTS)

$(TESTS): %: %.cpp mySort.h
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TESTS)
