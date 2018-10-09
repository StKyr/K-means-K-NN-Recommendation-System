#include "NDVector.h"

NDVector::NDVector(){
    this->coords = std::vector<double>();
}

NDVector::NDVector(std::vector<double>& coords){
    this->coords = coords;
}

NDVector::NDVector(std::initializer_list<double> list){
    for (auto x : list) this->coords.push_back(x);
}

unsigned long NDVector::dim(){
    if (this->coords.empty()) throw std::range_error("Unitialized Vector Exception");

    return this->coords.size();
}

double NDVector::norm(unsigned long p){
    std::vector<double> xs = std::vector<double>(this->coords.size());

    std::transform(this->coords.begin(), this->coords.end(), xs.begin(),
                   [=](double x) -> double {return pow(x,p);} );

    return pow(std::accumulate(xs.begin(), xs.end(), (double)0), (double)1/p);
}


