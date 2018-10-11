#include "NearestNeighborSearch.h"

std::pair<std::string, double>  nearestNeighbor(NDVector q, std::unordered_map<std::string, NDVector> X, double (*distance)(NDVector, NDVector) ){
    double d, minDistance;
    std::string minId, vectorId;
    NDVector p;


    minDistance = 10e100; //TODO: change to first element

    for (std::pair<std::string, NDVector> pair: X){
        vectorId = pair.first;
        p = pair.second;

        d = distance(p, q);

        if (d <= minDistance){
            minDistance = d;
            minId = vectorId;
        }
    }

    std::pair<std::string, double> returnTuple = {minId, minDistance};
    return returnTuple;
}