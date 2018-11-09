#ifndef ERGASIA2_ASSIGNING_H
#define ERGASIA2_ASSIGNING_H

#include <vector>
#include <ApproximateNeighborSearch/NDVector.h>

class Assignment {
public:
    virtual std::vector<int> operator() (std::vector<NDVector>& X,std::vector<NDVector> representatives, double (*dist)(NDVector&,NDVector&)) = 0;
};

class LloydAssignment: public Assignment{
public:
    std::vector<int> operator() (std::vector<NDVector>& X,std::vector<NDVector> representatives, double (*dist)(NDVector&,NDVector&)) override;

};

#endif //ERGASIA2_ASSIGNING_H
