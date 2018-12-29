#include <cmath>
#include "../../include/clustering/StoppingCriterion.h"
#include "../../include/clustering/DistancesTable.h"


bool OrCriteriaOrchestrator::should_stop(std::vector<Cluster>& clustering) {
    bool criterion_found = false;
    for (auto& c: criteria) if (c->should_stop(clustering)) {
        if (message.str().empty()) {
            message << c->what();
        }else{
            message << " OR " << c->what();
        }
        criterion_found = true;
    }
    return criterion_found;
}


bool AndCriteriaOrchestrator::should_stop(std::vector<Cluster>& clustering) {
    for (auto& c: criteria) if (!c->should_stop(clustering)) return false;
    return true;
}

std::string AndCriteriaOrchestrator::what() {
    message << criteria[0]->what();
    for (int i=1; i<criteria.size(); i++) message << " AND " << criteria[i]->what();
    return message.str();
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
    return (passive)?false:true;
}

std::string CentroidsDisplacement::state() {
    std::string s = "centroids: ";
    for (auto& c: this->previous) s += str(c.get_centroid())+",";
    return s;
}

bool ToleranceCentroidsDisplacement::should_stop(std::vector<Cluster> &clusters) {
    if (passive) return false;

    if (previous.empty()){
        for (auto c: clusters) previous.emplace_back(c);
        return false;
    }

    double total_variance = 0;
    for (int i=0; i<previous.size(); i++){

        auto displacement_vector = clusters[i].get_centroid() - previous[i].get_centroid();
        double variance = displacement_vector.norm();
        total_variance += variance;
    }

    previous.clear();  for (auto c: clusters) previous.emplace_back(c);

    if (previous_displacement != 0){
        if ( total_variance / previous_displacement < tol){
            previous_displacement = total_variance;
            return (passive)?false: true;
        }else{
            previous_displacement = total_variance;
            return false;
        }
    }else{
        previous_displacement = total_variance;
        return false;
    }
}

double objective_function(Dataset* X, std::vector<Cluster> &clusters){

    double obj = 0;
    for (auto& C: clusters){
        double cluster_obj = 0;
        for (auto& id: C.get_points()){
            cluster_obj += DistancesTable::getInstance().distance(C.get_centroid(), (*X)[id]);
        }
        obj += cluster_obj;
    }
    return obj;
}


bool ObjectiveFunctionMinimization::should_stop(std::vector<Cluster> &clusters) {
    double objective = objective_function(&(this->X), clusters);
    if (previous_objective == 0){
        previous_objective = objective;
        return false;
    }

    if (fabs(previous_objective-objective) / previous_objective < tol){
        previous_objective = objective;
        return (passive)?false:true;
    }else{
        previous_objective = objective;
        return false;
    }
}

std::string CriteriaOrchestrator::state() {
    std::string s;
    for (auto& c: criteria) s+= c->state() + " | ";
    return s;
}

bool AssignmentChanges::should_stop(std::vector<Cluster> &clusters) {
    if (clusters_changed == -1){
        previous_clusters = clusters;
        clusters_changed = 0;
        return false;
    }

    clusters_changed=0;
    for (int i=0; i<clusters.size(); i++){
        if (clusters[i].get_points() != previous_clusters[i].get_points()) clusters_changed++;
    }
    return (passive)?false:clusters_changed == 0;
}
