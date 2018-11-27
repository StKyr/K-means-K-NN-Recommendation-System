#include <ApproximateNeighborSearch/NDVector.h>
#include <ApproximateNeighborSearch/VectorCSVReader.h>
#include <initializing.h>
#include <gtest/gtest.h>
#include <updating.h>
#include <DistancesIndex.h>
#include <utils.hpp>

TEST(test_updating, simple_test_kmeans ) {

    Dataset X;
    X["id0"] = NDVector({2,2});
    X["id1"] = NDVector({0,0});
    X["id2"] = NDVector({0,2});
    X["id3"] = NDVector({2,0});

    KMeansUpdate update(2);
    std::vector<Cluster> clusters;
    auto v = NDVector::random_vector(2);
    clusters.push_back(Cluster(v));
    clusters[0].assign("id0");
    clusters[0].assign("id1");
    clusters[0].assign("id2");
    clusters[0].assign("id3");
    update(X, clusters);


    EXPECT_EQ(clusters[0].get_centroid(), NDVector({1,1}));

}

TEST(test_updating, empty_cluster_kmeans ) {

    Dataset EMPTY;

    KMeansUpdate update(2);
    std::vector<Cluster> clusters;
    auto v = NDVector::random_vector(2);
    clusters.push_back(Cluster(v));
    update(EMPTY, clusters);


    EXPECT_EQ(clusters[0].get_centroid(), NDVector::null_vector);

}

TEST(test_updating, simple_test_pam ) {
    DistancesIndex::getInstance().initialize(metrics::euclidean_distance);

    Dataset X;
    X["id0"] = NDVector({2,2});
    X["id1"] = NDVector({0,0});
    X["id2"] = NDVector({0,2});
    X["id3"] = NDVector({2,0});
    X["medoid"] = NDVector({1,1});


    PAMalaLloydUpdate update;
    std::vector<Cluster> clusters;
    auto v = NDVector::random_vector(2);
    clusters.push_back(Cluster(v));
    clusters[0].assign("id0");
    clusters[0].assign("id1");
    clusters[0].assign("id2");
    clusters[0].assign("id3");
    clusters[0].assign("medoid");
    update(X, clusters);


    EXPECT_EQ(clusters[0].get_centroid(), NDVector({1,1}));

}


TEST(test_updating, empty_cluster_pam){
    Dataset EMPTY;

    PAMalaLloydUpdate update;
    std::vector<Cluster> clusters;
    auto v = NDVector::random_vector(2);
    clusters.push_back(Cluster(v));
    update(EMPTY, clusters);


    EXPECT_EQ(clusters[0].get_centroid(), NDVector::null_vector);

}


TEST(test_distances_index, rand_dataset_single_cluster){
    DistancesIndex::getInstance().initialize(metrics::euclidean_distance);
    Dataset X;

    for (int i=0; i<10; i++) X[str(i)] = NDVector::random_vector(204);

    PAMalaLloydUpdate update;
    std::vector<Cluster> clusters;
    auto v = NDVector::random_vector(204);
    Cluster C(v);
    for (auto& item: X) C.assign(item.first);

    clusters.push_back(C);
    update(X, clusters);

    std::cout << std::endl;
    DistancesIndex::getInstance().print_stats();


}