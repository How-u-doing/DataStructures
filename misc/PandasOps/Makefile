CMAKE := cmake

.PHONY: all clean

release: build_type=Release
debug: build_type=Debug

release debug: requirements
	pybind11_DIR=$(shell python3 -c 'import pybind11; print(pybind11.get_cmake_dir())') \
	$(CMAKE) -B build -S . -DCMAKE_BUILD_TYPE=$(build_type)
	$(MAKE) -C build -j$(shell nproc)

requirements:
	pip3 install pybind11

clean:
	rm -rf build/
