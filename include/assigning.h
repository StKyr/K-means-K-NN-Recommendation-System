#ifndef ERGASIA2_ASSIGNING_H
#define ERGASIA2_ASSIGNING_H

#include <vector>
#include <ApproximateNeighborSearch/NDVector.h>
#include <ApproximateNeighborSearch/similaritySearch.h>
#include <map>
#include <ApproximateNeighborSearch/VectorCSVReader.h>
#include "kmeans.hpp"

class Assignment {
public:
    explicit Assignment(double (*dist)(NDVector&,NDVector&)):dist(dist){}
    virtual void operator() (Dataset& X, std::vector<Cluster>& clusters) = 0;

protected:
    double (*dist)(NDVector&,NDVector&);
};

class LloydAssignment: public Assignment{
public:
    explicit LloydAssignment(double (*dist)(NDVector&,NDVector&)) :Assignment(dist){}
    void operator() (Dataset& X, std::vector<Cluster>& clusters) override;

};


class ReverseANNAssignment : public Assignment {
public:
    ReverseANNAssignment(AbstractSimilaritySearch& searchIndex, double (*dist)(NDVector&, NDVector&))
        : Assignment(dist), searchIndex(searchIndex){}

    void operator() (Dataset& X, std::vector<Cluster>& clusters) override;


private:
    //std::unordered_map<std::string, NDVector>& index;
    AbstractSimilaritySearch& searchIndex;
};


#endif //ERGASIA2_ASSIGNING_H
