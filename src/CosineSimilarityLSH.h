#ifndef ERGASIA1_COSINESIMILARITYLSH_H
#define ERGASIA1_COSINESIMILARITYLSH_H


#include <vector>
#include <NDVector.h>
#include <unordered_map>
#include <set>
#include <LSH.h>
#include "HashTable.h"


namespace cosine{
    class hFunction {
    public:
        explicit hFunction (int d);
        int operator () (NDVector p);
    private:
        NDVector r;
    };

}

class CosineSimilarityLSH :public LSH{


public:
    explicit CosineSimilarityLSH(int L,int k, int d);
    void                  insertVector     (NDVector p, std::string vectorId);
    void                  insertDataset    (std::unordered_map<std::string, NDVector> X);
    std::set<std::string> retrieveNeighbors(NDVector p);


private:

    //int tableSize;
    int L;
    int k;

    std::vector<std::vector<cosine::hFunction>> H;
    std::vector<HashTable> hashTables;


    int g  (NDVector p, int i);

};




#endif //ERGASIA1_COSINESIMILARITYLSH_H
