CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra

.PHONY: clean all

TESTS := Bitmap_test BloomFilter_test0 BloomFilter_test

all: $(TESTS)

Bitmap_test: Bitmap_test.cc Bitmap.h
	$(CXX) $(CXXFLAGS) -O3 -o $@ $<

BloomFilter_test0: BloomFilter_test0.cc BloomFilter.h Bitmap.h
	$(CXX) $(CXXFLAGS) -g -o $@ $<

BloomFilter_test: BloomFilter_test.cc BloomFilter.h Bitmap.h
	$(CXX) $(CXXFLAGS) -O3 -o $@ $<

clean:
	rm -f $(TESTS)
