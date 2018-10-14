#ifndef ERGASIA1_LSH_H
#define ERGASIA1_LSH_H


#include <unordered_map>
#include <set>
#include "NDVector.h"

class LSH{
public:
    virtual void                  insertVector     (NDVector p, std::string vectorId)            = 0;
    virtual void                  insertDataset    (std::unordered_map<std::string, NDVector> X) = 0;
    virtual std::set<std::string> retrieveNeighbors(NDVector p)                                  = 0;
};


#endif //ERGASIA1_LSH_H
