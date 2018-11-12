#ifndef ERGASIA2_INITIALIZING_H
#define ERGASIA2_INITIALIZING_H


#include <ApproximateNeighborSearch/NDVector.h>
#include <vector>
#include <map>

class Initializer{
public:
    virtual std::vector<NDVector> operator () (std::map<std::string, NDVector>& X, int k, int l) = 0;
};

class RandomInitializer: public Initializer{
public:
    std::vector<NDVector> operator () (std::map<std::string, NDVector>& X, int k, int l) override;
};

class KMeansPlusPlus : public Initializer{
public:
    explicit KMeansPlusPlus(double (*dist)(NDVector&, NDVector&)) : dist(dist) {}

    std::vector<NDVector> operator () (std::map<std::string, NDVector>& X, int k, int l) override;
private:
    double (*dist)(NDVector&, NDVector&);
};

#endif //ERGASIA2_INITIALIZING_H
