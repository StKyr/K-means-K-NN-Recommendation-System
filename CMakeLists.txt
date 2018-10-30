cmake_minimum_required(VERSION 3.12)
project(ergasia1)

set(CMAKE_CXX_STANDARD 11)

#[[set(BOOST_ROOT "~/Apps/tools/boost_1_68_0")

find_package(Boost 1.68.0)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif()]]

include_directories(include src test /home/kyriakos/Apps/tools/boost_1_68_0)

add_executable(lsh src/similaritySearch.cpp include/NDVector.h
        src/NDVector.cpp
        src/VectorTSVReader.cpp
        include/VectorTSVReader.h
        include/metrics.h
        src/metrics.cpp
        src/NearestNeighborSearch.cpp
        include/NearestNeighborSearch.h
        include/EuclideanSpaceLSH.h
        src/EuclideanSpaceLSH.cpp
        src/CosineSimilarityLSH.cpp
        include/CosineSimilarityLSH.h
        src/HashTable.cpp
        include/HashTable.h
        include/similaritySearch.h
        include/AbstractSimilaritySearch.h)
target_link_libraries(lsh /home/kyriakos/Apps/tools/boost_1_68_0/stage/lib/libboost_program_options.so)
target_compile_definitions(lsh PRIVATE RUN_LSH)

#[[
add_executable(euclidean src/similaritySearch.cpp include/NDVector.h
        src/NDVector.cpp
        src/VectorTSVReader.cpp
        include/VectorTSVReader.h
        include/metrics.h
        src/metrics.cpp
        src/NearestNeighborSearch.cpp
        include/NearestNeighborSearch.h
        include/EuclideanSpaceLSH.h
        src/EuclideanSpaceLSH.cpp
        src/HashTable.cpp
        include/HashTable.h
        include/similaritySearch.h
        include/AbstractSimilaritySearch.h)
target_link_libraries(euclidean /home/kyriakos/Apps/tools/boost_1_68_0/stage/lib/libboost_program_options.so)
]]

add_executable(hypercube src/similaritySearch.cpp include/NDVector.h
        src/NDVector.cpp
        src/VectorTSVReader.cpp
        include/VectorTSVReader.h
        include/metrics.h
        src/metrics.cpp
        src/NearestNeighborSearch.cpp
        include/NearestNeighborSearch.h
        include/EuclideanSpaceLSH.h
        src/EuclideanSpaceLSH.cpp
        src/CosineSimilarityLSH.cpp
        include/CosineSimilarityLSH.h
        src/HashTable.cpp
        include/HashTable.h
        include/similaritySearch.h
        include/HypercubeProjection.h
        src/HypercubeProjection.cpp include/AbstractSimilaritySearch.h)
target_link_libraries(hypercube /home/kyriakos/Apps/tools/boost_1_68_0/stage/lib/libboost_program_options.so)
target_compile_definitions(hypercube PRIVATE RUN_RPH)


add_subdirectory(test)





set(dev 1)

if (dev)
    target_compile_definitions(lsh PRIVATE DEVELOPMENT)
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

endif(dev)
