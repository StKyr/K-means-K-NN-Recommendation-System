#ifndef KMEANS_HPP
#define KMEANS_HPP

#include <vector>
#include <ApproximateNeighborSearch/NDVector.h>
#include <ApproximateNeighborSearch/VectorCSVReader.h>
#include <iostream>
#include "initializing.h"
#include "assigning.h"
#include "updating.h"
#include "StoppingCriterion.h"


class KMeansParams{
public:
	Initializer *initialize;
	Assignment  *assign;
	Update      *update;
	Criterion   *stopping_criterion;

	KMeansParams():initialize(nullptr), assign(nullptr), update(nullptr), stopping_criterion(nullptr){}
    KMeansParams(Initializer *initializer, Assignment *assignment, Update *update, Criterion *criterion)
            :initialize(initializer), assign(assignment), update(update), stopping_criterion(criterion) {}
};


std::vector<Cluster> k_means_clustering(Dataset& X, int k, KMeansParams& params, bool verbose=false) {
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

#endif