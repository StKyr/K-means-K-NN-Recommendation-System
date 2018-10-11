project(ergasiaTests)

add_subdirectory(lib/googletest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runUnitTests dummyTest.cpp testNDVector.cpp ../src/NDVector.cpp ../src/VectorTSVReader.cpp ../include/VectorTSVReader.h ../src/DatasetReader.cpp ../include/DatasetReader.h ../include/metrics.h ../src/QuerysetReader.cpp ../include/QuerysetReader.h ../src/metrics.cpp ../src/NearestNeighborSearch.cpp ../include/NearestNeighborSearch.h testNN.cpp)

target_link_libraries(runUnitTests gtest gtest_main)
