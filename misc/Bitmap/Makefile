CXX := g++
CXXFLAGS := -std=c++17 -Wall

.PHONY: clean

bitmap_test: bitmap_test.cc bitmap.h
	$(CXX) $(CXXFLAGS) -O3 -o $@ $<

clean:
	rm -f bitmap_test
