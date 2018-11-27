#include <gtest/gtest.h>
#include <ApproximateNeighborSearch/NDVector.h>
#include <ApproximateNeighborSearch/VectorCSVReader.h>
#include <cluster.hpp>
#include <assigning.h>
#include <DistancesIndex.h>
#include <utils.hpp>


TEST(assign, single_assignment){

    DistancesIndex::getInstance().initialize(metrics::euclidean_distance);

    Dataset X;
    X["id0"] = NDVector({1,2,3});
    X["id1"] = NDVector({4,5,3});
    X["id2"] = NDVector({111,2,3});
    X["id3"] = NDVector({1,222,3});
    X["id4"] = NDVector({1,2,334});
    X["id5"] = NDVector({12,23,3});
    X["id6"] = NDVector({111,2,322});
    X["id7"] = NDVector({1,2,353});
    X["id8"] = NDVector({12,00,3});
    X["id9"] = NDVector({1,1,1});


    std::vector<Cluster> clusters;
    auto v = NDVector::random_vector(3);
    clusters.push_back(Cluster(v));

    LloydAssignment assignment;
    assignment(X, clusters);

    EXPECT_EQ(clusters[0].num_points(), X.size());
}

TEST(assign, empty_single_assignment){
    DistancesIndex::getInstance().initialize(metrics::euclidean_distance);
    Dataset EMPTY;


    std::vector<Cluster> clusters;
    auto v = NDVector::random_vector(3);
    clusters.push_back(Cluster(v));

    LloydAssignment assignment;
    assignment(EMPTY, clusters);
    EXPECT_EQ(clusters[0].num_points(), 0);
}


TEST(assign, two_clustres_single_point){
    DistancesIndex::getInstance().initialize(metrics::euclidean_distance);

    Dataset X;
    X["id0"] = NDVector({99,99,99});


    std::vector<Cluster> clusters;
    NDVector c1({0,0,0});
    NDVector c2({100,100,100});

    clusters.push_back(Cluster(c1));
    clusters.push_back(Cluster(c2));

    LloydAssignment assignment;
    assignment(X, clusters);

    EXPECT_EQ(clusters[0].num_points(),0);
    EXPECT_EQ(clusters[1].num_points(),1);
}

TEST(assign, two_same_centroids){
    DistancesIndex::getInstance().initialize(metrics::euclidean_distance);

    Dataset X;
    for (int i=0; i<50; i++) X[str(i)] = NDVector::random_vector(3);


    std::vector<Cluster> clusters;
    NDVector c1 = NDVector::random_vector(3);

    clusters.push_back(Cluster(c1));
    clusters.push_back(Cluster(c1));

    LloydAssignment assignment;
    assignment(X, clusters);

    EXPECT_TRUE(clusters[0].num_points() == 0
              || clusters[1].num_points() == 0);
}

TEST(assign, two_same_centroids_cosine){

    DistancesIndex::getInstance().initialize(metrics::cosine_distance);

    Dataset X;
    for (int i=0; i<50; i++) X[str(i)] = NDVector::random_vector(3);


    std::vector<Cluster> clusters;
    NDVector c1 = NDVector::random_vector(3);

    clusters.push_back(Cluster(c1));
    clusters.push_back(Cluster(c1));

    LloydAssignment assignment;
    assignment(X, clusters);

    EXPECT_TRUE(clusters[0].num_points() == 0
                || clusters[1].num_points() == 0);
}






TEST(assign, three_clustres_easy){
    DistancesIndex::getInstance().initialize(metrics::euclidean_distance);
    Dataset X;
    X["1_1"] = NDVector({1,2,3});
    X["1_2"] = NDVector({4,5,6});
    X["1_3"] = NDVector({-2,-2,-3});

    X["2_1"] = NDVector({200,222,100});
    X["2_2"] = NDVector({1000,2000,3340});

    X["3_1"] = NDVector({-30,-30,-30});

    std::vector<Cluster> clusters;
    NDVector c1({0,0,0});
    NDVector c2({100,100,100});
    NDVector c3({-50,-50,-50});

    clusters.push_back(Cluster(c1));
    clusters.push_back(Cluster(c2));
    clusters.push_back(Cluster(c3));

    LloydAssignment assignment;
    assignment(X, clusters);

    EXPECT_EQ(clusters[0].num_points(),3);
    EXPECT_EQ(clusters[1].num_points(),2);
    EXPECT_EQ(clusters[2].num_points(),1);

    EXPECT_EQ(vec2set(clusters[0].get_points()), std::set<VectorId>({"1_1","1_2", "1_3"}));
    EXPECT_EQ(vec2set(clusters[1].get_points()), std::set<VectorId>({"2_1","2_2"}));
    EXPECT_EQ(vec2set(clusters[2].get_points()), std::set<VectorId>({"3_1"}));
}

TEST(assign, three_clustres_easy_cosine){
    DistancesIndex::getInstance().initialize(metrics::cosine_distance);

    Dataset X;
    X["1_1"] = NDVector({100,100,100});
    X["1_2"] = NDVector({ 10,  9, 10});
    X["1_3"] = NDVector({ 80, 70,120});

    X["2_1"] = NDVector({-3,-3,-3});
    X["2_2"] = NDVector({-100,-500,-1000});

    X["3_1"] = NDVector({700, 0.1, -0.1});

    std::vector<Cluster> clusters;
    NDVector c1({ 1, 1, 1});
    NDVector c2({-1,-1,-1});
    NDVector c3({ 1, 0, 0});

    clusters.push_back(Cluster(c1));
    clusters.push_back(Cluster(c2));
    clusters.push_back(Cluster(c3));

    LloydAssignment assignment;
    assignment(X, clusters);

    EXPECT_EQ(clusters[0].num_points(),3);
    EXPECT_EQ(clusters[1].num_points(),2);
    EXPECT_EQ(clusters[2].num_points(),1);

    EXPECT_EQ(vec2set(clusters[0].get_points()), std::set<VectorId>({"1_1","1_2", "1_3"}));
    EXPECT_EQ(vec2set(clusters[1].get_points()), std::set<VectorId>({"2_1","2_2"}));
    EXPECT_EQ(vec2set(clusters[2].get_points()), std::set<VectorId>({"3_1"}));
}