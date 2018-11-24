#ifndef ERGASIA2_SILHOUETTE_H
#define ERGASIA2_SILHOUETTE_H

#include <vector>
#include <unordered_map>
#include <ApproximateNeighborSearch/NDVector.h>
#include "cluster.hpp"


class SilhouetteResults{
public:
    std::vector<double> perClusterSilhouette;
    double averageSilhouette;
};



SilhouetteResults compute_silhouettes(Dataset& X, std::vector<Cluster>& final_clusters,
        double (*dist)(NDVector&, NDVector&));


#endif //ERGASIA2_SILHOUETTE_H
