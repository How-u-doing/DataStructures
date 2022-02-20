CXX := g++
CXXFLAGS := -std=c++17 -Wall -g

BSTTESTS := BstSet_test BstMap_test
BSTDEP   := ../BST_impl.h

AVLTESTS := AvlSet_test AvlMap_test
AVLDEP   := ../AVLtree_impl.h
AVL_INS_DEL_TESTS := AVL_insertion_test AVL_deletion_test

RBTESTS := RbSet_test RbMap_test
RBDEP   := ../RBtree_impl.h
RB_INS_DEL_TESTS := RB_insertion_test RB_deletion_test

TESTS := AVL_unit_tests TST_test $(BSTTESTS) $(AVLTESTS) $(AVL_INS_DEL_TESTS) $(RBTESTS) $(RB_INS_DEL_TESTS)

.PHONY: all clean

all: $(TESTS)

AVL_unit_tests: AVL_unit_tests.cpp ../AvlMap.h $(AVLDEP)
	$(CXX) $(CXXFLAGS) -o $@ $<

TST_test: TST_test.cpp ../TST.h
	$(CXX) $(CXXFLAGS) -o $@ $<

$(BSTTESTS): %_test : %_test.cpp ../%.h $(BSTDEP)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(AVLTESTS): %_test : %_test.cpp ../%.h $(AVLDEP)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(AVL_INS_DEL_TESTS): % : %.cpp ../AvlMap.h $(AVLDEP)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(RBTESTS): %_test : %_test.cpp ../%.h $(RBDEP)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(RB_INS_DEL_TESTS): % : %.cpp ../RbMap.h $(RBDEP)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TESTS)
