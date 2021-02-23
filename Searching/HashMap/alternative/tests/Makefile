CXX := g++
CXXFLAGS := -std=c++17 -Wall -g

HASHTABLE_TESTS := HashSet2_test HashMap2_test
HASHTABLE_DEP   := ../Hashtable2_impl.h

.PHONY: all clean

all: $(HASHTABLE_TESTS)

$(HASHTABLE_TESTS): %_test : %_test.cpp ../%.h $(HASHTABLE_DEP)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(HASHTABLE_TESTS)
