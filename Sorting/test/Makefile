CXX := g++
CXXFLAGS := -std=c++17 -Wall -O3

SINGLE := write qsort sort mysort
TOGETHER := $(patsubst %.cpp, %, $(wildcard together/*MB.cpp))
ALL := $(SINGLE) $(TOGETHER) 

.PHONY: all single together clean

single: $(SINGLE)

together: $(TOGETHER)

all: $(ALL)

$(SINGLE): % : %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(TOGETHER): % : %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<
	
clean:
	rm -f $(ALL)