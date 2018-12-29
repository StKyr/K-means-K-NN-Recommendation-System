#ifndef ERGASIA2_DISTANCESTABLE_HPP
#define ERGASIA2_DISTANCESTABLE_HPP


#include <vector>
#include <unordered_map>
#include <iostream>
#include "../ApproximateNeighborSearch/NDVector.h"
#include "../ApproximateNeighborSearch/metrics.h"


/**
 * A "global" entity that stores previously computed distance calculations. It is implemented as a Singleton and
 * it makes use of the unique incremental Id all vectors of the dataset have to store distances in a 2d array of size N.
 * All distance computations should be this module's responsibility.
 */
class DistancesTable {

public:
    static DistancesTable& getInstance(){
        static DistancesTable table;
        return table;
    }

    void initialize(size_t size, double (*distance_func)(NDVector&, NDVector&)=metrics::euclidean_distance);
    double distance(NDVector& p, NDVector& q);
    void print_stats(); // for debuging
    void reset();



    DistancesTable(DistancesTable const&) = delete;
    void operator=(DistancesTable const&) = delete;
private:

    double (*dist)(NDVector&, NDVector&) = nullptr;
    std::vector<std::vector<double>> table;
    size_t size = 0;
    unsigned long long num_saved = 0;
    unsigned long long total = 0;
    unsigned long long num_computed = 0;


    DistancesTable() = default;
    ~DistancesTable() = default;

};



#endif //ERGASIA2_DISTANCESTABLE_HPP
