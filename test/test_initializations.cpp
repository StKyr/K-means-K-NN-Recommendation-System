#include <gtest/gtest.h>
#include <ApproximateNeighborSearch/NDVector.h>
#include <ApproximateNeighborSearch/VectorCSVReader.h>
#include <initializing.h>

template <typename T>
std::string str(T &obj){
    std::stringstream ss;ss << obj;return ss.str();
}


TEST(test_initializations, random_dont_crash) {

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

    Initializer* init = new RandomInitializer();
    auto clusters = (*init)(X, 3);

    //std::cout<<std::endl;for (auto&c : clusters) std::cout << c.get_centroid() <<std::endl;

}

TEST(test_initializations, random_no_duplicates) {

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


    for (int i=0; i< 50; i++) {
        Initializer *init = new RandomInitializer();
        auto clusters = (*init)(X, 3);

        std::vector<std::string> centroid_reprs(3);
        std::transform(clusters.begin(), clusters.end(), centroid_reprs.begin(), [](Cluster c)->std::string{return str(c.get_centroid());});

        std::set<std::string> unique_centroids(centroid_reprs.begin(), centroid_reprs.end());

        EXPECT_EQ(centroid_reprs.size(), unique_centroids.size());
    }
}

TEST(test_initializations, plus_plus_dont_crash) {

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

    Initializer* init = new KMeansPlusPlus();
    auto clusters = (*init)(X, 2);

    std::cout<<std::endl;for (auto&c : clusters) std::cout << c.get_centroid() <<std::endl;

}

TEST(test_initializations, binary_search1){

    /*
     * 0: 0.2
     * 1: 0.1
     * 2: 0.2
     * 3: 0.5
     */

    int index = binary_probability_search(std::vector<double>({0,0.2, 0.3, 0.5, 1}), 0, 5, 0.9);

    EXPECT_EQ(index, 4);
}