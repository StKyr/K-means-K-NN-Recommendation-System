#include <DistancesTable.h>
#include <utils.hpp>
#include <cassert>
#include "assigning.h"


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

        int closest_cluster_index = single_assignment(x.second, all_centres);
        auto& C = clusters[closest_cluster_index];
        C.assign(x.first);
    }

}


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
    //std::unordered_map<std::string,NearestCenterInfo> assignmentTable;                                                   // 1.
    NearestCenterInfo assignmentTable[X.size()];
    for (int i=0; i<X.size(); i++) assignmentTable[i] = std::make_pair(-1,-1);

    auto k = clusters.size();

    size_t num_stored = 0;
    size_t num_collision = 0;

    size_t total_points_recieved = 0;

    for (int i=0; i<k; i++){                                                                                            // 2.

        std::set<std::string> nearby_points_to_centre = this-> searchIndex.retrieveNeighbors(clusters[i].get_centroid());       // 3.

        total_points_recieved += nearby_points_to_centre.size();

        for (auto &vectorId : nearby_points_to_centre){                                                                 // 4.


            //if (assignmentTable.find(vectorId) == assignmentTable.end()){                                                 // 4a.
            if (assignmentTable[X[vectorId].getId()].first == -1){
                double d = DistancesTable::getInstance().distance(X[vectorId], clusters[i].get_centroid());
                assignmentTable[X[vectorId].getId()] = std::make_pair(i, d);

                num_stored++;

            }else {
                NDVector &p = X[vectorId];
                NDVector &c_curr = clusters[i].get_centroid();

                double dist_curr = DistancesTable::getInstance().distance(p, c_curr);

                if (assignmentTable[X[vectorId].getId()].second == -1) {                                                            // 4b.
assert(false);
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

                num_collision++;
            }
        }
    }

    std::vector<NDVector> all_centres;
    all_centres.reserve(k);

    for (int i=0; i<k; i++){    // order not guaranteed in for each loop
        auto &C = clusters[i];
        C.clear_assignment();
        all_centres.push_back(C.get_centroid());
    }



    //range_assigment(clusters, assignmentTable, X.size());

    double proximity_threshold = average_dist_between_clusters(clusters) / 16;


    size_t num_missed=0;
    size_t num_no_collisions=0;
    for (auto &x: X){                                                                                                   // 5.
        int cluster_index;

        //if (assignmentTable.find(x.first) != assignmentTable.end()){                                                      // 5a.
        if (assignmentTable[X[x.first].getId()].first != -1){
            //if (assignmentTable[X[x.first].getId()].second==-1)num_no_collisions++;

            if (assignmentTable[X[x.first].getId()].second <= proximity_threshold){
                cluster_index = assignmentTable[X[x.first].getId()].first;
            }else{
                num_missed++;
                cluster_index = single_assignment(x.second, all_centres);
            }

        }else{                                                                                                          // 5b.
            num_missed++;
            cluster_index = single_assignment(x.second, all_centres);
        }

        Cluster& C = clusters[cluster_index];
        C.assign(x.first);
    }

    float d1 = ((float)num_collision + k*num_missed) /(X.size()*k)*100;
    float d2 = (float)num_no_collisions/X.size()*100;
    float d3 = (float)num_missed/X.size()*100;
    flog("Computed "+str(d1)+"% of distances | Assigned directly "+str(d2)+"% of points | Searched "+str(d3)+"% points exhaustively");

/*   flog("Total points retrieved: "+str(total_points_recieved)+
    " | Distinct points retrieved: "+str(num_stored)+
    "| Collisions: "+str(num_collision)+"| Points searched exhaustively: "+str(num_missed));*/
}
