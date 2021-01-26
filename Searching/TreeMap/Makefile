CXX := g++
CXXFLAGS := -std=c++17 -Wall -g

TESTS := TST_test

.PHONY: all clean

all: $(TESTS)

TST_test: TST_test.cpp TST.h
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TESTS)
