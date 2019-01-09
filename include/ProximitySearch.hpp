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
#include "clustering/kmeans.hpp"
#include "clustering/DistancesTable.h"


class ProximitySearch {
public:
    virtual std::vector<std::pair<std::string, double>> operator() (std::string id, NDVector &p)=0;
    virtual ~ProximitySearch() = default;
};


class NearesrNeighborProximitySearch : public ProximitySearch {
public:
    explicit NearesrNeighborProximitySearch(int P, Dataset& X, double (*dist)(NDVector&,NDVector&)=metrics::cosine_distance);
    std::vector<std::pair<std::string, double>> operator() (std::string id, NDVector &p) override;

private:
    int P;
    double (*dist)(NDVector&,NDVector&);
    Dataset &dataset;
    CosineSimilarityLSH lsh;

};


class Cluster;

class ClusteringProximitySearch : public ProximitySearch {
public:
    ClusteringProximitySearch(int P, Dataset &dataset, double (*dist)(NDVector&,NDVector&)=metrics::cosine_distance)
        :P(P), dataset(dataset), dist(dist), clusteringPerformed(false) {}
    std::vector<std::pair<std::string, double>> operator() (std::string id, NDVector &p) override;

private:
    int P;
    double (*dist)(NDVector&,NDVector&);
    Dataset &dataset;

    std::vector<Cluster> clusters;
    std::vector<NDVector> centroids;
    std::unordered_map<std::string, int> index;
    bool clusteringPerformed;

    void performClustering();
    std::vector<std::pair<std::string, double>> existingPointeighbors(std::string id, NDVector &p);
    std::vector<std::pair<std::string, double>> nonExistingPointeighbors(std::string id, NDVector &p);
};





#endif //ERGASIA3_PROXIMITYSEARCH_HPP
