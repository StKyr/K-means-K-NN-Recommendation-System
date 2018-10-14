#include "HypercubeProjection.h"

class HammingNeighborGenerator{
public:

    HammingNeighborGenerator(hammingNumber number, int maxNeighbors, int maxDistance)
        :number(number), maxNeighbors(maxNeighbors), maxDistance(maxDistance), neighborsRetrieved(0){

        this->sortedNeighbors.reserve(maxNeighbors);
        int len = number.size();
        this->findHammingNeighbors(number, len-1, maxDistance);
    }

    bool hashNext()      {return neighborsRetrieved < sortedNeighbors.size();}
    hammingNumber next() {return sortedNeighbors[neighborsRetrieved++];}

private:
    hammingNumber number;
    int maxNeighbors;
    int maxDistance;
    int neighborsRetrieved;
    std::vector<std::string> sortedNeighbors;


    //todo: check what happens with references
    void findHammingNeighbors(std::string str, int i, int changesLeft) {
        if (sortedNeighbors.size() == maxNeighbors) return;

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