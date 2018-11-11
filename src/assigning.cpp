#include "assigning.h"


int single_assignment(NDVector& x, std::vector<NDVector>& representatives, double (*dist)(NDVector&, NDVector&)){
    unsigned long k = representatives.size();
    double min_distance = dist(x, representatives[0]);
    int closest_cluster_index = 0;

    for (int i=1; i<k; i++){
        double d = dist(x, representatives[i]);

        if (d < min_distance){
            min_distance = d;
            closest_cluster_index = i;
        }
    }

    return closest_cluster_index;
}



std::vector<int> LloydAssignment::operator() (std::map<std::string, NDVector>& X, std::vector<NDVector> representatives){

    std::vector<int> assignment;
    assignment.reserve(X.size());

    for (auto& x: X){
        int closest_cluster_index = single_assignment(x.second, representatives, dist);
        assignment.push_back(closest_cluster_index);
    }
    return assignment;
}





std::vector<int> ReverseANNAssignment::operator() (std::map<std::string, NDVector>& X, std::vector<NDVector> representatives){

    /*
     * Business Logic:
     *
     * We will implement a strategy that postpones computing any distances until a point is found to be "nearby" more
     * than one centroid. The algorithm works in linear time and ensures that 1) each centroid-point distance is
     * computed at most once and 2) points that are sufficiently close to a single centroid don't have their distances
     * (to any centred) computed. The space overhead is just a hash table for storing info on retrieved points.
     *
     * 1. We will hold a single dictionary structure for each retrieved point, where info will be stored about (current)
     * nearest center and its distance.
     * 2. For each centroid:
     *  3. We will first retrieve all its "nearby" using LSH.
     *  4. Then, we examine each of its retrieved points:
     *   a) If that point is not previously stored in the dictionary (i.e. no conflicts so far), we store centre's Id
     *     and -1 as distance.
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
    std::unordered_map<std::string,NearestCenterInfo> nearestCenters;                                                   // 1.

    auto k = representatives.size();

    for (int i=0; i<k; i++){                                                                                            // 2.

        std::set<std::string> nearby_points_to_centre = this-> searchIndex.retrieveNeighbors(representatives[i]);       // 3.
        for (auto &vectorId : nearby_points_to_centre){                                                                 // 4.

            if (nearestCenters.find(vectorId) != nearestCenters.end()){                                                 // 4a.
                nearestCenters[vectorId] = std::make_pair(i,-1.0);

            }else {
                NDVector &p = X[vectorId];
                NDVector &c_curr = representatives[i];

                double dist_curr = this->dist(p, c_curr);

                if (nearestCenters[vectorId].second == -1) {                                                            // 4b.
                    int i_prev = nearestCenters[vectorId].first;
                    NDVector &c_prev = representatives[i_prev];
                    double dist_prev = this->dist(p, c_prev);

                    if (dist_curr < dist_prev) nearestCenters[vectorId] = std::make_pair(i, dist_curr);
                    else                       nearestCenters[vectorId] = std::make_pair(i_prev, dist_prev);

                } else {                                                                                                // 4c.
                    if (dist_curr < nearestCenters[vectorId].second){
                        nearestCenters[vectorId] = std::make_pair(i, dist_curr);
                    }
                }
            }
        }
    }

    std::vector<int> assigned_clusters;
    assigned_clusters.reserve(X.size());
    for (auto &x: X){                                                                                                   // 5.

        if (nearestCenters.find(x.first) != nearestCenters.end()){                                                      // 5a.
            assigned_clusters.push_back(nearestCenters[x.first].first);

        }else{                                                                                                          // 5b.
            int assigned_cluster = single_assignment(x.second, representatives, dist);
            assigned_clusters.push_back(assigned_cluster);
        }
    }

    return assigned_clusters;
}
