#include <gtest/gtest.h>
#include <NDVector.h>
#include <unordered_map>
#include <metrics.h>
#include <NearestNeighborSearch.h>

TEST(testNN, single_point){

    NDVector p = {0,0,0};
    NDVector q = {1, 1, 1};

    std::unordered_map<std::string, NDVector> X;
    X["aa"] = p;

    std::pair<std::string, double> result = nearestNeighbor(q, X, metrics::euclidean_distance);

    EXPECT_EQ(result.first, "aa");
    EXPECT_EQ(result.second, q.norm());
}


TEST(testNN, many_points){

    NDVector q = {1,1,1};

    NDVector v1 = {10, 10, 10}
            ,v2 = {-10, -10, -10}
            ,v3 = {2, 2, 0}
            ,v4 = {100, 0, 0}
            ;

    std::unordered_map<std::string, NDVector> X = {{"a", v1}, {"b", v2}, {"c", v3}, {"d", v4} };

    std::pair<std::string, double> result = nearestNeighbor(q, X, metrics::euclidean_distance);

    EXPECT_EQ(result.first, "c");

    NDVector segment = q-v3;
    EXPECT_EQ(result.second, segment.norm());
}


TEST(testNN, many_points_equal){

    NDVector q = {1,1,1};

    NDVector v1 = {10, 10, 10}
    ,v2 = {-10, -10, -10}
    ,v3 = {2, 2, 0}
    ,v4 = {100, 0, 0}
    ;

    std::unordered_map<std::string, NDVector> X = {{"a", v1}, {"b", v2}, {"c", v3}, {"d", v4}, {"!!!", q} };

    std::pair<std::string, double> result = nearestNeighbor(q, X, metrics::euclidean_distance);

    EXPECT_EQ(result.first, "!!!");
    EXPECT_EQ(result.second, 0);
}

TEST(testNN, vectors_1D){

    NDVector q = {100};

    NDVector v1 = {10}
    ,v2 = {-10}
    ,v3 = {2}
    ,v4 = {101}
    ;

    std::unordered_map<std::string, NDVector> X = {{"a", v1}, {"b", v2}, {"c", v3}, {"d", v4}};

    std::pair<std::string, double> result = nearestNeighbor(q, X, metrics::euclidean_distance);

    EXPECT_EQ(result.first, "d");
    EXPECT_EQ(result.second, 1);
}

TEST(testNN, large_dimension){

    NDVector q = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

    NDVector v1 = 2*q
    ,v2 = -10*q
    ,v3 = 50*q
    ,v4 = -4*q
    ;

    std::unordered_map<std::string, NDVector> X = {{"a", v1}, {"b", v2}, {"c", v3}, {"d", v4}};

    std::pair<std::string, double> result = nearestNeighbor(q, X, metrics::euclidean_distance);

    EXPECT_EQ(result.first, "a");
    NDVector segment = q-v1;
    EXPECT_EQ(result.second, segment.norm() );
}