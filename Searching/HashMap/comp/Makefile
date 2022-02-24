CXX := g++
CXXFLAGS := -std=c++17 -Wall

TEST_OPT := $(patsubst %.cc, %, $(wildcard *.cc))
TEST_DBG := $(patsubst %.cc, %_d, $(wildcard *.cc))

TESTS := $(TEST_OPT) $(TEST_DBG)

.PHONY: all clean

all: $(TESTS)

# quick, dirty, lazy (overkill)
$(TEST_OPT): % : %.cc test.cc test_int.cc test_int_hash.cc
	$(CXX) $(CXXFLAGS) -O3 -o $@ $<

$(TEST_DBG): %_d : %.cc test.cc test_int.cc test_int_hash.cc
	$(CXX) $(CXXFLAGS) -g -o $@ $<

clean:
	rm -f $(TESTS)
