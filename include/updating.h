#ifndef ERGASIA2_UPDATING_H
#define ERGASIA2_UPDATING_H

#include <vector>
#include <ApproximateNeighborSearch/NDVector.h>


class Update{
public:
    Update(double (*dist)(NDVector&, NDVector&)): dist(dist) {}
    virtual std::vector<NDVector> operator ()(std::vector<NDVector>& X, std::vector<int> assignment, int k) = 0;

protected:
    double (*dist)(NDVector&, NDVector&);

};


class KMeansUpdate: public Update{
public:
    std::vector<NDVector> operator()(std::vector<NDVector>& X, std::vector<int> assignment, int k) override;
};

class PAMalaLloydUpdate :public Update{
public:
    explicit PAMalaLloydUpdate(double (*dist)(NDVector&, NDVector&)): Update(dist) {}

    std::vector<NDVector> operator()(std::vector<NDVector>& X, std::vector<int> assignment, int k) override;

private:

};
#endif //ERGASIA2_UPDATING_H
