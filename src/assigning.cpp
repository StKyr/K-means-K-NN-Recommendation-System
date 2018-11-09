#include "assigning.h"

std::vector<int> LloydAssignment::operator() (std::vector<NDVector>& X, std::vector<NDVector> representatives, double (*dist)(NDVector&,NDVector&)){

    int k = representatives.size();
    std::vector<int> assignment;
    assignment.reserve(X.size());

    double d;

    for (auto& x: X){
        double min_distance = dist(x, representatives[0]);
        int closest_cluster_index = 0;

        for (int i=1; i<k; i++){
            double d = dist(x, representatives[i]);

            if (d < min_distance){
                min_distance = d;
                closest_cluster_index = i;
            }
        }

        assignment.push_back(closest_cluster_index);
    }
    return assignment;
}
