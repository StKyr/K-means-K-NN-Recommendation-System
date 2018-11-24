#ifndef ERGASIA2_DISTANCESINDEX_H
#define ERGASIA2_DISTANCESINDEX_H

#include <vector>
#include <map>
#include <ApproximateNeighborSearch/NDVector.h>


class DistancesIndex {

public:
    explicit DistancesIndex(double (*distance_func)(NDVector&, NDVector&)):dist(distance_func){}

    double get_distance(NDVector& p, NDVector& q){
        auto key_str = p.toString()+"-"+q.toString();

        if (index.find(key_str) == index.end()){
            double d = dist(p,q);
            index[key_str] = d;
            return d;
        }else{
            return index[key_str];
        }
    }

private:
    double (*dist)(NDVector&, NDVector&);
    std::map<std::string, double> index;
};


#endif //ERGASIA2_DISTANCESINDEX_H
