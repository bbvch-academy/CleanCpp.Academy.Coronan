[![CI](https://github.com/bbvch-academy/CleanCpp.Academy.Coronan/actions/workflows/ci.yml/badge.svg)](https://github.com/bbvch-academy/CleanCpp.Academy.Coronan/actions/workflows/ci.yml)
[![Build Status](https://travis-ci.org/bbvch-academy/CleanCpp.Academy.Coronan.svg?branch=master)](https://travis-ci.org/bbvch-academy/CleanCpp.Academy.Coronan)
[![Build status](https://ci.appveyor.com/api/projects/status/gqqlo64h52e3eaml/branch/master?svg=true)](https://ci.appveyor.com/project/meshell/cleancpp-academy-coronan/branch/master)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

[![Open in Gitpod](https://gitpod.io/button/open-in-gitpod.svg)](https://gitpod.io/#https://github.com/bbvch-academy/CleanCpp.Academy.Coronan)

# Documentation for Co[ro]nan

Co[ro]nan is an example C++17 project using [conan](https://conan.io/) and [CMake](https://cmake.org/).
It reads the current data of Corona (Covid-19) cases for a country from the [The About Corona Covid-19 API](https://about-corona.net/documentation). 

_The About Corona Covid-19_ provides statistics via REST API from The [World Health Organization Situation Reports](https://www.who.int/emergencies/diseases/novel-coronavirus-2019/situation-reports), [Johns Hopkins CSSE](https://coronavirus.jhu.edu/map.html), [The U.S. Department of Health & Human Services](https://www.hhs.gov/), [The National Health Commission of the People’s Republic of China](http://en.nhc.gov.cn/), [The European Centre for Disease Prevention and Control](https://www.ecdc.europa.eu/en), and [China CDC Weekly](http://weekly.chinacdc.cn/news/TrackingtheEpidemic.htm).

![Screenshot of the qt application](docs/images/Screenshot-qt.png)

## Development

To build the project you need python and a recent [conan version](https://conan.io/):
```bash
#> pip install conan
```
Further a recent version of [CMake](https://cmake.org/) (> 3.10), a modern C++ compiler (C++17) and [Qt](https://www.qt.io/) with [QtCharts](https://doc.qt.io/qt-5/qtcharts-index.html) must be installed.

### How to build
To build the project in `build` execute:
```bash
#> ./build.sh build
```

See `./build.sh -h` for more options

You can also build it yourself:
```bash
#> mkdir build
#> (cd build && cmake ..)
#> cmake --build build
```
### Source Code formatting
For source code formatting [clang-format](https://clang.llvm.org/docs/ClangFormat.html) for C++ files and [cmake-format](https://pypi.org/project/cmake-format/) for the CMake files are used.

## Gitpod
To start right away without installing anything on you local machine, you can simply use the ![Open in Gitpod](https://gitpod.io/button/open-in-gitpod.svg) Button at the top of this README.
