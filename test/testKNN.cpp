#include <gtest/gtest.h>
#include <NDVector.h>
#include <unordered_map>
#include <metrics.h>
#include <NearestNeighborSearch.h>


TEST(testKNN, case1){

    NDVector q = {1,1,1};

    NDVector v1 = {10, 10, 10}
    ,v2 = {-10, -10, -10}
    ,v3 = {2, 2, 0}
    ,v4 = {1, 1, 0}
    ;

    std::unordered_map<std::string, NDVector> X = {{"a", v1}, {"b", v2}, {"c", v3}, {"d", v4} };

    std::vector<std::string> results = k_nearestNeighbors(q,2,X, metrics::euclidean_distance);

    EXPECT_EQ(std::find(results.begin(), results.end(), "d") != results.end(), true);
    EXPECT_EQ(std::find(results.begin(), results.end(), "c") != results.end(), true);

}



TEST(testKNN, exact_match){

    NDVector q = {1,1,1};

    NDVector v1 = {10, 10, 10}
    ,v2 = {-10, -10, -10}
    ,v3 = {2, 2, 0}
    ,v4 = {1, 1, 0}
    ;

    std::unordered_map<std::string, NDVector> X = {{"a", v1}, {"b", v2}, {"c", v3}, {"d", v4}, {"q", q} };

    std::vector<std::string> results = k_nearestNeighbors(q,2,X, metrics::euclidean_distance);

    EXPECT_EQ(results[0], "q");
    EXPECT_EQ(results[1], "d");

}


TEST(testKNN, invalid_k_1){

    NDVector q = {1,1,1};

    NDVector v1 = {10, 10, 10}
    ,v2 = {-10, -10, -10}
    ,v3 = {2, 2, 0}
    ,v4 = {1, 1, 0}
    ;

    std::unordered_map<std::string, NDVector> X = {{"a", v1}, {"b", v2}, {"c", v3}, {"d", v4}, {"q", q} };

    try {
        std::vector<std::string> results = k_nearestNeighbors(q,6,X, metrics::euclidean_distance);
        FAIL() << "Exception was not thrown on empty vector.";
    }catch (std::logic_error& e){
        EXPECT_EQ(e.what(), std::string("kNN called with illegal k value"));
    }catch(...){
        FAIL() << "Different type of exception was thrown.";
    }

}


TEST(testKNN, invalid_k_2){

    NDVector q = {1,1,1};

    NDVector v1 = {10, 10, 10}
    ,v2 = {-10, -10, -10}
    ,v3 = {2, 2, 0}
    ,v4 = {1, 1, 0}
    ;

    std::unordered_map<std::string, NDVector> X = {{"a", v1}, {"b", v2}, {"c", v3}, {"d", v4}, {"q", q} };

    try {
        std::vector<std::string> results = k_nearestNeighbors(q,-1,X, metrics::euclidean_distance);
        FAIL() << "Exception was not thrown on empty vector.";
    }catch (std::logic_error& e){
        EXPECT_EQ(e.what(), std::string("kNN called with illegal k value"));
    }catch(...){
        FAIL() << "Different type of exception was thrown.";
    }
}
