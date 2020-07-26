build:
	mkdir build 	

build/CMakeCache.txt: build
	cd build && cmake -DCMAKE_BUILD_TYPE=Debug ..

.PHONY: delete_build
delete_build:
	rm -rf build

.SILENT:
%: build/CMakeCache.txt
	make -C build --no-print-directory $@ 
