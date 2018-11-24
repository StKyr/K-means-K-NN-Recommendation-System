#include "assigning.h"

//TODO: Fuckin understand references

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



void LloydAssignment::operator() (Dataset& X, std::vector<Cluster>& clusters){


    int k = clusters.size();
    std::vector<NDVector> all_centres;
    all_centres.reserve(k);

    for (int i=0; i<k; i++){    // order not guaranteed in for each loop
        auto &C = clusters[i];
        C.clear_assignment();
        all_centres.push_back(C.get_centroid());
    }

    for (auto x: X){

        int closest_cluster_index = single_assignment(x.second, all_centres, dist);
        auto& C = clusters[closest_cluster_index];
        C.assign(x.first);
    }

}





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
    std::unordered_map<std::string,NearestCenterInfo> nearestCenters;                                                   // 1.

    auto k = clusters.size();

    for (int i=0; i<k; i++){                                                                                            // 2.

        std::set<std::string> nearby_points_to_centre = this-> searchIndex.retrieveNeighbors(clusters[i].get_centroid());       // 3.
        for (auto &vectorId : nearby_points_to_centre){                                                                 // 4.

            if (nearestCenters.find(vectorId) != nearestCenters.end()){                                                 // 4a.
                nearestCenters[vectorId] = std::make_pair(i,-1.0);

            }else {
                NDVector &p = X[vectorId];
                NDVector &c_curr = clusters[i].get_centroid();

                double dist_curr = this->dist(p, c_curr);

                if (nearestCenters[vectorId].second == -1) {                                                            // 4b.
                    int i_prev = nearestCenters[vectorId].first;
                    NDVector &c_prev = clusters[i_prev].get_centroid();
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

    std::vector<NDVector> all_centres(k);

    for (int i=0; i<k; i++){    // order not guaranteed in for each loop
        auto &C = clusters[i];
        C.clear_assignment();
        all_centres.push_back(C.get_centroid());
    }

    for (auto &x: X){                                                                                                   // 5.
        int cluster_index;

        if (nearestCenters.find(x.first) != nearestCenters.end()){                                                      // 5a.
            cluster_index = nearestCenters[x.first].first;

        }else{                                                                                                          // 5b.
            cluster_index = single_assignment(x.second, all_centres, dist);
        }

        Cluster& C = clusters[cluster_index];
        C.assign(x.first);
    }
}
