#include "StoppingCriterion.h"


bool OrCriteriaOrchestrator::should_stop(std::vector<Cluster>& clustering) {
    for (auto& c: criteria) if (c.should_stop(clustering)) return true;
    return false;
}

bool AndCriteriaOrchestrator::should_stop(std::vector<Cluster>& clustering) {
    for (auto& c: criteria) if (!c.should_stop(clustering)) return false;
    return true;
}

bool CentroidsDisplacement::should_stop(std::vector<Cluster> &clusters) {

    if (previous.empty()){
        for (auto c: clusters) previous.emplace_back(c);
        return false;
    }

    for (int i=0; i<previous.size(); i++){

        if (clusters[i].get_centroid() != previous[i].get_centroid()){
            previous.clear();  for (auto c: clusters) previous.emplace_back(c);
            return false;
        }
    }
    previous.clear();  for (auto c: clusters) previous.emplace_back(c);
    return true;
}