CXX := g++
CXXFLAGS := -std=c++17 -Wall

.PHONY: clean

test: test.cc tortoise_hare.h
	$(CXX) $(CXXFLAGS) -g -o $@ $<

clean:
	rm -f test
