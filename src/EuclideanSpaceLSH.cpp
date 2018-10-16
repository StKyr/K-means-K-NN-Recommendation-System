#include <utility>

#include "EuclideanSpaceLSH.h"
#include <random>
#include <ctime>
#include <cstdlib>



using namespace euclidean;


hFunction::hFunction(int w, int d) :w(w) {
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


int hFunction::operator () (NDVector p){
    double prod = v.dot(p);
    double x    = ( prod + t ) / w;
    int y =  static_cast <int> (std::floor(x));
    int z  = y + ((x > 0 ) ? 1 : -1); //TODO: mention that!!
    return z;
}





EuclideanSpaceLSH::EuclideanSpaceLSH(int L, int tableSize, int M, int k, int d, int w)
:L(L), tableSize(tableSize){

    this->M = M;

    srand((unsigned int)time(nullptr));


    auto l = (unsigned long) L;
    int r;

    hashTables.reserve(l);
    for (int i=0; i<L; i++) hashTables.emplace_back( HashTable(tableSize) );


    H.reserve(l);
    R.reserve(l);
    for (int i=0; i<L; i++){

        H.emplace_back(std::vector<hFunction>());
        R.emplace_back(std::vector<int>());

        H[i].reserve((unsigned long)k);
        R[i].reserve((unsigned long)k);

        for (int j=0; j<k; j++){

            r = rand();    //TODO: change rand() to something of C++

            H[i].emplace_back( hFunction(w, d) );
            R[i].push_back(r);
        }
    }
}




void EuclideanSpaceLSH::insertVector(NDVector p, std::string vectorId){
#ifdef DEBUG
    int hashKey;
    int g_key;


    using namespace std;


    cout << "\nVector: " << p << endl;

    for (int i=0; i<this->L; i++){

        cout << "-HashTable " << i << ":" << endl;


        hashKey = this->phi(p, i);
        g_key   = this->g(p, i);

        cout << "--g(p) is " << g_key << endl;
        cout << "--inserting in " << hashKey << " (φ(i)) position" <<endl;

        this->hashTables[i].insert(hashKey, make_Bucket(g_key, vectorId));
    }

#else

    int hashKey;
    int g_key;


    for (int i=0; i<this->L; i++){
        hashKey = this->phi(p, i);
        g_key   = this->g(p, i);

        Bucket bucket;
        bucket.g_key = g_key;
        bucket.VectorId = vectorId;

        this->hashTables[i].insert(hashKey, bucket);


    }

#endif
}

void EuclideanSpaceLSH::insertDataset(std::unordered_map<std::string, NDVector> X){

    for (auto pair: X) this->insertVector(pair.second, pair.first);

}



std::set<std::string> EuclideanSpaceLSH::retrieveNeighbors(NDVector p){

    std::set<std::string> neighborIds;
    std::vector<std::string> perTableIds;

    int hashKey;
    int g_key;


    for (int i=0; i<this->L; i++){
        hashKey = this->phi(p, i);
        g_key   = this->g(p, i);

        perTableIds = this->hashTables[i].getVectorIds(hashKey, g_key);

        for (const auto &id : perTableIds) neighborIds.insert(id);
    }

    return neighborIds;
}


int EuclideanSpaceLSH::g(NDVector p, int j){

    std::vector<hFunction> &h = this->H[j];

    std::string s = "";
    for (auto h_i : h){
        int val = h_i(p);
         s += (val > 0) ? std::to_string( val ) : std::to_string( -val );
    }

    std::stringstream ss(s);
    int value;
    ss >> value;
    return value;
}



int EuclideanSpaceLSH::phi(NDVector p, int j) {

/*    std::vector<hFunction> &h = this->H[j];
    std::vector<int> &r = this->R[j];

    int result = 0;

    std::vector<hFunction>::iterator it1;
    std::vector<int>::iterator it2;

    for (it1 = h.begin(), it2 = r.begin(); it1 != h.end() && it2 != r.end(); it1++, it2++){
        hFunction &h_i = (*it1);
        int        r_i = (*it2);

        result += ( r_i * h_i(p) ) % this->M;
    }

    result = result % this->M;
    result = result % (this->tableSize / 2);
    result += (this->tableSize / 2);
    return result;
*/

    std::vector<hFunction> &h = this->H[j];

    std::stringstream ss("");

    for (auto h_i : h){
        ss << "," << h_i(p);
    }

    std::string key_str = ss.str();

    std::hash<std::string> hash_str;

    int key = (int) hash_str(key_str); //TODO: check if uint can be casted in int. Check keys types in general
    return key % tableSize;

}








