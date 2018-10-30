#include <gtest/gtest.h>

#include "EuclideanSpaceLSH.h"



TEST(testEuclideanLSH, initialize){

        EuclideanSpaceLSH lsh (5,20, 3, 3, 4);
}


TEST(testEuclideanLSH, insert_single_vector){


    EuclideanSpaceLSH lsh (1,5, 3, 2, 4);
    NDVector v = {1, 2};
    lsh.insertVector(v, "v1");
}






TEST(testEuclideanLSH, g_values_digits){

    srand(time(NULL));

    int k = 5;
    int L = 1;
    EuclideanSpaceLSH lsh (L,10, k, 3, 5);


    for (int i=0; i<100; i++) {
        std::vector<double> coords;
        coords.push_back(rand() % 10);
        coords.push_back(rand() % 10);
        coords.push_back(rand() % 10);
        NDVector v = NDVector(coords);
        int g = lsh.g(v, 0);

        EXPECT_GE(g, 10000);
    }




}

TEST(testEuclideanLSH, g_values_exact){
    int k = 5;
    int L = 1;
    EuclideanSpaceLSH lsh (L,10, k, 3, 5);

    NDVector v1 = {1,2,3};
    NDVector v2 = {1,2,3};

    EXPECT_EQ(lsh.g(v1,0),lsh.g(v2,0));
}

TEST(testEuclideanLSH, g_values_nearby){
    int k = 5;
    int L = 1;
    EuclideanSpaceLSH lsh (L,10, k, 3, 5);

    NDVector v1 = {1,2,3};
    NDVector v2 = {1.01,2.02,3.03};

    NDVector q = {-100, 2222, 325};

    EXPECT_EQ(lsh.g(v1,0),lsh.g(v2,0));
    EXPECT_NE(lsh.g(v1,0), lsh.g(q,0));
}


TEST(testEuclideanLSH, insert_multiple_vectors){


#define DEBUG

    EuclideanSpaceLSH lsh (1,10, 5, 3, 5);

    lsh.insertVector(NDVector({1, 2, 3}), "v1");
    lsh.insertVector(NDVector({1.1, 2.1, 3.1}), "v2");
    lsh.insertVector(NDVector({7, 8, 9}), "v3");
    lsh.insertVector(NDVector({11, 21, 10}), "v4");
    lsh.insertVector(NDVector({13, 22, 34}), "v5");
    lsh.insertVector(NDVector({133, 222, 34}), "v6");
    lsh.insertVector(NDVector({12, 211, 33}), "v7");
    lsh.insertVector(NDVector({1222, 222, 344}), "v8");
}
TEST(testEuclideanLSH, insert_dataset){


    EuclideanSpaceLSH lsh (1,10, 5, 3, 5);


    std::unordered_map<std::string, NDVector> map;
    map["v1"] = NDVector({1, 2, 3});
    map["v2"] = NDVector({1.1, 2.1, 3.1});
    map["v3"] = NDVector({7, 8, 9});
    map["v4"] = NDVector({11, 21, 10});
    map["v5"] = NDVector({13, 22, 34});
    map["v6"] = NDVector({133, 222, 34});
    map["v7"] = NDVector({12, 211, 33});
    map["v8"] = NDVector({1, 2, 344});

    lsh.insertDataset(map);




}
TEST(testEuclideanLSH, retrieve_trivial_1){


    EuclideanSpaceLSH lsh (2,5, 3, 2, 4);
    NDVector v = {1, 2};
    lsh.insertVector(v, "v1");

    auto resIds = lsh.retrieveNeighbors(NDVector({1.5,1.4}));

    EXPECT_EQ(resIds.size(), 1);
    EXPECT_NE(resIds.find("v1"), resIds.end());
}


TEST(testEuclideanLSH, retrieve_existing){

    EuclideanSpaceLSH lsh (2,10, 5, 3, 5);


    std::unordered_map<std::string, NDVector> map;
    map["v1"] = NDVector({1, 2, 3});
    map["v2"] = NDVector({1.1, 2.1, 3.1});
    map["v3"] = NDVector({7, 8, 9});
    map["v4"] = NDVector({11, 21, 10});
    map["v5"] = NDVector({13, 22, 34});
    map["v6"] = NDVector({133, 222, 34});
    map["v7"] = NDVector({12, 211, 33});
    map["v8"] = NDVector({1222, 222, 344});
    map["!"] = NDVector({-10000,-10000, -10000});

    lsh.insertDataset(map);

    auto resIds = lsh.retrieveNeighbors({-10000,-10000, -10000});

    EXPECT_GE(resIds.size(), 1);
    EXPECT_NE(resIds.find("!"), resIds.end());
}
TEST(testEuclideanLSH, retrieve_batches){
    FAIL() << "unimplemented";
}







































TEST(testhFunction, initialization){
    hEucl h(10,10);
}


TEST(testhFunction, check_t_randomness){
    hEucl h1(10,3);
    hEucl h2(10,3);

    if (h1.t == h2.t) FAIL() << "Two h functions with the same t. NOTE: This test is probabilistic. Sometimes they get to have the same random values! (with probability 1/w) ";
}

TEST(testhFunction, check_equal_t){
    hEucl h1(1,3);
    hEucl h2(1,3);

    EXPECT_EQ(h1.t, h2.t);
}


TEST(testhFunction, check_v_randomness){
    hEucl h1(5,3);
    hEucl h2(5,3);

    if (h1.v == h2.v) FAIL() << "Two h functions with the same vectors. NOTE: This test is probabilistic, although very very unlikely to fail...";
}

/*

TEST(testhFunction, return_value_constraints){
    srand(time(NULL));

    int N_H = 1;
    int N_W = 1;
    int N_VECTORS = 1;


    for (int i=0; i<N_H; i++) {

        for (int k=0; k < N_W; k++) {

            int w = rand() % 10;

            hFunction h1(w, 3);

            for (int j = 0; j < N_VECTORS; j++) {

                std::vector<double> coords;
                coords.push_back(rand() % 10);
                coords.push_back(rand() % 10);
                coords.push_back(rand() % 10);
                NDVector v = NDVector(coords);

                EXPECT_GE(h1(v), 0);
            }
        }
    }
}

*/



TEST(testHT, initialization){
    HashTable ht(100000);
}


TEST(testHT, single_element){
    HashTable ht(5);

    Bucket bucket;
    bucket.g_key = 542;
    bucket.VectorId = "s1";

    ht.insert(3, bucket);

    std::vector<std::string> result = ht.getVectorIds(3,542);

    EXPECT_EQ(result.size(),1);
    EXPECT_EQ(result[0], "s1");
}
Bucket make_a_Bucket(int g, std::string Vid){
    Bucket bucket;
    bucket.g_key = g;
    bucket.VectorId =  Vid;
    return bucket;
}


TEST(testHT, single_list){



    HashTable ht(5);

    ht.insert(0, make_a_Bucket(30303030, "s1"));
    ht.insert(0, make_a_Bucket(30303030, "s2"));
    ht.insert(0, make_a_Bucket(30303030, "s3"));
    ht.insert(0, make_a_Bucket(30303030, "s4"));
    ht.insert(0, make_a_Bucket(30303030, "s5"));

    std::vector<std::string> result = ht.getVectorIds(0,30303030);

    EXPECT_EQ(result.size(),5);
    EXPECT_EQ((std::find(result.begin(), result.end(), "s1") != result.end()), true);
    EXPECT_EQ((std::find(result.begin(), result.end(), "s2") != result.end()), true);
    EXPECT_EQ((std::find(result.begin(), result.end(), "s3") != result.end()), true);
    EXPECT_EQ((std::find(result.begin(), result.end(), "s4") != result.end()), true);
    EXPECT_EQ((std::find(result.begin(), result.end(), "s5") != result.end()), true);
}


TEST(testHT, multiple_lists){

    HashTable ht(5);

    ht.insert(0, make_a_Bucket(100, "s01"));
    ht.insert(0, make_a_Bucket(100, "s02"));
    ht.insert(0, make_a_Bucket(100, "s03"));
    ht.insert(0, make_a_Bucket(100, "s04"));
    ht.insert(0, make_a_Bucket(100, "s05"));

    ht.insert(2, make_a_Bucket(200, "s21"));
    ht.insert(2, make_a_Bucket(200, "s22"));
    ht.insert(2, make_a_Bucket(200, "s23"));
    ht.insert(2, make_a_Bucket(200, "s24"));
    ht.insert(2, make_a_Bucket(200, "s25"));

    ht.insert(4, make_a_Bucket(300, "s41"));
    ht.insert(4, make_a_Bucket(300, "s42"));
    ht.insert(4, make_a_Bucket(300, "s43"));
    ht.insert(4, make_a_Bucket(300, "s44"));
    ht.insert(4, make_a_Bucket(300, "s45"));


    std::vector<std::string> result0 = ht.getVectorIds(0,100);

    EXPECT_EQ(result0.size(),5);
    EXPECT_EQ((std::find(result0.begin(), result0.end(), "s01") != result0.end()), true);
    EXPECT_EQ((std::find(result0.begin(), result0.end(), "s02") != result0.end()), true);
    EXPECT_EQ((std::find(result0.begin(), result0.end(), "s03") != result0.end()), true);
    EXPECT_EQ((std::find(result0.begin(), result0.end(), "s04") != result0.end()), true);
    EXPECT_EQ((std::find(result0.begin(), result0.end(), "s05") != result0.end()), true);


    std::vector<std::string> result2 = ht.getVectorIds(2,200);

    EXPECT_EQ(result2.size(),5);
    EXPECT_EQ((std::find(result2.begin(), result2.end(), "s21") != result2.end()), true);
    EXPECT_EQ((std::find(result2.begin(), result2.end(), "s22") != result2.end()), true);
    EXPECT_EQ((std::find(result2.begin(), result2.end(), "s23") != result2.end()), true);
    EXPECT_EQ((std::find(result2.begin(), result2.end(), "s24") != result2.end()), true);
    EXPECT_EQ((std::find(result2.begin(), result2.end(), "s25") != result2.end()), true);


    std::vector<std::string> result4 = ht.getVectorIds(4,300);

    EXPECT_EQ(result4.size(),5);
    EXPECT_EQ((std::find(result4.begin(), result4.end(), "s41") != result4.end()), true);
    EXPECT_EQ((std::find(result4.begin(), result4.end(), "s42") != result4.end()), true);
    EXPECT_EQ((std::find(result4.begin(), result4.end(), "s43") != result4.end()), true);
    EXPECT_EQ((std::find(result4.begin(), result4.end(), "s44") != result4.end()), true);
    EXPECT_EQ((std::find(result4.begin(), result4.end(), "s45") != result4.end()), true);

}
TEST(testHT, different_keys){
    HashTable ht(5);

    ht.insert(0, make_a_Bucket(10, "s1"));
    ht.insert(0, make_a_Bucket(11, "s2"));
    ht.insert(0, make_a_Bucket(10, "s3"));
    ht.insert(0, make_a_Bucket(11, "s4"));
    ht.insert(0, make_a_Bucket(10, "s5"));

    std::vector<std::string> result10 = ht.getVectorIds(0,10);

    EXPECT_EQ(result10.size(),3);
    EXPECT_EQ((std::find(result10.begin(), result10.end(), "s1") != result10.end()), true);
    EXPECT_EQ((std::find(result10.begin(), result10.end(), "s3") != result10.end()), true);
    EXPECT_EQ((std::find(result10.begin(), result10.end(), "s5") != result10.end()), true);

    std::vector<std::string> result11 = ht.getVectorIds(0,11);

    EXPECT_EQ(result11.size(),2);
    EXPECT_EQ((std::find(result11.begin(), result11.end(), "s2") != result11.end()), true);
    EXPECT_EQ((std::find(result11.begin(), result11.end(), "s4") != result11.end()), true);
}
TEST(testHT, multiple_lists_different_keys){

    HashTable ht(5);

    ht.insert(0, make_a_Bucket(100, "s01"));
    ht.insert(0, make_a_Bucket(110, "s02"));
    ht.insert(0, make_a_Bucket(100, "s03"));

    std::vector<std::string> result0_100 = ht.getVectorIds(0,100);
    EXPECT_EQ(result0_100.size(),2);
    EXPECT_EQ((std::find(result0_100.begin(), result0_100.end(), "s01") != result0_100.end()), true);
    EXPECT_EQ((std::find(result0_100.begin(), result0_100.end(), "s03") != result0_100.end()), true);

    std::vector<std::string> result0_110 = ht.getVectorIds(0,110);
    EXPECT_EQ(result0_110.size(),1);
    EXPECT_EQ((std::find(result0_110.begin(), result0_110.end(), "s02") != result0_110.end()), true);




    ht.insert(2, make_a_Bucket(200, "s21"));
    ht.insert(2, make_a_Bucket(100, "s22"));
    ht.insert(2, make_a_Bucket(110, "s23"));

    std::vector<std::string> result2_200 = ht.getVectorIds(2,200);
    EXPECT_EQ(result2_200.size(),1);
    EXPECT_EQ((std::find(result2_200.begin(), result2_200.end(), "s21") != result2_200.end()), true);

    std::vector<std::string> result2_100 = ht.getVectorIds(2,100);
    EXPECT_EQ(result2_100.size(),1);
    EXPECT_EQ((std::find(result2_100.begin(), result2_100.end(), "s22") != result2_100.end()), true);

    std::vector<std::string> result2_110 = ht.getVectorIds(2,110);
    EXPECT_EQ(result2_110.size(),1);
    EXPECT_EQ((std::find(result2_110.begin(), result2_110.end(), "s23") != result2_110.end()), true);




    ht.insert(4, make_a_Bucket(300, "s41"));
    ht.insert(4, make_a_Bucket(200, "s42"));
    ht.insert(4, make_a_Bucket(100, "s43"));
    ht.insert(4, make_a_Bucket(100, "s44"));

    std::vector<std::string> result4_300 = ht.getVectorIds(4,300);
    EXPECT_EQ(result4_300.size(),1);
    EXPECT_EQ((std::find(result4_300.begin(), result4_300.end(), "s41") != result4_300.end()), true);

    std::vector<std::string> result4_200 = ht.getVectorIds(4,200);
    EXPECT_EQ(result4_200.size(),1);
    EXPECT_EQ((std::find(result4_200.begin(), result4_200.end(), "s42") != result4_200.end()), true);

    std::vector<std::string> result4_100 = ht.getVectorIds(4,100);
    EXPECT_EQ(result4_100.size(),2);
    EXPECT_EQ((std::find(result4_100.begin(), result4_100.end(), "s43") != result4_100.end()), true);
    EXPECT_EQ((std::find(result4_100.begin(), result4_100.end(), "s44") != result4_100.end()), true);
}

