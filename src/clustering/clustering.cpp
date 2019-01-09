#include <vector>
#include "../../include/clustering/cluster.hpp"
#include "../../include/Datasets.hpp"
#include "../../include/clustering/kmeans.hpp"
#include "../../include/clustering/clustering.h"
#include "../../include/clustering/DistancesTable.h"

std::vector<Cluster> k_means_clustering(Dataset& X, int k, KMeansParams& params, bool verbose) {
    std::vector<Cluster> clusters;

    clusters = params.initialize->operator()(X, k);
    do {
        params.assign->operator()(X, clusters);
        params.update->operator()(X, clusters);

        if (verbose) std::cout << params.stopping_criterion->state() << std::endl;

    } while (!params.stopping_criterion->should_stop(clusters));

    if (verbose) std::cout << params.stopping_criterion->state() << std::endl;

    return clusters;
}


std::vector<Cluster> vectorizedDatasetClustering(TfidfDataset &X, int K){
    Criterion *c1 = new IteratorCounter(30);
    Criterion *c3 = new ToleranceCentroidsDisplacement(0.00000001);
    Criterion *c4 = new ObjectiveFunctionMinimization(0.0000001, X.dataset);
    CriteriaOrchestrator *or1 = new OrCriteriaOrchestrator();
    or1->add_criterion(c1);
    or1->add_criterion(c3);
    or1->add_criterion(c4);

    DistancesTable::getInstance().reset();
    DistancesTable::getInstance().initialize(X.dataset.size(), metrics::euclidean_distance);

    KMeansParams params;
    params.initialize         = new KMeansPlusPlus(),
    params.assign             = new LloydAssignment(),
    params.update             = new KMeansUpdate(X.dim()),
    params.stopping_criterion = or1;

    auto result = k_means_clustering(X.dataset, K, params, false);

    delete params.initialize;
    delete params.update;
    delete params.assign;
    delete params.stopping_criterion;

    return result;
}