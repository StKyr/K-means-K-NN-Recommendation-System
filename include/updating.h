#ifndef ERGASIA2_UPDATING_H
#define ERGASIA2_UPDATING_H

#include <vector>
#include <ApproximateNeighborSearch/NDVector.h>
#include <map>
#include <unordered_map>
#include <ApproximateNeighborSearch/VectorCSVReader.h>
#include "kmeans.hpp"


class Update{
public:
    explicit Update(double (*dist)(NDVector&, NDVector&)): dist(dist) {}
    virtual void operator ()(std::vector<Cluster>& clusters) = 0;

protected:
    double (*dist)(NDVector&, NDVector&);

};


class KMeansUpdate: public Update{
public:
    explicit KMeansUpdate(int dim, double (*dist)(NDVector&, NDVector&)) : Update(dist), dim(dim) {}
    void operator()(std::vector<Cluster>& clusters) override;
private:
    int dim;
};

class PAMalaLloydUpdate :public Update{
public:
    explicit PAMalaLloydUpdate(double (*dist)(NDVector&, NDVector&)): Update(dist) {}
    void operator()(std::vector<Cluster>& clusters) override;

private:

};
#endif //ERGASIA2_UPDATING_H
