#include <iostream>
#include "HypercubeProjection.h"

bool warningDisplayed = false;

class HammingNeighborGenerator{
public:

    HammingNeighborGenerator(hammingNumber number, int maxDistance)
        : maxDistance(maxDistance), neighborsRetrieved(0){

        int len = number.size();


        if (0){//(maxDistance == 1){

            this->sortedNeighbors.reserve(len);

            for (int i=0; i<len; i++){
                std::string s = number;
                s[i] = (s[i] == '0') ? '1' : '0';
                sortedNeighbors.push_back(s);
            }

        }else {


            try {
                this->sortedNeighbors.reserve(len * maxDistance);
            } catch (std::bad_alloc &e) {
                if (!warningDisplayed) {
                    std::cerr << "Warning: Hypercube consists of " << len * maxDistance
                              << " vertices. Unable to produce all hamming vertices."
                              << "Search will be limited to hamming vertices with maximum distance of 10."
                              << std::endl;
                    warningDisplayed = true;
                }
                this->maxDistance = 10;
                this->sortedNeighbors.reserve(len * maxDistance);
            }
            this->findHammingNeighbors(number, len - 1, maxDistance);
        }
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

    int neighborsInserted = 0;
    int probesChecked = 1;
    hammingNumber hash = this->projectPoint(p);


    std::set<std::string> neighborIds;

    for (auto &id : this->hashTable[hash]){
        if (neighborsInserted++ >= this->M) return neighborIds;

        neighborIds.insert(id);
    }

    HammingNeighborGenerator generator(hash, 1); //todo: change to something else
    while (generator.hashNext()){
        if (probesChecked++ >= this->probes) return neighborIds;

        hammingNumber nearbyHash = generator.next();

        for (auto &id : hashTable[hash]) {

            neighborIds.insert(id);

            if (neighborsInserted++ >= this->M) return  neighborIds;
        }
    }

    return neighborIds;
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


