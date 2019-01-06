#ifndef ERGASIA3_PROXIMITYSEARCH_HPP
#define ERGASIA3_PROXIMITYSEARCH_HPP


#include <vector>
#include <unordered_map>
#include "ApproximateNeighborSearch/NDVector.h"
#include "ApproximateNeighborSearch/metrics.h"
#include "ApproximateNeighborSearch/similaritySearch.h"
#include "ApproximateNeighborSearch/CosineSimilarityLSH.h"
#include "ApproximateNeighborSearch/VectorCSVReader.h"
#include "ApproximateNeighborSearch/NearestNeighborSearch.h"


class ProximitySearch {
public:
    virtual std::vector<std::pair<std::string, double>> operator() (NDVector &p)=0;
};


class NearesrNeighborProximitySearch : public ProximitySearch {
public:
    explicit NearesrNeighborProximitySearch(int P, Dataset& X, double (*dist)(NDVector&,NDVector&)=metrics::cosine_distance);
    std::vector<std::pair<std::string, double>> operator() (NDVector &p) override;

private:
    int P;
    double (*dist)(NDVector&,NDVector&);
    Dataset &dataset;
    CosineSimilarityLSH lsh;
};


/*class ClusteringProximitySearch : public ProximitySearch {
public:
    std::vector<std::pair<std::string, double>> operator() (NDVector &p) override;

private:
    double (*dist)(NDVector&,NDVector&);
    Dataset &dataset;

    std::vector<Cluster> clusters;
    std::unordered_map<std::string, int> index;
    bool clusteringPerformed;

    void performClustering();
};


std::vector<std::pair<std::string, double>> ClusteringProximitySearch::operator() (NDVector &p){
    if (!this->clusteringPerformed) performClustering();

    for (auto &id: this->clusters[this->index[   ]])


}*/




#endif //ERGASIA3_PROXIMITYSEARCH_HPP
