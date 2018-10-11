#include "NearestNeighborSearch.h"
#include <list>


std::pair<std::string, double>  nearestNeighbor(NDVector q, std::unordered_map<std::string, NDVector> X, double (*dist)(NDVector, NDVector) ){
    double d, minDistance;
    std::string minId, vectorId;
    NDVector p;


    minDistance = 10e100; //TODO: change to first element

    for (std::pair<std::string, NDVector> pair: X){
        vectorId = pair.first;
        p = pair.second;

        d = dist(p, q);

        if (d <= minDistance){
            minDistance = d;
            minId = vectorId;
        }
    }

    return make_pair(minId, minDistance);
}


std::vector<std::string> k_nearestNeighbors(NDVector q, int k, std::unordered_map<std::string, NDVector> X, double (*dist)(NDVector, NDVector) ){
    //TODO: change to an O(N*k) implementation with a k-sized array/list
    using namespace std;

    if (k>=X.size()) throw logic_error("kNN called with illegal k value");
    if (k<=0) throw logic_error("kNN called with zero or negative k value");

    double d;
    vector< pair<double, string> > all_distances;

    vector<string> closestIds;


    for (auto item : X){
        NDVector p = item.second;

        d = dist(q,p);

        all_distances.emplace_back(make_pair(d,item.first));
    }

    sort(all_distances.begin(), all_distances.end());

    int i=0;
    for (auto it = all_distances.begin(); it != all_distances.end() && i<k; i++, ++it){
        closestIds.push_back((*it).second);
    }

    return closestIds;
}