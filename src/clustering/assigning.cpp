#include "../../include/clustering/DistancesTable.h"
#include "../../include/utils.hpp"
#include <cassert>
#include "../../include/clustering/assigning.h"


/**
 * For a single point, compute distances to all centroids and return the index of the closest
 * @param x The point to be assigned
 * @param representatives The centroids to be searched
 * @return The index in the table of the nearest centroid
 */
int single_assignment(NDVector& x, std::vector<NDVector>& representatives){
    unsigned long k = representatives.size();
    double min_distance = DistancesTable::getInstance().distance(x, representatives[0]);
    int closest_cluster_index = 0;

    for (int i=1; i<k; i++){
        double d = DistancesTable::getInstance().distance(x, representatives[i]);

        if (d < min_distance){
            min_distance = d;
            closest_cluster_index = i;
        }
    }

    return closest_cluster_index;
}


/**
 * Implementation of the Lloyd assignment
 * @param X
 * @param clusters
 */
void LloydAssignment::operator() (Dataset& X, std::vector<Cluster>& clusters){


    int k = clusters.size();
    std::vector<NDVector> all_centres;
    all_centres.reserve(k);

    for (int i=0; i<k; i++){
        auto &C = clusters[i];
        C.clear_assignment();
        all_centres.push_back(C.get_centroid());
    }

    for (auto x: X){

        int closest_cluster_index = single_assignment(x.second, all_centres);
        auto& C = clusters[closest_cluster_index];
        C.assign(x.first);
    }

}



// To be used by possible range searches or thresholds
double min_dist_between_clusters(std::vector<Cluster>& clusters){

    double min_dist = std::numeric_limits<double>::max();

    for (int i=0; i<clusters.size(); i++){
        for (int j=0; j<clusters.size(); j++){
            auto c1 = clusters[i].get_centroid();
            auto c2 = clusters[j].get_centroid();
            double d = DistancesTable::getInstance().distance(c1,c2);
            if (d) d = std::min(d, min_dist);
        }
    }
    return min_dist;
}

// To be used by possible range searches or thresholds
double average_dist_between_clusters(std::vector<Cluster>& clusters){

    double avg_dist = 0;

    for (int i=0; i<clusters.size(); i++){
        for (int j=0; j<clusters.size(); j++){
            auto c1 = clusters[i].get_centroid();
            auto c2 = clusters[j].get_centroid();
            double d = DistancesTable::getInstance().distance(c1,c2);

            avg_dist += d / clusters.size();
        }
    }
    return avg_dist;
}



/**
 * Implementation of the reverse assignment both for the LSH and the Hypercube case
 * @param X
 * @param clusters
 */
void ReverseANNAssignment::operator() (Dataset& X, std::vector<Cluster>& clusters){

    /*
     * Business Logic:
     *
     * We will implement a strategy that postpones computing any distances until a point is found to be "nearby" more
     * than one centroid. The algorithm works in linear time and ensures that 1) each centroid-point distance is
     * computed at most once and 2) points that are "sufficiently" close to a single centroid don't have their distances
     * (to any center) computed. The space overhead is just a hash table for storing info on retrieved points.
     *
     * 1. We will hold a single dictionary structure for each retrieved point, where info will be stored about (current)
     * nearest center and its distance.
     * 2. For each centroid:
     *  3. We will first retrieve all its "nearby" points using LSH.
     *  4. Then, we examine each of its retrieved points:
     *   a) If that point is not previously stored in the dictionary (i.e. no conflicts so far), we store centre's Id
     *     and -1 as distance. (no distance computation here)
     *   b) If that point exists in the dictionary and distance is -1, we calculate its distance from the stored centred
     *   and its distance from the current centre and we store the centre-distance combination of the minimum distance.
     *   c) If the point exists in the dictionary and current distance is != -1, then we compute the distance to current
     *   centre, we compare it to the saved distance and we again store the minimum combination.
     *
     * 5. We are now ready to assign points to clusters: For every point x in X,
     *   a) if x exists in our dictionary, we assign to currently stored cluster
     *   b) otherwise, we exhaustively compute distances to all centres and assign to argmin.
     */

    typedef std::pair<int, double> NearestCenterInfo;
    NearestCenterInfo assignmentTable[X.size()];                                                                        // 1
    for (int i=0; i<X.size(); i++) assignmentTable[i] = std::make_pair(-1,-1);

    auto k = clusters.size();


    for (int i=0; i<k; i++){                                                                                            // 2.

        std::set<std::string> nearby_points_to_centre = this-> searchIndex.retrieveNeighbors(clusters[i].get_centroid());       // 3.

        for (auto &vectorId : nearby_points_to_centre){                                                                 // 4.


            if (assignmentTable[X[vectorId].getId()].first == -1){                                                      // 4a
                //double d = DistancesTable::getInstance().distance(X[vectorId], clusters[i].get_centroid());
                assignmentTable[X[vectorId].getId()] = std::make_pair(i, -1);

            }else {
                NDVector &p = X[vectorId];
                NDVector &c_curr = clusters[i].get_centroid();

                double dist_curr = DistancesTable::getInstance().distance(p, c_curr);

                if (assignmentTable[X[vectorId].getId()].second == -1) {                                                            // 4b.

                    int i_prev = assignmentTable[X[vectorId].getId()].first;
                    NDVector &c_prev = clusters[i_prev].get_centroid();
                    double dist_prev = DistancesTable::getInstance().distance(p, c_prev);

                    if (dist_curr < dist_prev) assignmentTable[X[vectorId].getId()] = std::make_pair(i, dist_curr);
                    else                       assignmentTable[X[vectorId].getId()] = std::make_pair(i_prev, dist_prev);

                } else {                                                                                                // 4c.
                    if (dist_curr < assignmentTable[X[vectorId].getId()].second){
                        assignmentTable[X[vectorId].getId()] = std::make_pair(i, dist_curr);
                    }
                }
            }
        }
    }

    std::vector<NDVector> all_centres;
    all_centres.reserve(k);

    for (int i=0; i<k; i++){
        auto &C = clusters[i];
        C.clear_assignment();
        all_centres.push_back(C.get_centroid());
    }



    for (auto &x: X){                                                                                                   // 5.
        int cluster_index;

        if (assignmentTable[X[x.first].getId()].first != -1){                                                           // 5a.
            cluster_index = assignmentTable[X[x.first].getId()].first;

        }else{                                                                                                          // 5b.
            cluster_index = single_assignment(x.second, all_centres);
        }

        Cluster& C = clusters[cluster_index];
        C.assign(x.first);
    }
}
