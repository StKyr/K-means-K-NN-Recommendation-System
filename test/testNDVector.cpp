/*

#include <gtest/gtest.h>
#include <ApproximateNeighborSearch/NDVector.h>

TEST(testNDVector, check_initialization1) {
    std::vector<double > xs = {1, 2, 3};

    NDVector v = NDVector(xs);

    for (int i=0; i<xs.size(); i++){
        EXPECT_EQ(v[i], xs[i]);
    }
}

TEST(testNDVector, check_initialization2) {
    std::vector<double > xs = {1, 2, 3};

    NDVector v = NDVector({1, 2, 3});

    for (int i=0; i<xs.size(); i++){
        EXPECT_EQ(v[i], xs[i]);
    }
}


TEST(testNDVector, dimensions) {
    NDVector v1 = NDVector({1}),
            v2 = NDVector({1, 2}),
            v3 = NDVector({1, 2, 3}),
            v4 = NDVector({1, 1, 1, 1, 1, 1, 1, 1, 1, 1});

    EXPECT_EQ(v1.dim(), 1);
    EXPECT_EQ(v2.dim(), 2);
    EXPECT_EQ(v3.dim(), 3);
    EXPECT_EQ(v4.dim(), 10);
}

TEST(testNDVector, empty_dimension) {
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

TEST(testNDVector, norm_1D){
    double x = 47.3;
    NDVector v1 = NDVector({x});

    EXPECT_EQ(v1.norm(), x);
}

TEST(testNDVector, norm_2D){
    double x = 47.3;
    NDVector v1 = NDVector({x, 0});
    EXPECT_EQ(v1.norm(), x);

    NDVector v2 = NDVector({0, x});
    EXPECT_EQ(v1.norm(), x);

}

TEST(testNDVector, zero_norm){
    NDVector v1 = NDVector({0});
    EXPECT_EQ(v1.norm(), 0);

    NDVector v2 = NDVector({0, 0, 0, 0, 0, 0, 0});
    EXPECT_EQ(v1.norm(), 0);
}


TEST(testNDVector, norm_ND1){
    NDVector v1 = NDVector({3, 4, 5, 6, 999});

    EXPECT_NEAR(v1.norm(), 999.043, 10e-4);
}


TEST(testNDVector, norm_ND2){
    NDVector v1 = NDVector({3.5, 4.7, 5.2, 6, 999});

    EXPECT_NEAR(v1.norm(), 999.049, 10e-4);
}


TEST(testNDVector, p_norm){
    NDVector v1 = NDVector({3, 4, 5, 6, 999});

    EXPECT_NEAR(v1.norm(3), 999, 10e-4);
}


TEST(testNDVector, equal_norm){
    NDVector v1 = NDVector({3, 4, 5, 6, 999});
    NDVector v2 = NDVector({3, 4, 5, 6, 999});

    EXPECT_EQ(v1.norm(3), v2.norm(3));

}


TEST(testNDVector, identity_norm){
    NDVector v1 = NDVector({3, 4, 5, 6, 999});
    NDVector v2 = NDVector({999, 4, 5, 6, 3});
    NDVector v3 = NDVector({3, 5, 999, 6, 4});


    EXPECT_EQ(v1.norm(3), v2.norm(3));
    EXPECT_EQ(v1.norm(4), v3.norm(4));
}

TEST(testNDVector, equality_operators){
    NDVector v = {1, 2, 3};
    NDVector u = {1, 2, 3};

    EXPECT_EQ((u == v), true);

    u = {100, 100, 100};

    EXPECT_NE(&v, &u);


}

TEST(testNDVector, copy_constructor){
    NDVector v1 = NDVector({1, 2, 3});
    NDVector &v2 = v1;

    EXPECT_EQ(&v1, &v2);
}







TEST(testNDVector, addition){
    NDVector v1 = NDVector({1, 2, 3});
    NDVector v2 = NDVector({4, 5, 6});

    NDVector v3 = v1 + v2;
    NDVector result = NDVector({5,7,9});
    EXPECT_EQ((v3 == result), true);
}

TEST(testNDVector, subtraction){
    NDVector v1 = NDVector({1, 2, 3});
    NDVector v2 = NDVector({4, 5, 6});

    NDVector v3 = v1 - v2;
    NDVector result = NDVector({-3,-3,-3});
    EXPECT_EQ((v3 == result), true);
}

TEST(testNDVector, scalar_multiplication){
    NDVector v1 = NDVector({1, 2, 3});
    double l = 4;

    NDVector v2 = l * v1;
    NDVector result = NDVector({4,8,12});

    EXPECT_EQ((v2 == result), true);
}

TEST(testNDVector, invalid_operations1){
    NDVector v1 = NDVector({1, 2, 3});
    NDVector v2 = NDVector({4, 5, 6, 0, 0, 0, 0});

    try {
        v1 + v2;
        FAIL() << "Exception was not thrown on empty vector.";
    }catch (std::logic_error& e){
        EXPECT_EQ(e.what(), std::string("Nonsensical vector addition of different dimensions"));
    }catch(...){
        FAIL() << "Different type of exception was thrown.";
    }

    try {
        v1 - v2;
        FAIL() << "Exception was not thrown on empty vector.";
    }catch (std::logic_error& e){
        EXPECT_EQ(e.what(), std::string("Nonsensical vector subtraction of different dimensions"));
    }catch(...){
        FAIL() << "Different type of exception was thrown.";
    }


}


TEST(testNDVector, unarry_addition){
    NDVector v1 = {1, 2, 3};
    NDVector v2 = {4, 5, 6};

    v1 += v2;
    NDVector result = NDVector({5,7,9});
    EXPECT_EQ((v1 == result), true);
}

TEST(testNDVector, unarry_subtraction){
    NDVector v1 = NDVector({1, 2, 3});
    NDVector v2 = NDVector({4, 5, 6});

    v1 -= v2;
    NDVector result = NDVector({-3,-3,-3});
    EXPECT_EQ((v1 == result), true);
}

TEST(testNDVector, unarry_scalar_multiplication){
    NDVector v1 = NDVector({1, 2, 3});
    double l = 4;

    v1 *= l;
    NDVector result = NDVector({4,8,12});

    EXPECT_EQ((v1 == result), true);
}


TEST(testNDVector, array_subscript_operator){
    NDVector v1 = NDVector({3, 2, 1});

    EXPECT_EQ( (v1[0]), 3);
    EXPECT_EQ( (v1[1]), 2);
    EXPECT_EQ( (v1[2]), 1);
}


TEST(testNDVector, out_of_bounds_subscript_index_1){
    NDVector v1 = NDVector({3, 2, 1});

    try {
        v1[50];

        FAIL() << "Exception was not thrown on empty vector.";
    }catch (std::range_error& e){
        EXPECT_EQ(e.what(), std::string("Invalid subscript index: 50 (dimension is: 3)"));
    }catch(...){
        FAIL() << "Different type of exception was thrown.";
    }
}


TEST(testNDVector, out_of_bounds_subscript_index_2){
    NDVector v1 = NDVector({3, 2, 1});

    try {
        v1[-3];

        FAIL() << "Exception was not thrown on empty vector.";
    }catch (std::range_error& e){
        EXPECT_EQ(e.what(), std::string("Invalid subscript index: -3 (dimension is: 3)"));
    }catch(...){
        FAIL() << "Different type of exception was thrown.";
    }
}



TEST(testNDVector, stream){
    NDVector v1 = NDVector({3, 2, 1});
    std::stringstream stringstream;
    stringstream << v1;
    std::string string = stringstream.str();

    EXPECT_EQ(string, std::string("{3, 2, 1}"));
}

TEST(testNDVector, dot_product){
    NDVector v1 = NDVector({3, 2, 1});
    NDVector v2 = { 1, 2, 3};

    double product = v1.dot(v2);

    EXPECT_EQ(product, (3*1 + 2*2 + 1*3) );
}

TEST(testNDVector, dot_product_invalid){
    NDVector v1 = {1, 2, 3},
            v2 = {1, 2, 3, 4};

    try {
        v1.dot(v2);
        FAIL() << "Exception was not thrown on empty vector.";
    }catch (std::logic_error& e){
        EXPECT_EQ(e.what(), std::string("Nonsensical dot product of vectors of different dimensions"));
    }catch(...){
        FAIL() << "Different type of exception was thrown.";
    }

    NDVector u;
    try {
        u.dot(v2);
        FAIL() << "Exception was not thrown on empty vector.";
    }catch (std::range_error& e){
        EXPECT_EQ(e.what(), std::string("Unitialized Vector Exception"));
    }catch(...){
        FAIL() << "Different type of exception was thrown.";
    }


}
*/
