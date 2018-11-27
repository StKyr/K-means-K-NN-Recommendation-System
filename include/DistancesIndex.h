#ifndef ERGASIA2_DISTANCESINDEX_H
#define ERGASIA2_DISTANCESINDEX_H

#include <vector>
#include <ApproximateNeighborSearch/NDVector.h>
#include <unordered_map>
#include <ApproximateNeighborSearch/metrics.h>
#include <iostream>




class DistancesIndex {

public:
    void initialize(double (*distance_func)(NDVector&, NDVector&)) {dist=distance_func;}

    static DistancesIndex& getInstance(){
        static DistancesIndex index;
        return index;
    }

/*    double distance(NDVector& p, NDVector& q){
        if (!dist) throw std::logic_error("Unitialized distance exception");
        total++;

        auto key_str = std::min(p,q).toString()+"-"+std::max(p,q).toString();

        if (index.find(key_str) == index.end()){
            double d = dist(p,q);
            index[key_str] = d;
            num_computed++;
            return d;
        }else{
            num_saved++;
            return index[key_str];
        }
    }*/

    double distance(NDVector& p, NDVector& q){
        if (!dist) throw std::logic_error("Unitialized distance exception");
        return dist(p,q);
    }

    void print_stats(){ std::cout<<"Calculated " << num_computed << " distances and did not calculate "<<num_saved<<" (totaling: " << total << ")."<<std::endl;}

    DistancesIndex(DistancesIndex const&) = delete;
    void operator=(DistancesIndex const&) = delete;
private:
    unsigned long long num_saved = 0;
    unsigned long long total = 0;
    unsigned long long num_computed = 0;

    DistancesIndex() = default;
    double (*dist)(NDVector&, NDVector&) = metrics::euclidean_distance;
    std::unordered_map<std::string, double> index;
};


#endif //ERGASIA2_DISTANCESINDEX_H
