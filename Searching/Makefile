CXX := g++
CXXFLAGS := -std=c++17 -Wall -O3

COUNTWORDS := $(patsubst %.cpp, %, $(wildcard *.cpp))

.PHONY: all clean

all: $(COUNTWORDS)

# quick, dirty, lazy one
$(COUNTWORDS): % : %.cpp count_words.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(COUNTWORDS)
