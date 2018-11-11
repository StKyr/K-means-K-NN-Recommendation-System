#ifndef ERGASIA2_ASSIGNING_H
#define ERGASIA2_ASSIGNING_H

#include <vector>
#include <ApproximateNeighborSearch/NDVector.h>
#include <ApproximateNeighborSearch/similaritySearch.h>
#include <map>

class Assignment {
public:
    explicit Assignment(double (*dist)(NDVector&,NDVector&)):dist(dist){}
    virtual std::vector<int> operator() (std::map<std::string, NDVector>& X,std::vector<NDVector> representatives) = 0;

protected:
    double (*dist)(NDVector&,NDVector&);
};

class LloydAssignment: public Assignment{
public:
    explicit LloydAssignment(double (*dist)(NDVector&,NDVector&)) :Assignment(dist){}
    std::vector<int> operator() (std::map<std::string, NDVector>& X, std::vector<NDVector> representatives) override;

};


class ReverseANNAssignment : public Assignment {
public:
    ReverseANNAssignment(AbstractSimilaritySearch& searchIndex, double (*dist)(NDVector&, NDVector&))
        : Assignment(dist), searchIndex(searchIndex){}

    std::vector<int> operator() (std::map<std::string, NDVector>& X, std::vector<NDVector> representatives);


private:
    //std::map<std::string, NDVector>& index;
    AbstractSimilaritySearch& searchIndex;
};


#endif //ERGASIA2_ASSIGNING_H
