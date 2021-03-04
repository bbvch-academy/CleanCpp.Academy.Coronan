[![Gitpod ready-to-code](https://img.shields.io/badge/Gitpod-ready--to--code-blue?logo=gitpod)](https://gitpod.io/#https://github.com/bbvch-academy/CleanCpp.Academy.Coronan)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)


# Documentation for Co[ro]nan

Co[ro]nan is an example C++17 project using [conan](https://conan.io/) and [CMake](https://cmake.org/).
It reads the current data of Corona (Covid-19) cases for a country from the _The About Corona Covid-19 API_. 

[The About Corona Covid-19](https://about-corona.net/documentation) provides statistics via REST API from The [World Health Organization Situation Reports](https://www.who.int/emergencies/diseases/novel-coronavirus-2019/situation-reports), [Johns Hopkins CSSE](https://coronavirus.jhu.edu/map.html), [The U.S. Department of Health & Human Services](https://www.hhs.gov/), [The National Health Commission of the People’s Republic of China](http://en.nhc.gov.cn/), [The European Centre for Disease Prevention and Control](https://www.ecdc.europa.eu/en), and [China CDC Weekly](http://weekly.chinacdc.cn/news/TrackingtheEpidemic.htm).

![Screenshot of the qt application](docs/images/Screenshot-qt.png)

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

### gitpod
For ease of use [gitpod.io](https://www.gitpod.io/) can be used for this project. Simply press the ![Gitpod ready-to-code](https://img.shields.io/badge/Gitpod-ready--to--code-blue?logo=gitpod) buttom on top of thid README to start coding. Check out the [gitpod docu](https://www.gitpod.io/docs/getting-started/) for getting started.