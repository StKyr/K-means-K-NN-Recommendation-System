#include <random>
#include "CosineSimilarityLSH.h"


using namespace cosine;

hFunction::hFunction(int d) {

    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<double> r_coords;
    r_coords.reserve((unsigned long)d);
    std::normal_distribution<> dist_normal(0,1);

    for (int i=0; i<d; i++){
        r_coords.push_back(  dist_normal(gen) );
    }
    this->r = NDVector(r_coords);
}

int hFunction::operator()(NDVector p) {
    return (this->r.dot(p) >= 0) ? 1 : 0;
}







CosineSimilarityLSH::CosineSimilarityLSH(int L, int k, int d) :k(k), L(L) {

    int tableSize = (int)pow(2, k);

    H.reserve(L);
    hashTables.reserve(L);

    for (int i=0; i<L; i++) {
        hashTables.emplace_back(HashTable(tableSize));

        H.emplace_back(std::vector<hFunction>());
        H[i].reserve(k);
        for (int j = 0; j < k; j++) H[i].emplace_back(hFunction(d));
    }
}


int CosineSimilarityLSH::g(NDVector p, int i) {

    int g_val = 0;
    for (auto h_i: this->H[i]){
        int h_val = h_i(p);

        g_val = g_val << 1;
        g_val += h_val;
    }
    return g_val;
}

void CosineSimilarityLSH::insertVector(NDVector p, std::string vectorId) {
    for (int i=0; i<this->L; i++) {
        int hashKey = this->g(p, i);
        this->hashTables[i].insert(hashKey, make_Bucket(hashKey, vectorId));
    }
}

void CosineSimilarityLSH::insertDataset(std::unordered_map<std::string, NDVector> X) {
    for (auto item : X){
        this->insertVector(item.second, item.first);
    }
}

std::set<std::string> CosineSimilarityLSH::retrieveNeighbors(NDVector p) {

    std::set<std::string> res;
    for (int i=0; i<this->L; i++) {
        int hashValue = g(p, i);

        std::vector<std::string> ids = this->hashTables[i].getVectorIds(hashValue, hashValue);

        res.insert(ids.begin(), ids.end());
    }
    return res;
}


