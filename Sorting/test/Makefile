CXX := g++
CXXFLAGS := -std=c++17 -Wall -O3

MYSORTS :=  mysort mysort_copyswap
SINGLE := write qsort sort sort_pointers indirection_sort
TOGETHER := $(patsubst %.cpp, %, $(wildcard together/*MB.cpp))
ALL := $(SINGLE) $(TOGETHER) 

.PHONY: all single together clean

single: $(SINGLE) $(MYSORTS)

together: $(TOGETHER)

all: $(ALL)

$(MYSORTS): % : %.cpp mySort.h
	$(CXX) $(CXXFLAGS) -o $@ $<

$(SINGLE): % : %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(TOGETHER): % : %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<
	
clean:
	rm -f $(ALL)