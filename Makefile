build:
	mkdir build 	

build/CMakeCache.txt: build
	cd build && cmake ..

.PHONY: delete_build
delete_build:
	rm -rf build

.SILENT:
%: build/CMakeCache.txt
	make -C build --no-print-directory $@ 
