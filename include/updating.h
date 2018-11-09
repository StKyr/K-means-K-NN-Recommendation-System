#ifndef ERGASIA2_UPDATING_H
#define ERGASIA2_UPDATING_H

#include <vector>
#include <ApproximateNeighborSearch/NDVector.h>


class Update{
public:
    virtual std::vector<NDVector> operator ()(std::vector<NDVector>& X, std::vector<int> assignment, int k) = 0;
};


class KMeansUpdate: public Update{
public:
    std::vector<NDVector> operator()(std::vector<NDVector>& X, std::vector<int> assignment, int k) override;
};

#endif //ERGASIA2_UPDATING_H
