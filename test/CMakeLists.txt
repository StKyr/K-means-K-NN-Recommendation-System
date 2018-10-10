project(ergasiaTests)

add_subdirectory(lib/googletest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runUnitTests dummyTest.cpp testNDVector.cpp ../src/NDVector.cpp)

target_link_libraries(runUnitTests gtest gtest_main)
