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

    Dataset U;
    std::unordered_map<UserId, std::vector<bool>> cryptoMentions;
private:

    std::pair<std::string, NDVector> createSentimentVector(UserId userId, int vectorCounter, std::vector<TokensList> &tweets, SentimentLexicon& A, CryptoLexicon& K);
    double computeSentiment(TokensList& tweet, SentimentLexicon &A);



    std::unordered_map<UserId, double> averages;


};

UserVectorDataset::UserVectorDataset(TokenizedDataset &T, SentimentLexicon &A, CryptoLexicon &K) {
    int cnt=0;

    int __num_zeros=0;

    for (std::string& userId : T.getUserIds()){

        auto tweets = T.getUserTweets(userId);
        auto V = createSentimentVector(userId, cnt++, tweets, A, K);

        if (V.second == NDVector::zero_vector(V.second.dim())){
            __num_zeros++;
            V = createSentimentVector(userId, cnt, tweets, A, K);
        }

        U[V.first] = V.second;

    }

    std::cerr<<"Found "<<__num_zeros<<" zero vectors" << std::endl;
}



double UserVectorDataset::computeSentiment(TokensList& tweet, SentimentLexicon &A){
    static const double alpha = 15;

    double totalscore = 0;
    for (std::string& token : tweet){
        if (A.exists(token)) totalscore += A.getSentiment(token);
    }

    double Si = totalscore / sqrt(totalscore*totalscore + alpha);
    return Si;
}


std::pair<std::string, NDVector> UserVectorDataset::createSentimentVector(std::string userId, int cnt, std::vector<TokensList> &tweets, SentimentLexicon& A, CryptoLexicon& K){
    double sentiment;

    NDVector sentimentVector = NDVector::zero_vector(K.dim());
    std::vector<bool> mentions(K.dim(), false);


    for (auto &t: tweets) {

        sentiment = computeSentiment(t, A);
        std::vector<double> cryptoVector = K.createCryptoVector(t);
        NDVector vector(cryptoVector, cnt);
        vector *= sentiment;
        sentimentVector += vector;

        std::vector<bool> currMentions = K.createBoolVector(t);
        for (int i=0; i<mentions.size(); i++) mentions[i] = mentions[i] || currMentions[i];
    }

    this->cryptoMentions[userId] = mentions;
    return std::make_pair(userId, sentimentVector);
}



void UserVectorDataset::filterOutZeros() {
    static NDVector zero = NDVector::zero_vector(this->U.begin()->second.dim());
    std::unordered_map<UserId, NDVector>::iterator it;
    for (it=this->U.begin(); it != this->U.end(); ){
        if (it->second == zero){
            this->cryptoMentions.erase(it->first);
            this->averages.erase(it->first);
            it = this->U.erase(it);
        }else{
            it++;
        }
    }
}

#endif //ERGASIA3_USERVECTORS_HPP
