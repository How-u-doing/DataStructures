CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra

.PHONY: clean

rolling_max_test: rolling_max_test.cc rolling_min_max.h
	$(CXX) $(CXXFLAGS) -O2 -o $@ $^

rolling_max_bench:
	python3 rolling_max.py

clean:
	rm -f rolling_max_test
