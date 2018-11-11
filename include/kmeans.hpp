#include <vector>
#include <tuple>
#include <ApproximateNeighborSearch/NDVector.h>
#include "initializing.h"
#include "assigning.h"
#include "updating.h"

class k_means_params{
public:
	Initializer& initialize;
	Assignment&  assign;
	Update&      update;

	k_means_params(Initializer& initializer, Assignment& assignment, Update& update)
		:initialize(initializer), assign(assignment), update(update) {}

};


std::vector<std::tuple<NDVector, long>>
k_means_clustering(std::map<std::string, NDVector>& X, int k, k_means_params& params)
{
	std::vector<int>      assignment;
	std::vector<NDVector> old_representatives;

	std::vector<NDVector> representatives = params.initialize(k,(int)X[0].dim());
	
	do {

		old_representatives = representatives;

		assignment      = params.assign(X, representatives);
		representatives = params.update(X, assignment, k);

	} while (representatives != old_representatives);




	std::vector<std::tuple<NDVector, long>> result;
	result.reserve(k);

	for (int i=0; i<k; i++){

		auto c = representatives[i];
		long num_points_assigned = std::count(assignment.begin(), assignment.end(), i);

		result.emplace_back(std::make_tuple(c, num_points_assigned));
	}
	return result;
}