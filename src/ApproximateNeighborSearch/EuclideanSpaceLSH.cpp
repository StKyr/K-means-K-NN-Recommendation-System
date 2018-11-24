#include <utility>

#include <random>
#include <ctime>
#include <cstdlib>
#include <ApproximateNeighborSearch/NDVector.h>
#include <ApproximateNeighborSearch/EuclideanSpaceLSH.h>

hEucl::hEucl(int w, int d) :w(w) {
    std::random_device rd;
    std::mt19937 gen(rd());


    std::uniform_int_distribution<> dist_uniform(0,w);
    while ((this->t = dist_uniform(gen)) == w) ;

    std::vector<double> v_coords;
    v_coords.reserve((unsigned long)d);
    std::normal_distribution<> dist_normal(0,1);

    for (int i=0; i<d; i++){
        v_coords.push_back(  dist_normal(gen) );
    }
    this->v = NDVector(v_coords);
}


int hEucl::operator () (NDVector &p){
    double prod = v.dot(p);                          // code is in pieces for easy debug
    double x    = ( prod + t ) / w;
    int    y    = static_cast <int> (std::floor(x));
    return y;
}





EuclideanSpaceLSH::EuclideanSpaceLSH(int L, int tableSize, int k, int d, int w)
{

    this->L = L;
    this->tableSize = tableSize;

    srand((unsigned int)time(nullptr));


    auto l = (unsigned long) L;

    hashTables.reserve(l);
    for (int i=0; i<L; i++) hashTables.emplace_back( HashTable(tableSize) );


    H.reserve(l);
    for (int i=0; i<L; i++){

        H.emplace_back(std::vector<hFunction *>());

        H[i].reserve((unsigned long)k);

        for (int j=0; j<k; j++){
            H[i].emplace_back(new  hEucl(w, d) );
        }
    }
}




void EuclideanSpaceLSH::insertVector(NDVector &p, std::string vectorId){


    unsigned hashKey;
    std::string g_key;

    for (int i=0; i<this->L; i++){
        hashKey = this->phi(p, i);
        g_key   = this->g(p, i);

        Bucket bucket;
        bucket.g_key = g_key;
        bucket.VectorId = vectorId;

        this->hashTables[i].insert(hashKey, bucket);

    }
}

void EuclideanSpaceLSH::insertDataset(std::unordered_map<std::string, NDVector> &X){

    for (auto pair: X) this->insertVector(pair.second, pair.first);

}



std::set<std::string> EuclideanSpaceLSH::retrieveNeighbors(NDVector &p){

    std::set<std::string> neighborIds;
    std::vector<std::string> perTableIds;

    unsigned hashKey;
    std::string g_key;

    for (int i=0; i<this->L; i++){
        hashKey = this->phi(p, i);
        g_key = g(p,i);


        perTableIds = this->hashTables[i].getVectorIds(hashKey, g_key);

        for (const auto &id : perTableIds) neighborIds.insert(id);
    }

    return neighborIds;
}


std::string EuclideanSpaceLSH::g(NDVector p, int j){

    std::vector<hFunction*> &h = this->H[j];
    std::stringstream ss("{");

    for (auto &h_i : h){
        ss << (*h_i)(p) << ",";
    }
    ss << "}";

    std::string key_str = ss.str();
    return key_str;
}



unsigned EuclideanSpaceLSH::phi(NDVector p, int j) {

    std::string key_str = g(p,j);

    std::hash<std::string> hash_str;

    auto key = (unsigned) hash_str(key_str);
    return key % tableSize;
}








