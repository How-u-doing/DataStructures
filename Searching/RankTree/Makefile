CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O3

.PHONY: all clean

all: benchmark

benchmark: benchmark.cc rolling_rank.h
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f benchmark
