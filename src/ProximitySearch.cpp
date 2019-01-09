#include "../include/ProximitySearch.hpp"
#include "../include/parameters.hpp"


NearesrNeighborProximitySearch::NearesrNeighborProximitySearch(int P, Dataset& X, double (*dist)(NDVector &, NDVector &))
        :P(P), dist(dist), lsh(HyperParams::L_LSH, HyperParams::k_LSH, X.begin()->second.dim()), dataset(X) {

    lsh.insertDataset(X);
}


std::vector<std::pair<std::string, double>> NearesrNeighborProximitySearch::operator() (std::string id, NDVector &p){

    Dataset neighborVectors;
    std::set<std::string> nearestNeighbors;

    nearestNeighbors = lsh.retrieveNeighbors(p);

    for (auto &id: nearestNeighbors){
        NDVector &v = dataset[id];
        neighborVectors[id] = v;
    }

    P++;
    return k_nearestNeighbors(P, p, dataset, dist);
}

void ClusteringProximitySearch::performClustering(){

    int K = (HyperParams::K_Cj>0) ? HyperParams::K_Cj : HyperParams::K_tweets/HyperParams::P;

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

    for (auto &C: this->clusters){
        this->centroids.emplace_back(C.get_centroid());
    }

    this->clusteringPerformed = true;
}





std::vector<std::pair<std::string, double>> ClusteringProximitySearch::existingPointeighbors(std::string id, NDVector &p){

    std::vector<std::pair<std::string, double>> neighbors;

    int clusterNumber = this->index[id];
    for (auto &neighborId: this->clusters[clusterNumber].get_points()){
        double d = dist(p, dataset[neighborId]);
        neighbors.emplace_back(std::make_pair(neighborId, d));
    }
    return neighbors;
}


std::vector<std::pair<std::string, double>> ClusteringProximitySearch::nonExistingPointeighbors(std::string id, NDVector &p){


    double minDist = std::numeric_limits<double>::infinity();
    int minIndex = -1;

    for (int i=0; i<this->centroids.size(); i++){
        double d = dist(p, centroids[i]);
        if (d < minDist){
            minDist = d;
            minIndex = i;
        }
    }

    std::vector<std::pair<std::string, double>> neighbors;

    for (auto &neighborId: this->clusters[minIndex].get_points()){
        NDVector q = dataset[neighborId];
        double d = dist(p, q);
        neighbors.emplace_back(std::make_pair(neighborId, d));
    }
    return neighbors;
}




std::vector<std::pair<std::string, double>> ClusteringProximitySearch::operator() (std::string id, NDVector &p){
    if (!this->clusteringPerformed) performClustering();

    if (this->index.find(id) != this->index.end()){
        return this->existingPointeighbors(id, p);
    }else{
        return this->nonExistingPointeighbors(id, p);
    }
}

