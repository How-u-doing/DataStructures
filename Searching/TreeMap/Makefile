CXX := g++
CXXFLAGS := -std=c++17 -Wall -g

TESTS := _Tree_test TreeMap_test TST_test
COUNTWORDS  :=  count_words count_words_BST count_words_TST

.PHONY: all clean

all: $(TESTS) $(COUNTWORDS)

$(TESTS): %_test : %_test.cpp %.h
	$(CXX) $(CXXFLAGS) -o $@ $<

$(COUNTWORDS): % : %.cpp
	$(CXX) -std=c++17 -Wall -O3 -o $@ $<

clean:
	rm -f $(TESTS) $(COUNTWORDS)