#ifndef ERGASIA2_INITIALIZING_H
#define ERGASIA2_INITIALIZING_H


#include <ApproximateNeighborSearch/NDVector.h>
#include <vector>

class Initializer{
public:
    virtual std::vector<NDVector> operator () (int k, int l) = 0;
};

class RandomInitializer: public Initializer{
public:
    std::vector<NDVector> operator () (int k, int l) override;
};

#endif //ERGASIA2_INITIALIZING_H
