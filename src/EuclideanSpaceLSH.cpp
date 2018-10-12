#include "EuclideanSpaceLSH.h"
#include <random>
#include <ctime>
#include <cstdlib>

EuclideanSpaceLSH::EuclideanSpaceLSH(int L, int tableSize, int M, int k, int d, int w)
:L(L), tableSize(tableSize){

    EuclideanSpaceLSH::M = M;

    srand((unsigned int)time(nullptr));


    auto l = (unsigned long) L;
    int r;

    hashTables.reserve(l);
    for (int i=0; i<L; i++) hashTables[i] = HashTable(tableSize);


    H.reserve(l);
    R.reserve(l);
    for (int i=0; i<L; i++){

        H[i].reserve((unsigned long)k);
        R[i].reserve((unsigned long)k);

        for (int j=0; j<k; j++){

            while ((r = rand()) <= 0);    //TODO: change rand() to something of C++

            H[i].emplace_back( hFunction(w, d) );
            R[i].push_back(r);
        }
    }
}




void EuclideanSpaceLSH::insertVector(NDVector p, std::string vectorId){

    int hashKey;
    int g_key;

    for (int i=0; i<this->L; i++){
        hashKey = this->phi(p, i);
        g_key   = this->g(p, i);

        this->hashTables[i].insert(hashKey, make_Bucket(g_key, vectorId));
    }
}







std::set<std::string> EuclideanSpaceLSH::retireveNeighbors(NDVector p){

    std::set<std::string> neighborIds;
    std::vector<std::string> perTableIds;

    int hashKey;
    int g_key;


    for (int i=0; i<this->L; i++){
        hashKey = this->phi(p, i);
        g_key   = this->g(p, i);

        perTableIds = this->hashTables[i].getVectorIds(hashKey, g_key);

        for (auto id : perTableIds) neighborIds.insert(id);
    }

    return neighborIds;
}


int EuclideanSpaceLSH::g(NDVector p, int j){

    std::vector<hFunction> &h = this->H[j];

    std::stringstream ss("");
    for (auto h_i : h){
        ss << h_i(p);
    }

    int value;
    ss >> value;
    return value;
}



int EuclideanSpaceLSH::phi(NDVector p, int j){

    std::vector<hFunction> &h = this->H[j];
    std::vector<int> &r = this->R[j];

    int result = 0;

    std::vector<hFunction>::iterator it1;
    std::vector<int>::iterator it2;

    for (it1 = h.begin(), it2 = r.begin(); it1 != h.end() && it2 != r.end(); it1++, it2++){
        hFunction &h_i = (*it1);
        int        r_i = (*it2);

        result += ( r_i * h_i(p) ) % EuclideanSpaceLSH::M;
    }

    result = result % EuclideanSpaceLSH::M;
    result = result % this->tableSize;
    return result;
}







EuclideanSpaceLSH::hFunction::hFunction(int w, int d) :w(w) {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()


    std::uniform_int_distribution<> dist_uniform(0,w);
    this->t = dist_uniform(gen);


    std::vector<double> v_coords;
    v_coords.reserve((unsigned long)d);
    std::normal_distribution<> dist_normal(0,1);

    for (int i=0; i<d; i++){
        v_coords.push_back(  dist_normal(gen) );
    }
    this->v = NDVector(v_coords);
}


int EuclideanSpaceLSH::hFunction::operator () (NDVector p){
    double x =   ( p.dot(v) + t ) / w;
    return static_cast <int> (std::floor(x));
}

std::vector<std::string> EuclideanSpaceLSH::HashTable::
getVectorIds(int position, int originalKey){
    std::vector<std::string> vectorIds;

    for (auto item : this->array[position]){
        if (item.g_key == originalKey){
            vectorIds.push_back(item.VectorId);
        }
    }
    return vectorIds;
}