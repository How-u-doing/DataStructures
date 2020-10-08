CXX := g++
CXXFLAGS := -std=c++17 -Wall -O3

headers := ../mySort.h mySort2.h

test1 := sort0 sort0+
test2 := sort  sort+

tests := $(test1) $(test2)

.PHONY: all clean

all: $(tests)

$(test1): % : %.cc
	$(CXX) $(CXXFLAGS) -o $@ $<

$(test2): % : %.cc $(headers)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(tests)

