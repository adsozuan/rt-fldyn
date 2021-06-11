# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_PREFIX_PATH c:/Libs)

project ("rt-fldyn")

# Include sub-projects.
add_subdirectory ("src/basic")

