CXX := g++
CXXFLAGS := -std=c++17 -Wall

.PHONY: clean all

TESTS := lcs_length lcs_test

all: $(TESTS) lcs_search

lcs_length: lcs_length.cc
	$(CXX) $(CXXFLAGS) -g -o $@ $<

lcs.o: lcs.cc lcs.h
	$(CXX) $(CXXFLAGS) -O3 -c $<

lcs_test.o: lcs_test.cc lcs.h
	$(CXX) $(CXXFLAGS) -g -c $<

lcs_test: lcs_test.o lcs.o
	$(CXX) $(CXXFLAGS) -o $@ $^

lcs_search.o: lcs_search.cc lcs.h
	$(CXX) $(CXXFLAGS) -O3 -c $<

lcs_search: lcs_search.o lcs.o
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TESTS) lcs_search *.o
