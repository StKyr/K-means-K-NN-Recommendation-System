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



class hFunction;
class HashTable;


class EuclideanSpaceLSH {
public:
                          EuclideanSpaceLSH(int L, int tableSize, int M, int k, int d, int w);
    void                  insertVector     (NDVector p, std::string vectorId);
    void                  insertDataset    (std::unordered_map<std::string, NDVector> X);
    std::set<std::string> retrieveNeighbors(NDVector p);


private:
    int L;
    int tableSize;
    int M;

    std::vector<std::vector<hFunction>> H;
    std::vector<std::vector<int>>       R;
    std::vector<HashTable>              hashTables;


    int g  (NDVector p, int j);
    int phi(NDVector p, int j);
};


#endif //ERGASIA1_EUCLIDEANSPACELSH_H
