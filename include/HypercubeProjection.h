#ifndef ERGASIA1_HYPERCUBEPROJECTION_H
#define ERGASIA1_HYPERCUBEPROJECTION_H


#include <unordered_map>
#include <set>
#include "NDVector.h"
#include "EuclideanSpaceLSH.h"
#include "CosineSimilarityLSH.h"

typedef std::string hammingNumber;

class HypercubeProjection{
public:
                             HypercubeProjection(int d_, int M, int probes, int N)           : d_(d_), M(M), probes(probes) {}
    void                     insertVector       (NDVector p, std::string vectorId)           ;
    void                     insertDataset      (std::unordered_map<std::string, NDVector> X);
    std::vector<std::string> retrieveNeighbors  (NDVector p)                                 ;



protected:

    std::vector<int (*) (NDVector p)> H;
    int (* f) (int h_p);

private:
    int d_;
    int M;
    int probes;
    std::unordered_map<hammingNumber , std::vector<std::string>> hashTable;


    hammingNumber projectPoint(NDVector p);
};

class EuclideanHypercubeProjection : HypercubeProjection {
public:

    EuclideanHypercubeProjection(int d_, int M, int probes, int N, int w, int d)  :HypercubeProjection(d_, M, probes, N){
        for (int i=0; i<d_; i++) H.emplace_back(euclidean::hFunction(w,d));
        this->f = euclideanF;
    }

private:
    static int euclideanF(int h_p){return (h_p > 0)? 1 : 0;}
};

class CosineHyperspaceProjection : HypercubeProjection {
public:

    CosineHyperspaceProjection(int d_, int M, int probes, int N, int d)  :HypercubeProjection(d_, M, probes, N){
        for (int i=0; i<d_; i++) H.emplace_back(cosine::hFunction(d));
        this->f = cosineF;
    }

private:
    static int cosineF(int h_p){return h_p;}
};




#endif //ERGASIA1_HYPERCUBEPROJECTION_H
