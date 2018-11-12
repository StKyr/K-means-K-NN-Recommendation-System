#include "Silhouette.h"



double average_distance(NDVector& p, std::vector<NDVector&> objects, double (*dist)(NDVector&, NDVector&)){
    double d = 0;
    for (auto q: objects) d += dist(p,q);
    return d / objects.size();
}

SilhouetteResults compute_silhouettes(std::map<std::string, NDVector>& X, std::vector<int> assignment, std::vector<NDVector> centers, double (*dist)(NDVector&, NDVector&)){
    int k = (int)centers.size();

    SilhouetteResults results;
    results.perClusterSilhouette.reserve(k);
    for (int i=0; i<k; i++) results.perClusterSilhouette.push_back(0);

    std::vector<std::vector<NDVector&>> perClusterObjects;
    for (int i=0; i<k; i++) perClusterObjects.emplace_back(std::vector<NDVector&>());

    int i=0;
    for (auto &item : X){ perClusterObjects[assignment[i]].push_back(item.second); i++;}

    i = 0;
    for (auto &item : X){
        double second_min_dist = std::numeric_limits<double>::max();
        int second_best_cluster=-1;
        for (int j=0; j<k; j++){
            if (j == assignment[i]) continue;
            double d = dist(item.second, centers[j]);
            if (d < second_min_dist){
                second_min_dist = d;
                second_best_cluster = j;
            }
        }

        double a_i = average_distance(item.second, perClusterObjects[assignment[i]], dist);
        double b_i = average_distance(item.second, perClusterObjects[second_best_cluster], dist);
        double s_i = (b_i - a_i) / std::max(a_i, b_i);
        results.perClusterSilhouette[assignment[i]] += s_i;

        i++;
    }

    results.averageSilhouette = 0;
    for (int i=0; i<k; i++){
        results.perClusterSilhouette[i] /= perClusterObjects[i].size();
        results.averageSilhouette += results.perClusterSilhouette[i];
    }
    results.averageSilhouette /= k;

    return results;
}
