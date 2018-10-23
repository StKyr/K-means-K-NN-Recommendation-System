#ifndef ERGASIA1_EUCLIDEANSPACELSH_H
#define ERGASIA1_EUCLIDEANSPACELSH_H

#include <queue>
#include <list>
#include <string>
#include <array>
#include <vector>
#include <tuple>
#include <set>
#include <unordered_map>

#include "NDVector.h"
#include "HashTable.h"
#include "similaritySearch.h"

#ifdef DEVELOPMENT
#include <gtest/gtest.h>
#endif



/**
 * h function for Euclidean LSH. Objects of this class can be called as functions. Property naming
 * follows theoretical notation.
 */
class hEucl : public hFunction {
    public:
        explicit hEucl (int w, int d);
        int operator () (NDVector &p) override;
    private:
        int w;
        int t;
        NDVector v;

/*
#ifdef DEVELOPMENT
    private:
        FRIEND_TEST(testhFunction, check_t_randomness);
        FRIEND_TEST(testhFunction, check_v_randomness);
        FRIEND_TEST(testhFunction, check_equal_t);
        FRIEND_TEST(testhFunction, return_value_constraints);

#endif
 */
};




/**
 * Implementation of the LSH for the Euclidean Space case. Property naming follows theoretical notation here as well.
 */
class EuclideanSpaceLSH : public LSH{
public:
    /**
     * Construct an EuclideanSpaceLSH object
     * @param L Number of Hash Tables
     * @param tableSize Size of each Hash Table
     * @param k Number of h functions in each table
     * @param d Dimension of vectors in the datasets
     * @param w Discretization hyperparameter of h functions
     */
    EuclideanSpaceLSH(int L, int tableSize, int k, int d, int w);

    /**
     * Insert the ID of a single vector in inner structures
     * @param p The vector whose ID will be stored
     * @param vectorId The ID to be stored
     */
    void insertVector(NDVector &p, std::string vectorId) override;

    /**
     * Insert (the IDs of) all vectors of a dataset in inner structures
     * @param X A dataset with IDs of the vectors as keys
     */
    void insertDataset(std::unordered_map<std::string, NDVector> &X) override;

    /**
     * Search inner structures and retrieve all vector IDs that are possible neighbors candidates of a given vector.
     * @param p The vector to be searched for possible neighbors
     * @return A set of IDs of neighbor candidates from the dataset inserted
     */
    std::set<std::string> retrieveNeighbors(NDVector &p) override;

    ~EuclideanSpaceLSH() override {for (auto &h_family : H) for (auto &h_i : h_family) delete (hEucl *)h_i;}


private:
    static const int M = 0;

    /**
     * The g(p) augmented hash key as presented in theory, with the change that it is formatted as a concatenated string,
     * i.e. {@code "{1,2,3,4}"}.
     * @param p A vector to compute its hash key
     * @param i The index of the hash table to compute the key for (so as to use appropriate h functions)
     * @return A string repesentation of the hash key for the i-th table.
     */
    std::string g(NDVector p, int i);

    /**
     * Get the position of the corresponding bucket of a given vector and a given hash table index.
     * This function will internally call {@code g(p,i)} for getting the hash key and then hash it again using STL's
     * hash function to locate the bucket position.
     * @param p A vector to find its bucket position
     * @param j The index of the hash table to be searched
     * @return The index of the bucket in the j-th hash table
     */
    unsigned phi(NDVector p, int j);


/*
#ifdef DEVELOPMENT

    std::set<std::string> ids1;

    FRIEND_TEST(testEuclideanLSH, g_values_digits);
    FRIEND_TEST(testEuclideanLSH, g_values_exact);
    FRIEND_TEST(testEuclideanLSH, g_values_nearby);

#endif
*/

};


#endif //ERGASIA1_EUCLIDEANSPACELSH_H
