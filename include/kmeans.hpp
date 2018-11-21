#ifndef KMEANS_HPP
#define KMEANS_HPP

#include <vector>
#include <tuple>
#include <ApproximateNeighborSearch/NDVector.h>
#include <ApproximateNeighborSearch/VectorCSVReader.h>
#include "initializing.h"
#include "assigning.h"
#include "updating.h"
#include "Silhouette.h"
#include "StoppingCriterion.h"


class Cluster{
public:
                            Cluster         ()                            : centroid(NDVector::zero_vector(1)){}
    explicit                Cluster         (NDVector& centroid)          : centroid(centroid) {}
    void                    assign          (const NDVector& point)       {assigned_points.push_back(point);}
    void                    clear_assignment()                            {assigned_points.clear();}
    NDVector&               get_centroid    ()                            {return centroid;}
    void                    update_centroid (const NDVector& new_centroid){centroid = new_centroid;};
    int                     num_points      () const                      {return assigned_points.size();}
    std::vector<NDVector&>& get_points      ()                            {return assigned_points;}

private:
    NDVector               centroid;
    std::vector<NDVector&> assigned_points;
};





class KMeansParams{
public:
	Initializer &initialize;
	Assignment  &assign;
	Update      &update;
	Criterion   &stopping_criterion;

    KMeansParams(Initializer& initializer, Assignment& assignment, Update& update, Criterion& criterion)
            :initialize(initializer), assign(assignment), update(update), stopping_criterion(criterion) {}
};


std::vector<Cluster> k_means_clustering(Dataset& X, int k, KMeansParams& params, double (*dist)(NDVector&,NDVector&));


/*
typedef std::tuple<std::vector<std::tuple<NDVector, long>>, SilhouetteResults, std::vector<int>> clustering_results;

clustering_results k_means_clustering(std::unordered_map<std::string, NDVector>& X, int k,
        KMeansParams& params, double (*dist)(NDVector&,NDVector&));

std::vector<std::vector<std::string>> complete_assignment(clustering_results results,
        std::map<std::string, NDVector>& X, int k);
*/


#endif