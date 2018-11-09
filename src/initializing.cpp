#include "initializing.h"
#include <limits>
#include <random>

std::vector<NDVector> RandomInitializer::operator () (int k, int l){

    std::vector<NDVector> representatives;
    representatives.reserve(k);
    for (int i=0; i<k; i++){
        representatives.emplace_back(NDVector::random_vector(l));
    }
    return representatives;
}