#include "updating.h"

std::vector<NDVector> KMeansUpdate::operator() (std::map<std::string, NDVector>& X, std::vector<int> assignment, int k){

    std::vector<NDVector> representatives;
    std::vector<int> points_per_cluster;
    representatives.reserve(k);
    points_per_cluster.reserve(k);

    for (int i=0; i<k; i++) {
        representatives.emplace_back(NDVector::zero_vector(dim);
        points_per_cluster.push_back(0);
    }

    int i=0;
    for (auto &item: X){
        NDVector &x = item.second;
        int cluster = assignment[i];
        representatives[cluster] += x;    //TODO: check for overflow
        points_per_cluster[cluster]++;
        i++;
    }

    for (int i=0; i<k; i++) representatives[i] *= (1.0/points_per_cluster[i]);

    return representatives;
}



std::vector<std::vector<NDVector&>> points_per_cluster(std::map<std::string, NDVector>& X, std::vector<int> assignment, int k){

    std::vector<std::vector<NDVector&>> filtered;
    filtered.reserve(k);
    for (int i=0; i<k; i++){
        filtered.emplace_back(std::vector<NDVector&>());
        long num_cluster_points = std::count(assignment.begin(), assignment.end(), assignment[i]);
        filtered[i].reserve((unsigned)num_cluster_points); //todo: check this
    }

    int i=0;
    for (auto& x: X){
        int index = assignment[i];
        filtered[index].push_back(x.second);
        i++;
    }

    return filtered;
}




std::vector<NDVector> PAMalaLloydUpdate::operator() (std::map<std::string, NDVector>& X, std::vector<int> assignment, int k) {

    std::vector<NDVector> representatives;
    std::vector<double> min_medoid_J;

    representatives.reserve(k);
    min_medoid_J.reserve(k);
    for (int i=0; i<k; i++){
        min_medoid_J.push_back(std::numeric_limits<double>::max());
        representatives.emplace_back(NDVector::zero_vector(0));
    }

    auto filtered_points = points_per_cluster(X, assignment, k);

    for (int i=0; i<k; i++){
        std::vector<NDVector&> cluster_points = filtered_points[i];

        for (auto t: cluster_points){

            double J = 0;
            for (auto p: cluster_points) J += dist(t,p);

            if (J < min_medoid_J[i]){
                min_medoid_J[i] = J;
                representatives[i] = t;
            }
        }
    }

    return representatives;
}