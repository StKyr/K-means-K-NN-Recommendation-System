#include "../include/UserVectors.hpp"


UserVectorDataset::UserVectorDataset(TokenizedDataset &T, SentimentLexicon &A, CryptoLexicon &K) {
    int cnt=0;

    int __num_zeros=0;

    for (std::string& userId : T.getUserIds()){

        auto tweets = T.getUserTweets(userId);
        auto V = createSentimentVector(userId, cnt++, tweets, A, K);

//        if (V.second == NDVector::zero_vector(V.second.dim())){
//            __num_zeros++;
//            V = createSentimentVector(userId, cnt, tweets, A, K);
//        }

        U[V.first] = V.second;

    }

//    std::cerr<<"Found "<<__num_zeros<<" zero vectors" << std::endl;
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

    Dataset newU;

    std::unordered_map<UserId, NDVector>::iterator it;
    for (it=this->U.begin(); it != this->U.end(); ){
        if (it->second != zero){
            newU[it->first] = it->second;
        }else{
        }
        it++;

    }
    this->U = newU;
}

void UserVectorDataset::subtract_average() {
    int cnt=0;
    for (auto &item: this->U){

        double avg = computeAverage(item.first, item.second);
        std::vector<double> coords;
        coords.reserve(item.second.dim());
        for (int i=0; i<item.second.dim(); i++){
            if (this->cryptoMentions[item.first][i]){
                coords.push_back(item.second[i]-avg);
            }else{
                coords.push_back(avg);
            }
        }


        NDVector v(coords, cnt++);
        this->U[item.first] = v;
        this->averages[item.first] = avg;
    }
}

double UserVectorDataset::computeAverage(std::string userId, NDVector& vector){
    double avg=0;
    int num_non_zero = 0;
    for (int i=0; i<vector.dim(); i++){
        if ( this->cryptoMentions[userId][i]){
            avg += vector[i];
            num_non_zero++;
        }
    }
    if (num_non_zero) avg /= num_non_zero;
    return avg;
}




VirtualUserVectorDataset::VirtualUserVectorDataset(std::vector<Cluster>& clusters, TokenizedDataset& T, SentimentLexicon& A, CryptoLexicon& K)
    :UserVectorDataset(){

    const std::string prefix = "cluster";

    int cnt=0;
    for (auto& c: clusters){

        auto pointsIds = c.get_points();
        std::vector<TokensList> clusterTweets;
        clusterTweets.reserve(pointsIds.size());

        for (auto& p: pointsIds){
            clusterTweets.emplace_back( T.getTweet(p) );
        }

        std::stringstream ss("");
        ss << "cluster" << cnt;

        auto V = createSentimentVector(ss.str(), cnt++, clusterTweets, A, K);
        U[V.first] = V.second;
    }
}
