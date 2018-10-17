#include <random>
#include "CosineSimilarityLSH.h"




hCos::hCos(int d) {

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

int hCos::operator()(NDVector &p) {
    return (this->r.dot(p) >= 0) ? 1 : 0;
}







CosineSimilarityLSH::CosineSimilarityLSH(int L, int k, int d) {
    this->L = L;

    int tableSize = (int)pow(2, k);

    H.reserve(L);
    hashTables.reserve(L);

    tableSize = (int) pow(2,k);
    for (int i=0; i<L; i++) {
        hashTables.emplace_back(HashTable(tableSize));

        H.emplace_back(std::vector<hFunction *>());
        H[i].reserve(k);
        for (int j = 0; j < k; j++) H[i].emplace_back(new hCos(d));
    }
}


int CosineSimilarityLSH::g(NDVector p, int i){

    int g_val = 0;
    for (auto &h_i: this->H[i]){
        int h_val = h_i->operator()(p);

        g_val = g_val << 1;
        g_val += h_val;
    }
    return g_val;
}

void CosineSimilarityLSH::insertVector(NDVector p, std::string vectorId) {
    for (int i=0; i<this->L; i++) {
        int hashKey = this->g(p, i);
        std::stringstream ss("");
        ss << hashKey;
        std::string s = ss.str();
        Bucket bucket = {.g_key = s, .VectorId = vectorId};
        this->hashTables[i].insert(hashKey, bucket);
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
        std::stringstream ss("");
        ss << hashValue;
        std::string s = ss.str();

        std::vector<std::string> ids = this->hashTables[i].getVectorIds(hashValue, s);

        res.insert(ids.begin(), ids.end());
    }
    return res;
}


