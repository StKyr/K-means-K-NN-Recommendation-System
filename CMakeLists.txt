cmake_minimum_required(VERSION 3.12)
project(ergasia1)

set(CMAKE_CXX_STANDARD 11)

#[[set(BOOST_ROOT "~/Apps/tools/boost_1_68_0")

find_package(Boost 1.68.0)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif()]]

include_directories(include src test /home/kyriakos/Apps/tools/boost_1_68_0)

add_executable(ergasia1 src/LSH.cpp include/NDVector.h src/NDVector.cpp src/VectorTSVReader.cpp include/VectorTSVReader.h src/DatasetReader.cpp include/DatasetReader.h include/metrics.h src/QuerysetReader.cpp include/QuerysetReader.h src/metrics.cpp src/NearestNeighborSearch.cpp include/NearestNeighborSearch.h include/EuclideanSpaceLSH.h src/EuclideanSpaceLSH.cpp src/CosineSimilarityLSH.cpp include/CosineSimilarityLSH.h src/HashTable.cpp include/HashTable.h include/LSH.h src/EuclideanHypercubeProjection.cpp include/EuclideanHypercubeProjection.h include/HypercubeProjection.h scr/HypercubeProjection.cpp)


target_link_libraries(ergasia1 /home/kyriakos/Apps/tools/boost_1_68_0/stage/lib/libboost_program_options.so)

add_subdirectory(test)





set(dev 1)

if (dev)

    target_compile_definitions(ergasia1 PRIVATE DEVELOPMENT)
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

endif(dev)
