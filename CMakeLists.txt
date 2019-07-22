cmake_minimum_required(VERSION 2.8.4)
project(example)

# Check if CPLEX environment variables are set.
if("$ENV{CPLEX_INCLUDE}" STREQUAL "")
    message(SEND_ERROR "CPLEX_INCLUDE environment variable is not set. Make sure to point the variable to the CPLEX include directory before compiling.\nTo do this run on the terminal (for example):\nexport CPLEX_INCLUDE=/opt/ibm/ILOG/CPLEX_Studio129/cplex/include\nThis can be added permanently by adding the export command to the file /etc/environment and rebooting (on Linux).")
endif()
if("$ENV{CPLEX_BIN}" STREQUAL "")
    message(SEND_ERROR "CPLEX_BIN environment variable is not set. Make sure to point the variable to the CPLEX binary (.a) before compiling.\nTo do this run on the terminal (for example):\nexport CPLEX_BIN=/opt/ibm/ILOG/CPLEX_Studio129/cplex/lib/x86-64_linux/static_pic/libcplex.a\nThis can be added permanently by adding the export command to the file /etc/environment and rebooting (on Linux).")
endif()

# Including GOC.
add_subdirectory(code)
include_directories(code/include)

# Including CPLEX.
include_directories($ENV{CPLEX_INCLUDE})

# Set compiler flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++14 -m64 -O2 -DNDEBUG")

# Create executable files.
add_executable(lp_example examples/lp_example.cpp)
target_link_libraries(lp_example goc)
target_link_libraries(lp_example $ENV{CPLEX_BIN} -ldl -lm)

add_executable(bb_example examples/bb_example.cpp)
target_link_libraries(bb_example goc)
target_link_libraries(bb_example $ENV{CPLEX_BIN} -ldl -lm)

add_executable(lazy_constraint_example examples/lazy_constraint_example.cpp)
target_link_libraries(lazy_constraint_example goc)
target_link_libraries(lazy_constraint_example $ENV{CPLEX_BIN} -ldl -lm)

add_executable(cut_example examples/cut_example.cpp)
target_link_libraries(cut_example goc)
target_link_libraries(cut_example $ENV{CPLEX_BIN} -ldl -lm)

add_executable(colgen_example examples/colgen_example.cpp)
target_link_libraries(colgen_example goc)
target_link_libraries(colgen_example $ENV{CPLEX_BIN} -ldl -lm)