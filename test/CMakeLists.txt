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
        )

target_link_libraries(runUnitTests gtest gtest_main)




target_compile_definitions(runUnitTests PRIVATE DEVELOPMENT)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})