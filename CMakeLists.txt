cmake_minimum_required(VERSION 3.12)
project(ergasia2)

set(CMAKE_CXX_STANDARD 11)

include_directories(include libs/boost_1_67_0/stage/lib)

add_executable(ergasia2
        include/ApproximateNeighborSearch/AbstractSimilaritySearch.h
        include/ApproximateNeighborSearch/CosineSimilarityLSH.h
        include/ApproximateNeighborSearch/EuclideanSpaceLSH.h
        include/ApproximateNeighborSearch/HashTable.h
        include/ApproximateNeighborSearch/HypercubeProjection.h
        include/kmeans.hpp
        include/ApproximateNeighborSearch/metrics.h
        include/ApproximateNeighborSearch/NDVector.h
        include/ApproximateNeighborSearch/NearestNeighborSearch.h
        include/ApproximateNeighborSearch/similaritySearch.h
        include/ApproximateNeighborSearch/VectorCSVReader.h
        src/ApproximateNeighborSearch/CosineSimilarityLSH.cpp
        src/ApproximateNeighborSearch/EuclideanSpaceLSH.cpp
        src/ApproximateNeighborSearch/HashTable.cpp
        src/ApproximateNeighborSearch/HypercubeProjection.cpp
        src/ApproximateNeighborSearch/metrics.cpp
        src/ApproximateNeighborSearch/NDVector.cpp
        src/ApproximateNeighborSearch/NearestNeighborSearch.cpp include/initializing.h
        src/initializing.cpp
        src/assigning.cpp
        include/assigning.h
        src/updating.cpp
        include/updating.h
        src/Silhouette.cpp
        include/Silhouette.h
        src/ApproximateNeighborSearch/VectorCSVReader.cpp
        src/clustering.cpp
        src/StoppingCriterion.cpp
        include/StoppingCriterion.h
        include/cluster.hpp include/utils.hpp
        include/DistancesTable.h
        src/DistancesTable.cpp src/parameters.cpp src/parameters.hpp)


add_subdirectory(test)


LINK_DIRECTORIES(libs/boost_1_67_0/stage/lib)
target_link_libraries(ergasia2 libboost_program_options.so)
