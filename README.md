[![Build Status](https://travis-ci.org/alext234/CppCircularBuffer.svg?branch=master)](https://travis-ci.org/alext234/CppCircularBuffer)

# CppCircularBuffer
A simple circular buffer in C++, with iterator supported. At the moment this is not thread-safe.
Following is a simple example:

```C++
#include "CircularBuffer.h"
#include <iostream>

    CircularBuffer<int, 10> cbuf;  // maximum size=10
    for (int i=1; i<15; ++i) {
        cbuf.push_back(i);
    }
   
    cbuf.pop_front();
    std::cout << cbuf.front() << std:endl;

    // iterator   
    for (auto it = cbuf.cbegin(); it!=cbuf.cend(); ++it) {
        std::cout << *it << std::endl;
    }

```
To build and run tests
```
cd googletest
mkdir build
cd build
cmake ..
make
cd ../..

mkdir build
cd build
cmake ..
make
./runtests
```
