#ifndef ERGASIA1_LSH_H
#define ERGASIA1_LSH_H


#include <unordered_map>
#include <set>
#include "NDVector.h"
#include "HashTable.h"
#include "AbstractSimilaritySearch.h"

class hFunction;


/**
 * Interface for both LSH classes
 */
class LSH : public AbstractSimilaritySearch{
public:
    virtual void                  insertVector     (NDVector &p, std::string vectorId)            = 0;
    virtual void                  insertDataset    (std::unordered_map<std::string, NDVector> &X) = 0;
    virtual std::set<std::string> retrieveNeighbors(NDVector &p)                                  = 0;
    virtual                      ~LSH() = default;

protected:
    int L;                                              /* Number of Hash Tables */
    int tableSize;                                      /* Size of each Hash Table */

    std::vector<std::vector<hFunction *>> H;            /* A 2D array storing all h functions */
    std::vector<HashTable>              hashTables;     /* An array of Hash Tables for storing keys */
};



/**
 * Base class of all h functions. Paradoxically, h functions are better depicted by objects and not functions
 * as they need to store specific data that change according to each H family. They override the call operator
 * to provide a function-like interface, making them `functionObjects`.
 */
class hFunction{
public:
    virtual int operator () (NDVector &p) = 0;
};



#endif //ERGASIA1_LSH_H
