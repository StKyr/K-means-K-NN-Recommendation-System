#ifndef ERGASIA2_INITIALIZING_H
#define ERGASIA2_INITIALIZING_H

#include <vector>
#include "cluster.hpp"


class Initializer{
public:
    virtual std::vector<Cluster> operator () (Dataset& X, int k) = 0;
    virtual ~Initializer() = default;
};



class RandomInitializer: public Initializer{
public:
    std::vector<Cluster> operator () (Dataset& X, int k) override;

};

class KMeansPlusPlus : public Initializer{
public:
    std::vector<Cluster> operator () (Dataset& X, int k) override;
};

//int binary_probability_search(std::vector<double> arr, int l, int r, double x);


#endif //ERGASIA2_INITIALIZING_H
