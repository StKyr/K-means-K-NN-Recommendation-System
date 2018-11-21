#include "kmeans.hpp"


clustering_results k_means_clustering(Dataset& X, int k, KMeansParams& params, double (*dist)(NDVector&,NDVector&))
{
    std::vector<int>      assignment;
    std::vector<NDVector> old_representatives;

    std::vector<NDVector> representatives = params.initialize(X, k,(int)X[0].dim());
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

    SilhouetteResults silhouettes = compute_silhouettes(X, assignment, representatives, dist);
    return std::make_tuple(result, silhouettes, assignment);
}

std::vector<std::vector<std::string>> complete_assignment(clustering_results results, std::map<std::string, NDVector>& X, int k){
    std::vector<int>& final_assignment = std::get<2>(results);

    std::vector<std::vector<std::string>> classification;
    for (int i=0; i<k; i++) classification.emplace_back(std::vector<std::string>());

    int i=0;
    for (auto& item : X){
        std::string vectorId = item.first;
        int cluster = final_assignment[i];
        classification[i].push_back(vectorId);
    }
    return classification;
}

// ---------------------------------------------------------------------------------




std::vector<Cluster> k_means_clustering(Dataset& X, int k, KMeansParams& params, double (*dist)(NDVector&,NDVector&))
{

    std::vector<Cluster> clusters = params.initialize(X, k,(int)X[0].dim());
    do {
        params.assign();
        params.update();

    } while (! stopping_criterion.should_stop());

    return clusters;

}