cmake_minimum_required(VERSION 3.12)
project(ergasia1)

set(CMAKE_CXX_STANDARD 11)

include_directories(include src test)

add_executable(ergasia1 src/main.cpp include/NDVector.h src/NDVector.cpp src/VectorTSVReader.cpp include/VectorTSVReader.h src/DatasetReader.cpp include/DatasetReader.h include/metrics.h src/QuerysetReader.cpp include/QuerysetReader.h src/metrics.cpp)

add_subdirectory(test)
