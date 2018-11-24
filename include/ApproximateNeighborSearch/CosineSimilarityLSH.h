#ifndef ERGASIA1_COSINESIMILARITYLSH_H
#define ERGASIA1_COSINESIMILARITYLSH_H


#include <vector>
#include <unordered_map>
#include <set>
#include "HashTable.h"
#include "similaritySearch.h"

/**
 * h function for Cosine similarity LSH. Objects of this class can be called as functions. Property naming
 * follows theoretical notation.
 */
class hCos : public hFunction {
public:
    explicit hCos (int d);
    int operator () (NDVector &p) override;
private:
    NDVector r;
};


/**
 * Implementation of the LSH for the Cosine Similarity case. Property naming follows theoretical notation here as well.
 */
class CosineSimilarityLSH :public LSH{
public:

    /**
     * Construct a CosineSimilarityLSh object
     * @param L Number of Hash Tables
     * @param k Number of h functions per table
     * @param d Dimension of the vectors in the dataset
     */
    explicit CosineSimilarityLSH(int L,int k, int d);

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

    ~CosineSimilarityLSH() override {for (auto &h_family : H) for (auto &h_i : h_family) delete (hCos *)h_i;}


private:

    /**
     * g(p) function as presented: Compute the hash key for a given vector. Since h functions are binary, the resulted
     * key is an integer that is the decimal representation of the concatenated h bits.
     * @param p The vector to compute its hash key
     * @param i The index of the hash table to compute for
     * @return A hash key / hash table position of the bucket of vector p in the i-th hash table
     */
    int g(NDVector p, int i);
};




#endif //ERGASIA1_COSINESIMILARITYLSH_H
