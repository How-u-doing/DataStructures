.PHONY: clean

CXX := g++
CXXFLAGS := -Wall -Wextra -Wpedantic

reverse_sublist: reverse_sublist.cc
	$(CXX) -o $@ $^ $(CXXFLAGS) -g

clean:
	rm reverse_sublist
