CMAKE := cmake

.PHONY: all clean

all: requirements
	rustc -C opt-level=3 src/select_nth_unstable.rs --crate-type staticlib
	pybind11_DIR=$(shell python3 -c 'import pybind11; print(pybind11.get_cmake_dir())') \
	$(CMAKE) -B build -S . -DCMAKE_BUILD_TYPE=Release
	$(MAKE) -C build

requirements:
	pip3 install pybind11 numpy polars

clean:
	rm -rf build/ *.a
