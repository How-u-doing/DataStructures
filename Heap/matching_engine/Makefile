CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra

.PHONY: all clean

all: matching_engine P1

matching_engine: src/main.cc
	$(CXX) $(CXXFLAGS) -O2 -o $@ $^

P1: matching_engine
	FF_DOS=1 ./matching_engine 1 < data/orders.csv > res1.csv
	python3 verify.py

debug: matching_engine
	FF_DOS=1 ./matching_engine 1 < testdata.csv > res1.csv
	python3 verify.py

clean:
	rm -f *.o matching_engine res*.csv
