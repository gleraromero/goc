# GOC
A (Runner-Kaleidoscope compatible) C++ framework for Combinatorial Optimization research.

## Description
Some common tasks are done in many algorithms for solving Combinatorial Optimization problems. Generally, Mixed Integer Linear Programming _(MILP)_ is used to deal with such cases. Dealing with MILP solvers some times is not a straightforward task, and it can add some noise to the actual code that is needed to solve the problem.

GOC is a C++ library that comes with classes and functions to ease the process of writing code for MILP. Its main contributions are the following:
- Abstraction from a specific MILP Solver by definitions of model and solver interfaces.
- Standardization of the output of solvers and compatibility with [Kaleidoscope](https://github.com/gleraromero/kaleidoscope) formats.
- Some collection util functions such as printing and JSON serialization functions for STDLIB.
- Graph and Digraph classes, designed for a trade-off between performance and cleanness of code.
- Math related objects: Interval, Linear Function, Piecewise Linear Function, Point2D, etc.
- String resources.
- Time handling functions and classes.

## Documentation
To view the full documentation of the library visit the [Wiki](https://github.com/gleraromero/goc/wiki).

## Getting started
The following instructions will guide you through the steps to have GOC compiled and ready to use.

### Prerequisites
- CPLEX >=12.8 [(more info)](https://www.ibm.com/products/ilog-cplex-optimization-studio)
- Boost Graph Library >=1.66 [(more info)](https://www.boost.org/doc/libs/1_66_0/libs/graph/doc/index.html)
    - On Linux: ```sudo apt-get install libboost-all-dev```
- CMake >=2.8.4 [(more info)](https://cmake.org/)
    - On Linux: ```sudo apt-get install cmake```
- C++14 or higher [(more info)](https://es.wikipedia.org/wiki/C%2B%2B14)

### Compiling GOC
1. Download the [code](code) folder.
1. Add two environment variables to bash with CPLEX include and library paths.
    1. ```export CPLEX_INCLUDE=<path_to_cplex_include_dir>```
        - Usually on Linux: _/opt/ibm/ILOG/CPLEX_Studio\<VERSION\>/cplex/include_
    1. ```export CPLEX_BIN=<path_to_cplex_lib_binary_file>```
        - Usually on Linux: _/opt/ibm/ILOG/CPLEX_Studio\<VERSION\>/cplex/lib/x86-64_linux/static_pic/libcplex.a_
1. Add two environment variables to bash with BOOST Graph Library include and library paths.
    1. ```export BOOST_INCLUDE=<path_to_boost_include_dir>```
        - Usually on Linux: _/usr/include_
    1. ```export BOOST_BIN=<path_to_boost_lib_binary_file>```
        - Usually on Linux: _/usr/lib/x86_64-linux-gnu/libboost_graph.a_
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

## Examples
We include a series of examples on the usage of the different solvers with their different options. The examples include:
- **Fractional Knapsack**: Example of solving the fractional knapsack using a Linear Programming solver.
- **0-1 Knapsack**: Example of solving the 0-1 knapsack using a Branch a Bound solver.
- **Traveling Salesman Problem**: Example of solving the TSP using a branch and bound solver with lazy constraints.
- **Vertex Coloring Problem (BC)**: Example of solving the vertex coloring problem using a branch and cut algorithm.
- **Vertex Coloring Problem (CG)**: Example of an algorithm to get a lower bound on the chromatic number of a graph using column generation.

### Running examples
1. Download GOC repository.
1. Add two environment variables to bash with CPLEX include and library paths.
    1. ```export CPLEX_INCLUDE=<path_to_cplex_include_dir>```
    1. ```export CPLEX_BIN=<path_to_cplex_lib_binary_file>```
1. Run ```cmake .``` on the root directory.
1. Run ```make```.
1. Run the example executable.

## Built With
* [JSON for Modern C++](https://github.com/nlohmann/json)
* [Boost Graph Library](https://www.boost.org/doc/libs/1_66_0/libs/graph/doc/index.html)

## Authors
Gonzalo Lera-Romero

## License
This project is licensed under the MIT License - see the LICENSE.md file for details
