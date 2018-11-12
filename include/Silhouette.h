#ifndef ERGASIA2_SILHOUETTE_H
#define ERGASIA2_SILHOUETTE_H

#include <ApproximateNeighborSearch/NDVector.h>
#include <map>

class SilhouetteResults{
public:
    SilhouetteResults() = default;
    std::vector<double> perClusterSilhouette;
    double averageSilhouette;
};



SilhouetteResults compute_silhouettes(std::map<std::string, NDVector>& X, std::vector<int> assignment, std::vector<NDVector> centers, double (*dist)(NDVector&, NDVector&));

#endif //ERGASIA2_SILHOUETTE_H
