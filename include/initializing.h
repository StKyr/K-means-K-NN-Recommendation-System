#ifndef ERGASIA2_INITIALIZING_H
#define ERGASIA2_INITIALIZING_H

#include <vector>
#include <ApproximateNeighborSearch/NDVector.h>
#include <ApproximateNeighborSearch/VectorCSVReader.h>
#include "cluster.hpp"


class Initializer{
public:
    virtual std::vector<Cluster> operator () (Dataset& X, int k) = 0;
};



class RandomInitializer: public Initializer{
public:
    std::vector<Cluster> operator () (Dataset& X, int k) override;
};

class KMeansPlusPlus : public Initializer{
public:
    explicit KMeansPlusPlus(double (*dist)(NDVector&, NDVector&)) : dist(dist) {}

    std::vector<Cluster> operator () (Dataset& X, int k) override;
private:
    double (*dist)(NDVector&, NDVector&);
};


#endif //ERGASIA2_INITIALIZING_H
