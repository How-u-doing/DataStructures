CXX := g++
CFLAGS := -std=c++17 -Wall -g

TESTS := _Tree_test TreeMap_test

all: $(TESTS) count_words

_Tree_test: _Tree_test.cpp _Tree.h
	$(CXX) $(CFLAGS) -o $@ $<

TreeMap_test: TreeMap_test.cpp TreeMap.h _Tree.h
	$(CXX) $(CFLAGS) -o $@ $<

count_words: count_words.cpp TreeMap.h _Tree.h
	$(CXX) -std=c++17 -Wall -Ofast -o $@ $<

clean:
	rm -f $(TESTS) count_words

.PHONY: all clean
