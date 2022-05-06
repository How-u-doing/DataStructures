CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O3

COUNTWORDS := count_words_map count_words_avl count_words_bst  \
              count_words_rbt count_words_tst count_words_myht \
              count_words_myht2 count_words count_words_skiplist

.PHONY: all clean

all: $(COUNTWORDS)

count_words_map: count_words.cpp
	$(CXX) $(CXXFLAGS) -DUSE_STDMAP -o $@ $<

count_words_avl: count_words.cpp
	$(CXX) $(CXXFLAGS) -DUSE_AVL -o $@ $<

count_words_bst: count_words.cpp
	$(CXX) $(CXXFLAGS) -DUSE_BST -o $@ $<

count_words_rbt: count_words.cpp
	$(CXX) $(CXXFLAGS) -DUSE_RBT -o $@ $<

count_words_tst: count_words.cpp
	$(CXX) $(CXXFLAGS) -DUSE_TST -o $@ $<

count_words_myht: count_words.cpp
	$(CXX) $(CXXFLAGS) -DUSE_MYHT -o $@ $<

count_words_myht2: count_words.cpp
	$(CXX) $(CXXFLAGS) -DUSE_MYHT2 -o $@ $<

count_words: count_words.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

count_words_skiplist: count_words.cpp
	$(CXX) $(CXXFLAGS) -DUSE_SKIPLIST -o $@ $<

clean:
	rm -f $(COUNTWORDS)
