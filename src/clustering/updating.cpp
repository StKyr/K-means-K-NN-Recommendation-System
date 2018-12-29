#include "../../include/clustering/DistancesTable.h"
#include "../../include/utils.hpp"
#include "../../include/clustering/updating.h"


/**
 * Implementation of the k-means update method
 * @param X
 * @param clusters
 */
void KMeansUpdate::operator() (Dataset& X, std::vector<Cluster>& clusters){

    int k = clusters.size();


    for (auto &C : clusters){
        if (!C.num_points()){
            continue;
        }else {
            NDVector new_centroid = NDVector::zero_vector(dim);
            for (auto &id: C.get_points()) new_centroid += X[id];
            new_centroid *= (1.0 / C.num_points());
            C.update_centroid(new_centroid);
        }
    }
}



/**
 * Implementation of the PAM update method, improved like Lloyd's
 * @param X
 * @param clusters
 */
void PAMalaLloydUpdate::operator() (Dataset& X, std::vector<Cluster>& clusters) {

    for (auto& C : clusters){
        int ni = C.num_points();
        if (!ni){
            continue;
        }
        auto cluster_points = C.get_points();

        double   min_J = std::numeric_limits<double>::max();
        NDVector argmin = {0};

        for (const auto &t_id: cluster_points){
            double J = 0;
            for (const auto &p_id: cluster_points) {
                J += DistancesTable::getInstance().distance(X[t_id], X[p_id]);
            }
            if (J < min_J){
                min_J = J;
                argmin = X[t_id];
            }
        }
        C.update_centroid(argmin);
    }
}