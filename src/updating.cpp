#include "updating.h"

void KMeansUpdate::operator() (Dataset& X, std::vector<Cluster>& clusters){

    int k = clusters.size();


    for (auto &C : clusters){
        NDVector new_centroid = NDVector::zero_vector(dim);
        for (auto &id: C.get_points()) new_centroid += X[id];   //TODO: overflow here maybe
        new_centroid *= (1.0 / C.num_points());
        C.update_centroid(new_centroid);
    }
}



void PAMalaLloydUpdate::operator() (Dataset& X, std::vector<Cluster>& clusters) {

    for (auto& C : clusters){

        auto cluster_points = C.get_points();

        double   min_J = std::numeric_limits<double>::max();
        NDVector argmin = {0};

        for (const auto &t_id: cluster_points){

            double J = 0;
            for (const auto &p_id: cluster_points) J += dist(X[t_id],X[p_id]); // todo: return J for stopping criterion or use Friends

            if (J < min_J){
                min_J = J;
                argmin = X[t_id];
            }
        }
        C.update_centroid(argmin);
    }
}