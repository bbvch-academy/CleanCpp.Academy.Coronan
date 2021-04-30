# Documentation for Co[ro]nan

Co[ro]nan is an example C++17 project using [conan](https://conan.io/) and [CMake](https://cmake.org/).
It reads the current data of Corona (Covid-19) cases for a country from the _[The About Corona Covid-19 API](https://about-corona.net/documentation).

_The About Corona Covid-19_ provides statistics via REST API from The [World Health Organization Situation Reports](https://www.who.int/emergencies/diseases/novel-coronavirus-2019/situation-reports), [Johns Hopkins CSSE](https://coronavirus.jhu.edu/map.html), [The U.S. Department of Health & Human Services](https://www.hhs.gov/), [The National Health Commission of the People’s Republic of China](http://en.nhc.gov.cn/), [The European Centre for Disease Prevention and Control](https://www.ecdc.europa.eu/en), and [China CDC Weekly](http://weekly.chinacdc.cn/news/TrackingtheEpidemic.htm).

## Development

To build the project you need python and [conan (version 1.24 or newer)](https://conan.io/):
```bash
#> pip install conan
```
Further a recent version of [CMake](https://cmake.org/) (> 3.10), a modern C++ compiler (C++17) and [Qt](https://www.qt.io/) with [QtCharts](https://doc.qt.io/qt-5/qtcharts-index.html) must be installed.

### How to build
First time to install conan packages and build the project in `build` execute:
```bash
#> ./build.sh --init build
```
After that you can use
```bash
#> ./build.sh build
```

See `./build.sh -h` for more options

You can also build it yourself:
```bash
#> mkdir build
#> (cd build && conan install --build poco --build missing ..)
#> (cd build && cmake ..)
#> cmake --build build
```

### Source Code formatting
For source code formatting [clang-format](https://clang.llvm.org/docs/ClangFormat.html) for C++ files and [cmake-format](https://pypi.org/project/cmake-format/) for the CMake files are used.

## LICENCE

MIT License

Copyright (c) 2020 bbv Software Services AG

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
