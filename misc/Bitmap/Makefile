CXX := g++
CXXFLAGS := -std=c++17 -Wall

.PHONY: clean

bitmap_test: bitmap_test.cc bitmap.h
	$(CXX) $(CXXFLAGS) -g -o $@ $<

clean:
	rm -f bitmap_test
