#ifndef ERGASIA1_NEARESTNEIGHBORSEARCH_H
#define ERGASIA1_NEARESTNEIGHBORSEARCH_H

#include <unordered_map>
#include "NDVector.h"


/**
 * Search out of a group of datapoints for the nearest neighbor of a query point
 * @param q The query point to search for its neighbor
 * @param X key-value pairs of vectors to be searched
 * @param dist A distance metric function to be used while searching
 * @return A pair containing the ID of the nearest neighbor and its distance to q
 */
std::pair<std::string, double>  nearestNeighbor(NDVector& q, std::unordered_map<std::string, NDVector>& X,
        double (*dist)(NDVector&, NDVector&) );


/**
 * Search for all points in a dataset that are closer than a certain distance to a query point
 * @param q The query point to search for nearest neighbors
 * @param R The distance threshold
 * @param X A key-value pair of vectors to be searched
 * @param dist A distance metric function to be used while searching
 * @return All the IDs of the points in dataset that have distance to q less than R.
 */
std::vector<std::string> range_nearestNeighbors(NDVector& q, double R, std::unordered_map<std::string, NDVector>& X,
        double (*dist)(NDVector&, NDVector&) );

#endif //ERGASIA1_NEARESTNEIGHBORSEARCH_H
