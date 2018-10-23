#ifndef ERGASIA1_HYPERCUBEPROJECTION_H
#define ERGASIA1_HYPERCUBEPROJECTION_H


#include <unordered_map>
#include <set>
#include "NDVector.h"
#include "EuclideanSpaceLSH.h"
#include "CosineSimilarityLSH.h"
#include "AbstractSimilaritySearch.h"

typedef std::string hammingNumber;


/**
 * Base class for Random Hypercube Projection methods
 */
class HypercubeProjection : public AbstractSimilaritySearch{
public:

    HypercubeProjection(int d_, int M, int probes, int N) : d_(d_), M(M), probes(probes) {}

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

    virtual ~HypercubeProjection() {}



protected:
    int d_;
    std::vector<hFunction *> H;
    virtual int f(int h_p) =0;

private:

    int M;
    int probes;
    std::unordered_map<hammingNumber, std::vector<std::string>> hashTable;

    /**
     * Find the hamming number that is the hash key of a datapoint p
     * @param p The datapoint to be hashed
     * @return A hamming number representing the hypercube vertex
     */
    hammingNumber projectPoint(NDVector &p);
};




class EuclideanHypercubeProjection : public HypercubeProjection {
public:

    EuclideanHypercubeProjection(int d_, int M, int probes, int N, int w, int d) ;
    ~EuclideanHypercubeProjection() override;

private:
    std::unordered_map<int, int> h_to_f_index;
    int f(int h_p) override;
};




class CosineHypercubeProjection : public HypercubeProjection {
public:

    CosineHypercubeProjection(int d_, int M, int probes, int N, int d);
    ~CosineHypercubeProjection() override;

private:
    int f(int h_p) override {return h_p;}
};




#endif //ERGASIA1_HYPERCUBEPROJECTION_H
