#include "HypercubeProjection.h"

class HammingNeighborGenerator{
public:

    HammingNeighborGenerator(hammingNumber number, int maxDistance)
        : maxDistance(maxDistance), neighborsRetrieved(0){

        int len = number.size();
        this->sortedNeighbors.reserve((unsigned long)pow(2,len));
        this->findHammingNeighbors(number, len-1, maxDistance);
    }

    bool hashNext()      {return neighborsRetrieved < sortedNeighbors.size();}
    hammingNumber next() {return sortedNeighbors[neighborsRetrieved++];}

private:
    int neighborsRetrieved;
    int maxDistance;
    std::vector<std::string> sortedNeighbors;


    //todo: check what happens with references
    void findHammingNeighbors(std::string str, int i, int changesLeft) {

        if (changesLeft == 0) {
            sortedNeighbors.push_back(str);
        }
        if (i < 0) return;

        str[i] = str[i] == '0' ? '1' : '0';
        findHammingNeighbors(str, i - 1, changesLeft - 1);
        str[i] = str[i] == '0' ? '1' : '0';
        findHammingNeighbors(str, i - 1, changesLeft);
    }
};



hammingNumber HypercubeProjection::projectPoint(NDVector p){

    std::stringstream ss("");
    for (auto h: H){
        ss << f((*h)(p));
    }

    hammingNumber number = ss.str();
    return number;
}

void HypercubeProjection::insertVector(NDVector p, std::string vectorId){
    hammingNumber number = this->projectPoint(p);
    this->hashTable[number].push_back(vectorId); //TODO: possible seg here
}


void HypercubeProjection::insertDataset(std::unordered_map<std::string, NDVector> X){
    for (auto item : X){
        this->insertVector(item.second, item.first);
    }
}


std::set<std::string> HypercubeProjection::retrieveNeighbors(NDVector p){

    hammingNumber hash = this->projectPoint(p);
    std::vector<std::string> neighborIds = this->hashTable[hash];

    HammingNeighborGenerator generator(hash, this->probes);

    while (generator.hashNext()){

        hammingNumber nearbyHash = generator.next();
        std::vector<std::string> &nearbyIds = hashTable[hash];

        for (auto id : nearbyIds) { //TODO: change to iterator
            neighborIds.push_back(id);
        }
    }

    std::set<std::string> setIds(neighborIds.begin(), neighborIds.end());
    return setIds;
}


EuclideanHypercubeProjection::EuclideanHypercubeProjection(int d_, int M, int probes, int N, int w, int d)
    :HypercubeProjection(d_, M, probes, N){

    for (int i=0; i<d_; i++) H.emplace_back(new hEucl(w,d));
    srand(time(NULL));
}

EuclideanHypercubeProjection::~EuclideanHypercubeProjection(){
    for (int i=0; i<d_; i++){
        delete (hEucl *)H[i];
    }
}


int EuclideanHypercubeProjection::f(int h_p){

    if (h_to_f_index.find(h_p) == h_to_f_index.end()){

        h_to_f_index[h_p] = rand() % 2;
    }

    return h_to_f_index[h_p];
}


CosineHypercubeProjection::CosineHypercubeProjection(int d_, int M, int probes, int N, int d)
    :HypercubeProjection(d_, M, probes, N){

    for (int i=0; i<d_; i++) H.emplace_back(new hCos(d));
}


CosineHypercubeProjection::~CosineHypercubeProjection(){
    for (int i=0; i<d_; i++) delete (hCos *)H[i];
}


