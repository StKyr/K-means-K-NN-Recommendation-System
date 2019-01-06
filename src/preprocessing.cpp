#include <cassert>
#include <cmath>
#include "../include/preprocessing.hpp"
#include "../include/clustering/cluster.hpp"


double computeSentiment(TokensList& tweet, SentimentLexicon &A){
    static const double alpha = 15;

    double totalscore = 0;
    for (std::string& token : tweet){
        if (A.exists(token)) totalscore += A.getSentiment(token);
    }

    double Si = totalscore / sqrt(totalscore*totalscore + alpha);
    return Si;
}

std::unordered_map<std::string, double>* computeAverageScores(Dataset &X){
    auto averages = new std::unordered_map<std::string, double>();
    for (auto &item: X){
        double avg=0;
        int num_non_zero = 0;
        for (int i=0; i<item.second.dim(); i++){
            if (item.second[i] != 0 && item.second[i] != std::numeric_limits<double>::infinity() ){
                avg += item.second[i];
                num_non_zero++;
            }
        }
        if (num_non_zero) avg /= num_non_zero;
        (*averages)[item.first] = avg;
    }
    return averages;
}



NDVector normalize(NDVector &v){ //todo: normalize elsewhere
    double sum = 0;
    double count = 0;

    for (int i=0; i<v.dim(); i++ ){
        if (v[i] != std::numeric_limits<double>::infinity()){
            sum += v[i];
            count++;
        }
    }
    assert(count != 0);

    double mean = sum / count;
    std::vector<double> u;

    for (int i=0; i<v.dim(); i++ ){
        if (v[i] != 0){
            //double val = v[i]/mean;
            u.push_back(v[i]-mean);
        }else{
            u.push_back(0);
        }
    }


    //for (int i=0; i<u.size(); i++) u[i]++;


    NDVector normalized_vector(u, v.getId());
/*if (normalized_vector == NDVector::zero_vector(normalized_vector.dim())){
    std::cerr<<"Found zero normalized vector from non zero unnormalized one"<<std::endl;
}*/

    return normalized_vector;
}


std::pair<std::string, NDVector> createSentimentVector(std::string userId, int cnt, std::vector<TokensList> &tweets, SentimentLexicon& A, CryptoLexicon& K){
    double sentiment;


/*if (cnt==7){
    cnt=7;
}*/
    NDVector sentimentVector = NDVector::zero_vector(K.dim());
    for (auto &t: tweets) {

        sentiment = computeSentiment(t, A);
        std::vector<double> cryptoVector = K.createCryptoVector(t);
        NDVector vector(cryptoVector, cnt);
        vector *= sentiment;
        sentimentVector += vector;
    }

   /* for (int i=0; i<mentions.size(); i++){
        if (!mentions[i]) u[i] = std::numeric_limits<double>::infinity();
    }*/

/*bool has_crypto=false; for(auto x:mentions) has_crypto = has_crypto||x; if (!has_crypto){
    std::cerr<<"found user with no mentions"<<std::endl;
    for (auto list:tweets) for (auto word:list) if (word=="@ripple") std::cerr<<word<<": "<<K.exists(word)<<std::endl;
    //K.print();
    std::cerr<<"@ripple in K: "<<K.exists("@ripple")<<std::endl;
} else{ std::cerr<<".";}*/

    return std::make_pair(userId, sentimentVector);
}



Dataset *createUsersVectors(TokenizedDataset& T, SentimentLexicon& A, CryptoLexicon& K){

    //todo: keep track of not mentioned cryptos somehow
    Dataset *U = new Dataset();

    int cnt=0;

int __num_zeros=0;

    for (std::string& userId : T.getUserIds()){

        auto tweets = T.getUserTweets(userId);
        auto V = createSentimentVector(userId, cnt++, tweets, A, K);

if (V.second == NDVector::zero_vector(V.second.dim())){
__num_zeros++;
    V = createSentimentVector(userId, cnt, tweets, A, K);
}

        (*U)[V.first] = V.second;
    }

std::cerr<<"Found "<<__num_zeros<<" zero vectors" << std::endl;
    return U;
}


Dataset *normalizeDataset(Dataset &X){

    Dataset *X_normed = new Dataset();
    X_normed->reserve(X.size());

    for (auto& pair: X) (*X_normed)[pair.first] = normalize(pair.second);
    return X_normed;
}

Dataset *createCentroidsVectors(std::vector<Cluster>& clusters, TokenizedDataset& T, SentimentLexicon& A, CryptoLexicon& K){

    Dataset *C = new Dataset();
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
        (*C)[V.first] = V.second;
    }

    return C;
}


Dataset * filterOutZeros(Dataset &X){
    Dataset *Y = new Dataset();
    Y->reserve(X.size());

    for (auto &item: X){
        if (item.second != NDVector::zero_vector(item.second.dim())){
            (*Y)[item.first] = item.second;
        }
    }
    return Y;
}

