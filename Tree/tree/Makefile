CXX := g++
CXXFLAGS := -std=c++17 -Wall -g

TREETEST := Tree_test
TREEDEPS := Stack.h SeqStack.h

.PHONY: all clean

all: $(TREETEST) 

$(TREETEST): %_test: %_test.cpp %.h $(TREEDEPS) 
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TREETEST)
