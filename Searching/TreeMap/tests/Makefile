CXX := g++
CXXFLAGS := -std=c++17 -Wall -g

BSTTESTS := BstSet_test BstMap_test
BSTDEP   := ../BST_impl.h

AVLTESTS := AvlSet_test AvlMap_test
AVLDEP   := ../AVLtree_impl.h

TESTS := TST_test $(BSTTESTS) $(AVLTESTS)

.PHONY: all clean

all: $(TESTS)

TST_test: TST_test.cpp ../TST.h
	$(CXX) $(CXXFLAGS) -o $@ $<

$(BSTTESTS): %_test : %_test.cpp ../%.h $(BSTDEP)
	$(CXX) $(CXXFLAGS) -o $@ $<
    
$(AVLTESTS): %_test : %_test.cpp ../%.h $(AVLDEP)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TESTS)
