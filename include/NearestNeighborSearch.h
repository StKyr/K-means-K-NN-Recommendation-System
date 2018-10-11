#ifndef ERGASIA1_NEARESTNEIGHBORSEARCH_H
#define ERGASIA1_NEARESTNEIGHBORSEARCH_H

#include <unordered_map>
#include "NDVector.h"

std::pair<std::string, double>  nearestNeighbor(NDVector q, std::unordered_map<std::string, NDVector> X,
        double (*dist)(NDVector, NDVector) );

std::vector<std::string> k_nearestNeighbors(NDVector q, int k, std::unordered_map<std::string, NDVector> X,
        double (*dist)(NDVector, NDVector) );

#endif //ERGASIA1_NEARESTNEIGHBORSEARCH_H
