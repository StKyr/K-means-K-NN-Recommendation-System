#include <ApproximateNeighborSearch/metrics.h>
#include <ApproximateNeighborSearch/NDVector.h>

double metrics::euclidean_distance(NDVector& v, NDVector& u){
    NDVector diff = v - u;
    double d = diff.norm();
    return d;
}


double metrics::cosine_distance(NDVector& v, NDVector& u){
    if (v.norm() == 0 || u.norm() == 0) throw std::logic_error("Nonsensical definition of cosine similarity for zero vectors");

    double cosTheta = v.dot(u) / (v.norm() * u.norm());
    return 1 - cosTheta;
}


