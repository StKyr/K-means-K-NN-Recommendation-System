#include "../../include/ApproximateNeighborSearch/metrics.h"
#include "../../include/ApproximateNeighborSearch/NDVector.h"

double metrics::euclidean_distance(NDVector& v, NDVector& u){
    NDVector diff = v - u;
    double d = diff.norm_sq();
    return d;
}


double metrics::cosine_distance(NDVector& v, NDVector& u){
    if (v == NDVector::zero_vector(v.dim()) || u == NDVector::zero_vector(u.dim())) throw std::logic_error("Nonsensical definition of cosine similarity for zero vectors");

    double cosTheta = v.dot(u) / (v.norm() * u.norm());
    return 1 - cosTheta;
}


