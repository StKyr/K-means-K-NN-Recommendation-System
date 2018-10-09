
#include <gtest/gtest.h>
#include <NDVector.h>

TEST(testNDVector, checkInitialization1) {
    std::vector<double > xs = {1, 2, 3};

    NDVector v = NDVector(xs);

    for (int i=0; i<xs.size(); i++){
        EXPECT_EQ(v.coords[i], xs[i]);
    }
}

TEST(testNDVector, checkInitialization2) {
    std::vector<double > xs = {1, 2, 3};

    NDVector v = NDVector({1, 2, 3});

    for (int i=0; i<xs.size(); i++){
        EXPECT_EQ(v.coords[i], xs[i]);
    }
}


TEST(testNDVector, testDimensions) {
    NDVector v1 = NDVector({1}),
            v2 = NDVector({1, 2}),
            v3 = NDVector({1, 2, 3}),
            v4 = NDVector({1, 1, 1, 1, 1, 1, 1, 1, 1, 1});

    EXPECT_EQ(v1.dim(), 1);
    EXPECT_EQ(v2.dim(), 2);
    EXPECT_EQ(v3.dim(), 3);
    EXPECT_EQ(v4.dim(), 10);
}

TEST(testNDVector, testEmptyDimension) {
    NDVector v1 = NDVector();

    try {
        v1.dim();
        FAIL() << "Exception was not thrown on empty vector.";
    }catch (std::range_error& e){
        EXPECT_EQ(e.what(), std::string("Unitialized Vector Exception"));
    }catch(...){
        FAIL() << "Different type of exception was thrown.";
    }

}


