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
	Initializer &initialize;
	Assignment  &assign;
	Update      &update;
	Criterion   &stopping_criterion;

    KMeansParams(Initializer& initializer, Assignment& assignment, Update& update, Criterion& criterion)
            :initialize(initializer), assign(assignment), update(update), stopping_criterion(criterion) {}
};


std::vector<Cluster> k_means_clustering(Dataset& X, int k, KMeansParams& params) {
	std::vector<Cluster> clusters;
int cnt=0;

	clusters = params.initialize(X, k);
	do {
std::cout<<"Iteration "<<++cnt<<std::endl;
		params.assign(X, clusters);
		params.update(X, clusters);
	} while (!params.stopping_criterion.should_stop(clusters));

	return clusters;
}

#endif