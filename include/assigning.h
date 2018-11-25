#ifndef ERGASIA2_ASSIGNING_H
#define ERGASIA2_ASSIGNING_H

#include <vector>
#include <ApproximateNeighborSearch/NDVector.h>
#include <ApproximateNeighborSearch/VectorCSVReader.h>
#include <ApproximateNeighborSearch/AbstractSimilaritySearch.h>
#include "cluster.hpp"


class Assignment {
public:
    Assignment() = default;
    virtual void operator() (Dataset& X, std::vector<Cluster>& clusters) = 0;

protected:
};

class LloydAssignment: public Assignment{
public:
    void operator() (Dataset& X, std::vector<Cluster>& clusters) override;
};


class ReverseANNAssignment : public Assignment {
public:
    ReverseANNAssignment(AbstractSimilaritySearch& searchIndex) : searchIndex(searchIndex){}
    void operator() (Dataset& X, std::vector<Cluster>& clusters) override;


private:
    AbstractSimilaritySearch& searchIndex;
};


#endif //ERGASIA2_ASSIGNING_H
