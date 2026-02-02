CMAKE := cmake
RUST_LIB := lib/libselect_nth_unstable.a
RUST_SRC := src/select_nth_unstable.rs

.PHONY: all clean

release: build_type=Release
debug: build_type=Debug

release debug: requirements
	pybind11_DIR=$(shell python3 -c 'import pybind11; print(pybind11.get_cmake_dir())') \
	$(CMAKE) -B build -S . -DCMAKE_BUILD_TYPE=$(build_type)
	$(MAKE) -C build -j$(shell nproc)

requirements: $(RUST_LIB)
	pip3 install pybind11

$(RUST_LIB): $(RUST_SRC)
	mkdir -p lib
	rustc -C opt-level=3 $< --crate-type staticlib -o $@

clean:
	rm -rf build/
