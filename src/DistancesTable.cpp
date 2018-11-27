#include "DistancesTable.h"



void DistancesTable::initialize(size_t size, double (*distance_func)(NDVector&, NDVector&)){
    this->size = size;
    this->dist = distance_func;
    this->table = std::vector<std::vector<double>>();
    table.reserve(size);
    for (int i=0; i<size; i++) table.emplace_back(std::vector<double>(size));

    //table[1][1] = 0; //todo: here for segfault -- remove
}


double DistancesTable::distance(NDVector& p, NDVector& q){
    if (!dist) throw std::logic_error("unitialized distance table!");

    long i = std::min(p.getId(), q.getId());
    long j = std::max(p.getId(), q.getId());

    total++;

    if (i <0 || j< 0){
        num_computed++;
        return dist(p,q);
    }

    if (i==j){
        num_saved++;
        return 0;
    }
    if ( i>= size || j>= size) throw std::range_error("");


    if ((bool)table[i][j]){
        num_saved++;
        return table[i][j];
    }else{
        num_computed++;
        double d = dist(p,q);
        table[i][j] = d;
        return d;
    }

}
void DistancesTable::print_stats(){
    std::cout<<"Calculated " << num_computed << " distances and did not calculate "
    <<num_saved<<" (totaling: " << total << ")."<<std::endl;
}