cmake_minimum_required(VERSION 3.12)
project(ergasia1)

set(CMAKE_CXX_STANDARD 11)

include_directories(include src test)

add_executable(ergasia1 src/main.cpp include/NDVector.h src/NDVector.cpp src/VectorTSVReader.cpp src/VectorTSVReader.h src/DatasetReader.cpp src/DatasetReader.h include/metrics.h)

add_subdirectory(test)
