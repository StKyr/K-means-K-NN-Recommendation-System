#include <DistancesIndex.h>
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
        int ni = C.num_points();

        double dist_table[ni][ni]; for (int i=0; i<ni; i++) for (int j=0; j<ni; j++) dist_table[i][j] = 0;


        auto cluster_points = C.get_points();

        double   min_J = std::numeric_limits<double>::max();
        NDVector argmin = {0};

        //for (const auto &t_id: cluster_points){
        for (int i=0; i<ni; i++){
            auto& t_id = cluster_points[i];

            double J = 0;
            //for (const auto &p_id: cluster_points) J += DistancesIndex::getInstance().distance(X[t_id],X[p_id]); // todo: return J for stopping criterion or use Friends
            for (int j=0; j<ni; j++){
                auto& p_id = cluster_points[j];
                int x = std::min(i,j); int y = std::max(i,j);
                if (dist_table[x][y] == 0) dist_table[x][y] = DistancesIndex::getInstance().distance(X[t_id],X[p_id]);
                J += dist_table[x][y];
            }

            if (J < min_J){
                min_J = J;
                argmin = X[t_id];
            }
        }
        C.update_centroid(argmin);
    }
}