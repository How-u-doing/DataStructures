CXX := g++
CXXFLAGS := -std=c++17 -Wall -O3

TARGETS := Huffman

DEPS := BinaryIO.h Heap.h

.PHONY: all clean

all: $(TARGETS)

$(TARGETS): % : %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
