CXX := g++
CXXFLAGS := -std=c++17 -Wall -g

LRUCACHE      := LRUCache_test LRUPageReplacement
LRUCACHE_DEP  := ../Hashtable_impl.h

.PHONY: all clean

all: $(LRUCACHE)

$(LRUCACHE): % : %.cc LRUCache.h $(LRUCACHE_DEP)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(LRUCACHE)
