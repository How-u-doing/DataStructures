CXX := g++
CXXFLAGS := -std=c++17 -Wall

TESTS := _Tree_test TreeMap_test TST_test
COUNTWORDS  :=  count_words count_words_map count_words_BST count_words_TST

.PHONY: all clean

all: $(TESTS) $(COUNTWORDS)

$(TESTS): %_test : %_test.cpp %.h
	$(CXX) $(CXXFLAGS) -g -o $@ $<

count_words: count_words.cpp
	$(CXX) $(CXXFLAGS) -O3 -o $@ $<

count_words_map: count_words_map.cpp count_words.cpp
	$(CXX) $(CXXFLAGS) -O3 -o $@ $<

count_words_BST: count_words_BST.cpp count_words.cpp TreeMap.h _Tree.h
	$(CXX) $(CXXFLAGS) -O3 -o $@ $<

count_words_TST: count_words_TST.cpp count_words.cpp TST.h
	$(CXX) $(CXXFLAGS) -O3 -o $@ $<

clean:
	rm -f $(TESTS) $(COUNTWORDS)
