CXX := g++
CXXFLAGS := -std=c++17 -Wall -g

SKIPLIST_TESTS := SkiplistSet_test SkiplistMap_test
SKIPLIST_DEP   := ../SkipList_impl.h

TESTS := $(SKIPLIST_TESTS)

.PHONY: all clean

all: $(TESTS)

$(SKIPLIST_TESTS): %_test : %_test.cpp ../%.h $(SKIPLIST_DEP)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TESTS)