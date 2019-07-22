# GOC
A (Runner-Kaleidoscope compatible) CPP framework for Combinatorial Optimization research.

## Description
Some common tasks are done in many algorithms for solving Combinatorial Optimization problems. Generally, Mixed Integer Linear Programming _(MILP)_ is used to deal with such cases. Dealing with MILP solvers some times is not a straightforward task, and it can add some noise to the actual code that is needed to solve the problem.

GOC is a C++ library that comes with classes and functions to ease the process of writing code for MILP. Its main contributions are the following:
- Abstraction from a specific MILP Solver by definitions of model and solver interfaces.
  - Formulation, Expression, Constraint, Variable and Valuation classes.
  - LPSolver, BCSolver, CGSolver, CutStrategy, SeparationRoutine interfaces.
  - Implementation of LPSolver and BCSolver interfaces for CPLEX 12.9.
  - Column generation implementation.
- Standardization of the output of solvers and compatibility with [Kaleidoscope](https://github.com/gleraromero/kaleidoscope) formats.
- Some collection util functions such as printing and JSON serialization functions for STDLIB.
  - New Matrix collection.
- Graph and Digraph classes, designed for a trade-off between performance and cleanness of code.
  - Path finding algorithms (dijkstra, bellman ford, floyd).
  - Max-flow Min-Cut algorithm using the [Boost Graph Library](https://www.boost.org/doc/libs/1_66_0/libs/graph/doc/index.html).
- Math related objects: Interval, Linear Function, Piecewise Linear Function, Point2D.
  - epsilon-comparison functions (to see if two values are near given a certain precision).
- String util functions
  - contains, split, trim, ...
  - STR(obj) macro which converts any object to a string using the \<\<operator()
- Time handling functions and classes
  - Date, Duration, PointInTime.
  - Stopwatch, Watch, Timer.

## Getting started
The following instructions will guide you through the steps to have GOC compiled and ready to use.

### Prerequisites
- CPLEX >12.8 [(more info)](https://www.ibm.com/products/ilog-cplex-optimization-studio)
- Boost Graph Library >1.66 [(more info)](https://www.boost.org/doc/libs/1_66_0/libs/graph/doc/index.html)
- CMake >2.8.4 [(more info)](https://cmake.org/)
- C++14 or higher [(more info)](https://es.wikipedia.org/wiki/C%2B%2B14)

### Compiling GOC
1. Download the [code](code) folder.
1. Add two environment variables to bash with CPLEX include and library paths.
    1. ```export CPLEX_INCLUDE=<path_to_cplex_include_dir>```
    1. ```export CPLEX_BIN=<path_to_cplex_lib_binary_file>```
1. Run: ```cmake .```
1. Run ```make```
1. A compiled library file __libgoc.a__ should be generated.

> Note: the environment variables can be added permanently by adding those lines to the /etc/environment file.

> Note: CPLEX usually has its root at _/opt/ibm/ILOG/CPLEX_Studio\<version\>/cplex/_.

### Including GOC in your project (using CMake).
1. Add a _lib/goc/_ directory to your project.
1. Copy the _include_ folder of GOC to the _lib/goc_ directory.
1. Copy the __libgoc.a__ file to the _lib/goc/_ directory.
1. Add the include folder to your CMakeLists.txt file 
    1. ```include_directories(lib/goc)```
1. Link the goc library to your executables in CMakeLists.txt 
    1. ```target_link_libraries(<executable_name> lib/goc/libgoc.a)```
1. Include goc in your C++ files using
    1. ```#include <goc/goc.h>```
1. GOC library is ready to use.

## Built With
* [JSON for Modern C++](https://github.com/nlohmann/json)

## Authors
Gonzalo Lera-Romero

## License
This project is licensed under the MIT License - see the LICENSE.md file for details
