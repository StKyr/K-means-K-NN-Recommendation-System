#ifndef ERGASIA1_LSH_H
#define ERGASIA1_LSH_H


#include <unordered_map>
#include <set>
#include "NDVector.h"
#include "HashTable.h"
#include "AbstractSimilaritySearch.h"

class hFunction;

class LSH : public AbstractSimilaritySearch{
public:
    virtual void                  insertVector     (NDVector p, std::string vectorId)            = 0;
    virtual void                  insertDataset    (std::unordered_map<std::string, NDVector> X) = 0;
    virtual std::set<std::string> retrieveNeighbors(NDVector p)                                  = 0;
    virtual                      ~LSH() = default;

protected:
    int L;
    int tableSize;

    std::vector<std::vector<hFunction *>> H;
    std::vector<HashTable>              hashTables;
};



class hFunction{
public:
    virtual int operator () (NDVector &p) = 0;
};



#endif //ERGASIA1_LSH_H
