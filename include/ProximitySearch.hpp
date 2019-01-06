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
#include "clustering/cluster.hpp"
#include "clustering/kmeans.hpp"
#include "clustering/DistancesTable.h"


class ProximitySearch {
public:
    virtual std::vector<std::pair<std::string, double>> operator() (std::string id, NDVector &p)=0;
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
    ClusteringProximitySearch(int P, Dataset &dataset, double (*dist)(NDVector&,NDVector&))
        :P(P), dataset(dataset), dist(dist), clusteringPerformed(false) {}
    std::vector<std::pair<std::string, double>> operator() (std::string id, NDVector &p) override;

private:
    int P;
    double (*dist)(NDVector&,NDVector&);
    Dataset &dataset;

    std::vector<Cluster> clusters;
    std::unordered_map<std::string, int> index;
    bool clusteringPerformed;

    void performClustering();
};

void ClusteringProximitySearch::performClustering(){

    int K = dataset.size() / P;

    static KMeansParams params;
    Criterion *c1 = new IteratorCounter(30);
    Criterion *c3 = new ToleranceCentroidsDisplacement(0.00000001);
    Criterion *c4 = new ObjectiveFunctionMinimization(0.0000001, dataset);
    CriteriaOrchestrator *or1 = new OrCriteriaOrchestrator();
    or1->add_criterion(c1);
    or1->add_criterion(c3);
    or1->add_criterion(c4);

    DistancesTable::getInstance().reset();
    DistancesTable::getInstance().initialize(dataset.size(), this->dist);

    params.initialize         = new KMeansPlusPlus(),
    params.assign             = new LloydAssignment(),
    params.update             = new KMeansUpdate(dataset.begin()->second.dim()),
    params.stopping_criterion = or1;

    this->clusters = k_means_clustering(dataset, K, params, false);

    delete params.initialize;
    delete params.update;
    delete params.assign;
    delete params.stopping_criterion;


    this->clusteringPerformed = true;
}


std::vector<std::pair<std::string, double>> ClusteringProximitySearch::operator() (std::string id, NDVector &p){
    if (!this->clusteringPerformed) performClustering();

    std::vector<std::pair<std::string, double>> neighbors;

    for (auto &neighborId: this->clusters[this->index[id]].get_points()){
        double d = dist(p, dataset[neighborId]);
        neighbors.emplace_back(std::make_pair(neighborId, d));
    }
    return neighbors;
}




#endif //ERGASIA3_PROXIMITYSEARCH_HPP
