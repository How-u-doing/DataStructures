CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g

.PHONY: clean

AC_test: AC_test.cc AhoCorasick.h
	$(CXX) $(CXXFLAGS) -o $@ $<

format:
	clang-format -i AC_test.cc AhoCorasick.h

clean:
	rm AC_test
