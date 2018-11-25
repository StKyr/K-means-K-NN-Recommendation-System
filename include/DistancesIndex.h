#ifndef ERGASIA2_DISTANCESINDEX_H
#define ERGASIA2_DISTANCESINDEX_H

#include <vector>
#include <ApproximateNeighborSearch/NDVector.h>
#include <unordered_map>


class DistancesIndex {

public:
    void initialize(double (*distance_func)(NDVector&, NDVector&)) {dist=distance_func;}

    static DistancesIndex& getInstance(){
        static DistancesIndex index;
        return index;
    }

    double distance(NDVector& p, NDVector& q){
/*        auto key_str = p.toString()+"-"+q.toString();

        if (index.find(key_str) == index.end()){
            double d = dist(p,q);
            index[key_str] = d;
            return d;
        }else{
            return index[key_str];
        }*/
        return dist(p,q);
    }

    DistancesIndex(DistancesIndex const&) = delete;
    void operator=(DistancesIndex const&) = delete;
private:
    DistancesIndex() = default;
    double (*dist)(NDVector&, NDVector&);
    std::unordered_map<std::string, double> index;
};


#endif //ERGASIA2_DISTANCESINDEX_H
