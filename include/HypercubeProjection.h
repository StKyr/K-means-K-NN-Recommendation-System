#ifndef ERGASIA1_HYPERCUBEPROJECTION_H
#define ERGASIA1_HYPERCUBEPROJECTION_H


#include <unordered_map>
#include <set>
#include "NDVector.h"
#include "EuclideanSpaceLSH.h"
#include "CosineSimilarityLSH.h"
#include "AbstractSimilaritySearch.h"

typedef std::string hammingNumber;

class HypercubeProjection : public AbstractSimilaritySearch{
public:
                          HypercubeProjection(int d_, int M, int probes, int N)           : d_(d_), M(M), probes(probes) {}
    void                  insertVector       (NDVector p, std::string vectorId)            override;
    void                  insertDataset      (std::unordered_map<std::string, NDVector> X) override;
    std::set<std::string> retrieveNeighbors  (NDVector p)                                  override;
    virtual              ~HypercubeProjection()                                            {}



protected:
    int d_;
    std::vector<hFunction *> H;
    virtual int f(int h_p) =0;

private:

    int M;
    int probes;
    std::unordered_map<hammingNumber, std::vector<std::string>> hashTable;

    hammingNumber projectPoint(NDVector p);
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
