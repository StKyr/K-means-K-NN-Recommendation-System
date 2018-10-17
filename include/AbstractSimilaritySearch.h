#ifndef ERGASIA1_ABSTRACTSIMILARITYSEARCH_H
#define ERGASIA1_ABSTRACTSIMILARITYSEARCH_H

#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include "NDVector.h"

class AbstractSimilaritySearch{
public:
    virtual void                  insertVector       (NDVector p, std::string vectorId)            = 0;
    virtual void                  insertDataset      (std::unordered_map<std::string, NDVector> X) = 0;
    virtual std::set<std::string> retrieveNeighbors  (NDVector p)                                  = 0;
    virtual                      ~AbstractSimilaritySearch()                                       = default;
};

#endif //ERGASIA1_ABSTRACTSIMILARITYSEARCH_H
