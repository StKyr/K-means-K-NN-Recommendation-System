#ifndef ERGASIA3_USERVECTORS_HPP
#define ERGASIA3_USERVECTORS_HPP


#include "Lexicon.hpp"
#include "Datasets.hpp"
#include "ApproximateNeighborSearch/VectorCSVReader.h"

typedef std::string UserId;

class UserVectorDataset {
public:
    UserVectorDataset(TokenizedDataset& T, SentimentLexicon& A, CryptoLexicon& K);
    void filterOutZeros();
    void subtract_average();

    Dataset U;
    Dataset U_fixed;
    std::unordered_map<UserId, std::vector<bool>> cryptoMentions;
    std::unordered_map<UserId, double> averages;

private:
    double computeAverage(std::string userId, NDVector& vector);
    std::pair<std::string, NDVector> createSentimentVector(UserId userId, int vectorCounter, std::vector<TokensList> &tweets, SentimentLexicon& A, CryptoLexicon& K);
    double computeSentiment(TokensList& tweet, SentimentLexicon &A);
};



#endif //ERGASIA3_USERVECTORS_HPP
