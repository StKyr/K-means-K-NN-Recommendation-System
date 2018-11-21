#ifndef ERGASIA2_INITIALIZING_H
#define ERGASIA2_INITIALIZING_H

#include <ApproximateNeighborSearch/NDVector.h>
#include <vector>
#include <map>
#include <unordered_map>
#include "kmeans.hpp"


class Initializer{
public:
    virtual std::vector<Cluster> operator () (Dataset& X, int k, int l) = 0;
};



class RandomInitializer: public Initializer{
public:
    std::vector<Cluster> operator () (Dataset& X, int k, int l) override;
};

class KMeansPlusPlus : public Initializer{
public:
    explicit KMeansPlusPlus(double (*dist)(NDVector&, NDVector&)) : dist(dist) {}

    std::vector<Cluster> operator () (Dataset& X, int k, int l) override;
private:
    double (*dist)(NDVector&, NDVector&);
};


#endif //ERGASIA2_INITIALIZING_H
