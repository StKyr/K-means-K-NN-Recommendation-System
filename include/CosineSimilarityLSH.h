#ifndef ERGASIA1_COSINESIMILARITYLSH_H
#define ERGASIA1_COSINESIMILARITYLSH_H


#include <vector>
#include <NDVector.h>
#include <unordered_map>
#include <set>
#include <LSH.h>
#include "HashTable.h"


class hCos : public hFunction {
public:
    explicit hCos (int d);
    int operator () (NDVector &p) override;
private:
    NDVector r;
};


class CosineSimilarityLSH :public LSH{


public:
    explicit CosineSimilarityLSH(int L,int k, int d);
    void                  insertVector     (NDVector p, std::string vectorId);
    void                  insertDataset    (std::unordered_map<std::string, NDVector> X);
    std::set<std::string> retrieveNeighbors(NDVector p);
    virtual               ~CosineSimilarityLSH(){for (auto &h_family : H) for (auto &h_i : h_family) delete h_i;} //TODO: FIX THIS


private:
    int g(NDVector p, int i);
};




#endif //ERGASIA1_COSINESIMILARITYLSH_H
