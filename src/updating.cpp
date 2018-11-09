#include "updating.h"

std::vector<NDVector> KMeansUpdate::operator() (std::vector<NDVector>& X, std::vector<int> assignment, int k){

    std::vector<NDVector> representatives;
    std::vector<int> points_per_cluster;
    representatives.reserve(k);
    points_per_cluster.reserve(k);

    for (int i=0; i<k; i++) {
        representatives.emplace_back(NDVector::zero_vector(X[0].dim()));
        points_per_cluster.push_back(0);
    }


    for (int i=0; i<X.size(); i++){
        NDVector x = X[i];
        int cluster = assignment[i];
        representatives[cluster] += x;
        points_per_cluster[cluster]++;
    }

    for (int i=0; i<k; i++) representatives[i] *= (1.0/points_per_cluster[i]);

    return representatives;
}
