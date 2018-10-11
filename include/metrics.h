#ifndef ERGASIA1_METRICS_H
#define ERGASIA1_METRICS_H


#include "NDVector.h"

enum supported_metrics {_NULL_, Euclidean, Cosine};

namespace metrics {
    double euclidean_distance(NDVector v, NDVector u);
    double cosine_distance(NDVector v, NDVector u);
};

#endif //ERGASIA1_METRICS_H
