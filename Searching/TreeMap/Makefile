CXX := g++
CXXFLAGS := -std=c++17 -Wall -g

TESTS := _Tree_test TreeMap_test TST_test

.PHONY: all clean

all: $(TESTS) count_words

$(TESTS): %_test : %_test.cpp %.h
	$(CXX) $(CXXFLAGS) -o $@ $<

count_words: count_words.cpp TreeMap.h _Tree.h
	$(CXX) -std=c++17 -Wall -O3 -o $@ $<

clean:
	rm -f $(TESTS) count_words