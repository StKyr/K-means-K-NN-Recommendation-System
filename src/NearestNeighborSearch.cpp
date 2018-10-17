#include "NearestNeighborSearch.h"
#include <list>
#include <iostream>
#include <csignal>


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


std::vector<std::string> range_nearestNeighbors(NDVector q, double R, std::unordered_map<std::string, NDVector> X, double (*dist)(NDVector, NDVector) ){

    if (R<=0) throw std::logic_error("Nearest Neighbor called with zero or negative R value");

    std::vector<std::string> nearbyIds;

    double d;
    NDVector p;
    std::string vectorId;

    for (auto item : X){
        NDVector p = item.second;
        vectorId = item.first;

        d = dist(q,p);
        if (d < R) {
            nearbyIds.push_back(vectorId);
        }
    }
    return nearbyIds;
}