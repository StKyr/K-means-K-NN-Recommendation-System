#ifndef ERGASIA2_UPDATING_H
#define ERGASIA2_UPDATING_H

#include <vector>
#include <ApproximateNeighborSearch/NDVector.h>
#include <map>


class Update{
public:
    explicit Update(double (*dist)(NDVector&, NDVector&)): dist(dist) {}
    virtual std::vector<NDVector> operator ()(std::map<std::string, NDVector>& X, std::vector<int> assignment, int k) = 0;

protected:
    double (*dist)(NDVector&, NDVector&);

};


class KMeansUpdate: public Update{
public:
    explicit KMeansUpdate(int dim, double (*dist)(NDVector&, NDVector&)) : Update(dist), dim(dim) {}
    std::vector<NDVector> operator()(std::map<std::string, NDVector>& X, std::vector<int> assignment, int k) override;
private:
    int dim;
};

class PAMalaLloydUpdate :public Update{
public:
    explicit PAMalaLloydUpdate(double (*dist)(NDVector&, NDVector&)): Update(dist) {}

    std::vector<NDVector> operator()(std::map<std::string, NDVector>& X, std::vector<int> assignment, int k) override;

private:

};
#endif //ERGASIA2_UPDATING_H
