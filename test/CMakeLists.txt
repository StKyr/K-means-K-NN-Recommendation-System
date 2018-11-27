project(ergasiaTests)

add_subdirectory(lib/googletest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runUnitTests dummyTest.cpp testNDVector.cpp
        ../src/ApproximateNeighborSearch/NDVector.cpp
        ../src/ApproximateNeighborSearch/VectorCSVReader.cpp
        ../src/ApproximateNeighborSearch/metrics.cpp
        ../src/ApproximateNeighborSearch/NearestNeighborSearch.cpp
        ../src/ApproximateNeighborSearch/CosineSimilarityLSH.cpp
        ../src/ApproximateNeighborSearch/HashTable.cpp
        test_initializations.cpp
        test_assignments.cpp
        test_updating.cpp
        ../include/kmeans.hpp
        ../src/ApproximateNeighborSearch/CosineSimilarityLSH.cpp
        ../src/ApproximateNeighborSearch/EuclideanSpaceLSH.cpp
        ../src/ApproximateNeighborSearch/HashTable.cpp
        ../src/ApproximateNeighborSearch/HypercubeProjection.cpp
        ../src/ApproximateNeighborSearch/metrics.cpp
        ../src/ApproximateNeighborSearch/NDVector.cpp
        ../src/ApproximateNeighborSearch/NearestNeighborSearch.cpp
        ../src/initializing.cpp
        ../src/assigning.cpp
        ../src/updating.cpp
        ../src/Silhouette.cpp
        ../src/StoppingCriterion.cpp
        ../include/cluster.hpp
        test_assignments.cpp
        ../include/utils.hpp
        ../include/DistancesTable.h
        ../src/DistancesTable.cpp)


target_link_libraries(runUnitTests gtest gtest_main)




target_compile_definitions(runUnitTests PRIVATE DEVELOPMENT)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})