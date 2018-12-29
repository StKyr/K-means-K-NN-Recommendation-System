#ifndef ERGASIA3_CLUSTERING_H
#define ERGASIA3_CLUSTERING_H

#include <vector>
#include "../../include/clustering/cluster.hpp"
#include "../../include/Datasets.hpp"


std::vector<Cluster> vectorizedDatasetClustering(TfidfDataset &X, int K);


#endif //ERGASIA3_CLUSTERING_H
