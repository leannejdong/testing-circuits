# Testing repo for autocircuit

* The purpose is to run a characteristic test to confirm results havn't changed

* If you change the input data the results will change as well

* The test here is to make sure tension2 is correct

* the point of having the test is so that if we make a mistake in changing tension2 it can be caught quickly


## Build

### Dependency required

- CMake = 3.16

- Clang >= 10.0 (the use of `auto` in parameter declartion only available with '-fconcepts')

- Eigen3.3

### Linux

`clang++-10 -std=c++2a tension_test.cpp tension.cpp -Wall -Wextra  -o testtension -isystem /usr/include/eigen3`

### Windows (CLion)

Open the project directory and hit `shift-f10`

