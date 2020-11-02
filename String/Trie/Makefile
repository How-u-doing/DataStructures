CXX := g++
CXXFLAGS := -std=c++17 -Wall -g

tests := Trie_test

.PHONY: all clean

all: $(tests)

$(tests): %_test : %_test.cpp %.h
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(tests)
