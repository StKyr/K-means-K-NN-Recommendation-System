#ifndef ERGASIA3_USERVECTORS_HPP
#define ERGASIA3_USERVECTORS_HPP


#include "Lexicon.hpp"
#include "Datasets.hpp"
#include "ApproximateNeighborSearch/VectorCSVReader.h"
#include "clustering/cluster.hpp"

typedef std::string UserId;

class UserVectorDataset {
public:
    UserVectorDataset() = default;
    UserVectorDataset(TokenizedDataset& T, SentimentLexicon& A, CryptoLexicon& K);
    void filterOutZeros();
    void subtract_average();
    size_t dim() {return U.begin()->second.dim();}

    virtual double sim(double dist) {return 1-dist;}

    Dataset U;
    std::unordered_map<UserId, std::vector<bool>> cryptoMentions;
    std::unordered_map<UserId, double> averages;

protected:
    double computeAverage(std::string userId, NDVector& vector);
    std::pair<std::string, NDVector> createSentimentVector(UserId userId, int vectorCounter, std::vector<TokensList> &tweets, SentimentLexicon& A, CryptoLexicon& K);
    double computeSentiment(TokensList& tweet, SentimentLexicon &A);
};


class VirtualUserVectorDataset : public UserVectorDataset{
public:
    VirtualUserVectorDataset(std::vector<Cluster>& clusters, TokenizedDataset& T, SentimentLexicon& A, CryptoLexicon& K);
    double sim(double dist) override {if (dist != 0) return 1/dist; else return 0;}
private:
};


#endif //ERGASIA3_USERVECTORS_HPP
