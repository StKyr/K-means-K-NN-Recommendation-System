#include "../include/ProximitySearch.hpp"


NearesrNeighborProximitySearch::NearesrNeighborProximitySearch(int P, Dataset& X, double (*dist)(NDVector &, NDVector &))
        :P(P), dist(dist), lsh(3,8,X.begin()->second.dim()), dataset(X) {

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
