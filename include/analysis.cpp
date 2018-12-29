#include <cassert>
#include "analysis.hpp"
#include "clustering/cluster.hpp"


double computeSentiment(TokensList& tweet, SentimentLexicon &A){
    static const double alpha = 15;

    double totalscore = 0;
    for (std::string& token : tweet){
        if (A.exists(token)) totalscore += A.getSentiment(token);
    }

    double Si = totalscore / sqrt(totalscore*totalscore + alpha);
    return Si;
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
        if (v[i] != std::numeric_limits<double>::infinity()){
            u.push_back(v[i] - mean);
        }else{
            u.push_back(0);
        }
    }

    NDVector normalized_vector(u, v.getId());
    return normalized_vector;
}


std::pair<std::string, NDVector> createSentimentVector(std::string userId, int cnt, std::vector<TokensList> &tweets, SentimentLexicon& A, CryptoLexicon& K){
    double sentiment;

    std::vector<double> u(tweets[0].size(), 0);
    std::vector<bool> mentions(tweets[0].size(), false);

    for (auto &t: tweets) {

        sentiment = computeSentiment(t, A);

        std::vector<bool> cryptoVector = K.createCryptoVector(t);

        for (int i=0; i<mentions.size(); i++){
            mentions[i] = mentions[i] || cryptoVector[i];
            if (cryptoVector[i]){
                u[i] += sentiment;
            }
        }
    }

    for (int i=0; i<mentions.size(); i++){
        if (!mentions[i]) u[i] = std::numeric_limits<double>::infinity();
    }

    NDVector vector(u,cnt);
    vector = normalize(vector);

    return std::make_pair(userId, vector);
}



Dataset *createUsersVectors(TokenizedDataset& T, SentimentLexicon& A, CryptoLexicon& K){

    //todo: return pointer
    //todo: keep track of not mentioned cryptos somehow
    Dataset *U = new Dataset();

    int cnt=0;

    for (std::string& userId : T.getUserIds()){

        auto tweets = T.getUserTweets(userId);
        auto V = createSentimentVector(userId, cnt++, tweets, A, K);

        (*U)[V.first] = V.second;
    }
    return U;
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

        std::stringstream ss(prefix);
        ss << cnt;

        auto V = createSentimentVector(ss.str(), cnt++, clusterTweets, A, K);
        (*C)[V.first] = V.second;
    }

    return C;
}

