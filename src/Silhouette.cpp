#include "Silhouette.h"




double average_distance(NDVector& p, std::vector<NDVector>& objects, double (*dist)(NDVector&, NDVector&)){
    double d = 0;
    for (auto q: objects) d += dist(p,q);
    return d / objects.size();
}

std::vector<NDVector> ids_to_points(Dataset& X, std::vector<VectorId>& pointIds){
    std::vector<NDVector> points;
    points.reserve(pointIds.size());
    for (auto& id: pointIds) points.emplace_back(X[id]);
    //std::transform(pointIds.begin(), pointIds.end(), points.begin(), [&](VectorId id){return X[id];});
    return points;
}

SilhouetteResults compute_silhouettes(Dataset& X, std::vector<Cluster>& final_clusters, double (*dist)(NDVector&, NDVector&)){
    int k = (int)final_clusters.size();

    SilhouetteResults results;
    results.perClusterSilhouette.reserve(k);
    for (int i=0; i<k; i++) results.perClusterSilhouette.push_back(0);


    int i = 0;
    for (auto &item : X){

        double second_min_dist = std::numeric_limits<double>::max();

        int best_cluster_index = -1;
        int second_best_cluster_index = -1;

        for (int j=0; j<k; j++){
            if (final_clusters[j].has_point(item.first)){
                best_cluster_index = j;
                continue;
            }

            double d = dist(item.second, final_clusters[j].get_centroid());
            if (d < second_min_dist){
                second_min_dist = d;
                second_best_cluster_index = j;
            }
        }

        if (second_best_cluster_index != -1) {

            std::vector<NDVector> best_cluster_points = ids_to_points(X,final_clusters[best_cluster_index].get_points());
            double a_i = average_distance(item.second, best_cluster_points, dist);

            std::vector<NDVector> second_best_cluster_points = ids_to_points(X,final_clusters[second_best_cluster_index].get_points());
            double b_i = average_distance(item.second, second_best_cluster_points, dist);

            double s_i = (b_i - a_i) / std::max(a_i, b_i);
            results.perClusterSilhouette[best_cluster_index] += s_i;

        }else{
            //TODO: write code here
        }
        i++;
    }

    results.averageSilhouette = 0;
    int zero_sized_clusters = 0;
    for (int i=0; i<k; i++){

        if (final_clusters[i].num_points() != 0){
            results.perClusterSilhouette[i] /= final_clusters[i].num_points();
            results.averageSilhouette += results.perClusterSilhouette[i];
        }else{
            results.perClusterSilhouette[i] = 0;
            zero_sized_clusters++;
        }

    }
    results.averageSilhouette /= k;

    return results;
}

