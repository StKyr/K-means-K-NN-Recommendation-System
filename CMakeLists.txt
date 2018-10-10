cmake_minimum_required(VERSION 3.12)
project(ergasia1)

set(CMAKE_CXX_STANDARD 11)

include_directories(include src test)

add_executable(ergasia1 src/main.cpp include/NDVector.h src/NDVector.cpp)

add_subdirectory(test)
