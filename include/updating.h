#ifndef ERGASIA2_UPDATING_H
#define ERGASIA2_UPDATING_H

#include <vector>
#include <ApproximateNeighborSearch/NDVector.h>
#include "cluster.hpp"


class Update{
public:
    explicit Update() = default;
    virtual void operator ()(Dataset& X, std::vector<Cluster>& clusters) = 0;
    virtual ~Update() = default;
};


class KMeansUpdate: public Update{
public:
    explicit KMeansUpdate(int dim) : dim(dim) {}
    void operator()(Dataset& X, std::vector<Cluster>& clusters) override;
private:
    int dim;
};

class PAMalaLloydUpdate :public Update{
public:
    void operator()(Dataset& X, std::vector<Cluster>& clusters) override;

private:

};
#endif //ERGASIA2_UPDATING_H
