# Utils

TODO: Documentation

## Compiling

``` bash
# mkdir build
# cd build
# cmake -DCMAKE_BUILD_TYPE=Release ..
# make
```

## Unit Tests

We have two different types of unit tests: *memcheck* and *threads*
*memcheck* check the funtionality of the code and searches for memory leaks and such things.
*threads* is intendes to chekck for race conditions in the code.

To start both types of checks:

``` bash
cd build
make check
```

To start only the *memcheck*s:

``` bash
cd build
make check_memcheck
```

To start only the *threads* checks:

``` bash
cd build
make check_threads
```
