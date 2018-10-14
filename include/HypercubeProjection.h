#ifndef ERGASIA1_HYPERCUBEPROJECTION_H
#define ERGASIA1_HYPERCUBEPROJECTION_H


#include <unordered_map>
#include <set>
#include "NDVector.h"

typedef std::string hammingNumber;

class HypercubeProjection{
public:
                                  HypercubeProjection(int d_, int M, int probes) :d_(d_), M(M), probes(probes) {}
    void                  insertVector     (NDVector p, std::string vectorId)            ;
    void                  insertDataset    (std::unordered_map<std::string, NDVector> X) ;
    std::set<std::string> retrieveNeighbors(NDVector p)                                  ;



protected:

    int (* h) (NDVector p);
    int (* f) (NDVector p, int (*h) (NDVector p));

private:
    int d_;
    int M;
    int probes;
    std::unordered_map<hammingNumber , std::vector<std::string>> hashTable;


    hammingNumber projectPoint(NDVector p);




};




#endif //ERGASIA1_HYPERCUBEPROJECTION_H
