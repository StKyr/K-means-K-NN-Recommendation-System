
#include <iterator>
#include <cassert>
#include "../include/recommendations.hpp"
#include "../include/ApproximateNeighborSearch/VectorCSVReader.h"
#include "../include/ApproximateNeighborSearch/CosineSimilarityLSH.h"
#include "../include/Lexicon.hpp"
#include "../include/ApproximateNeighborSearch/NearestNeighborSearch.h"
#include "../include/ProximitySearch.hpp"
#include "../include/UserVectors.hpp"

bool compareDesc(std::pair<double, int> first, std::pair<double, int> second){
    if (first.first == NAN || first.first == -NAN ) return false;
    if (second.first == NAN || second.second == -NAN) return true;
    return first.first > second.first;
}


std::vector<std::pair<double,int>> rate(const std::string &userId, NDVector &u, std::vector<bool>& coinsMentioned,
        std::vector<std::pair<std::string, double>>& neighbors, UserVectorDataset &dataset){

    std::vector<std::pair<double,int>> ratings;

    double z=0;
    for (auto &pair: neighbors){
        double term = dataset.sim(pair.second);
        if (term < 0) term *= -1;
        z+= term;
    }
   if (z) {
       z = 1 / z;
   }else{
       return ratings;
   }

    for (int i=0; i<u.dim(); i++){

        if (coinsMentioned[i]) continue;

        double R=0;
        for (auto &pair: neighbors) {
            if (pair.first == userId) continue;

            double sim = dataset.sim(pair.second);
            NDVector v = dataset.U[pair.first];
            double r = v[i];
            R += sim*(r-dataset.averages[pair.first]);
        }
        R = z*R;
        ratings.emplace_back(std::make_pair(R,i));
    }
    return ratings;
}






std::vector<std::vector<std::string>> userBasedSuggestions(int num,
                                                           UserVectorDataset &users,
                                                           UserVectorDataset *suggestionDataset,
                                                           int P,
                                                           CryptoLexicon &K,
                                                           enum SuggestionType suggestionType,
                                                           Evaluation *evaluation ){

    std::vector<std::vector<std::string>> results;

    std::set<std::string>                       neighbors;
    std::vector<std::pair<std::string, double>> nearest_neighbors;
    std::vector<std::pair<double,int>>          ratings;
    std::vector<std::string>                    userResults;
    Dataset neighborVectors;

    bool shouldEvaluate = evaluation != nullptr;


    if (!suggestionDataset) suggestionDataset = &users;

    ProximitySearch *search;
    if (suggestionType == nearest_neighbor) search = new NearesrNeighborProximitySearch(P, suggestionDataset->U);
    else                                    search = new ClusteringProximitySearch(P, suggestionDataset->U);


    if (shouldEvaluate){
        evaluation->dataset = suggestionDataset;
        evaluation->search = search;
        evaluation->ratings_function = rate;
    }



int __cnt = 0;

    for (auto& item: users.U){


//if (__cnt++ == 100) return results;


        nearest_neighbors = (*search)(item.first, item.second);
        ratings = rate(item.first, item.second, users.cryptoMentions[item.first], nearest_neighbors, *suggestionDataset);
        std::sort(ratings.begin(), ratings.end(), compareDesc);
        userResults.clear();
        for (int i=0; i<num; i++) userResults.push_back(K.getCrypto(ratings[i].second));
        results.push_back(userResults);



//std::cout << "User " << item.first << ": {" << nearest_neighbors.size() << " similar users, mentioned cryptos:\t";
//for (int i=0; i<users.dim(); i++) if (users.cryptoMentions[item.first][i]) std::cout<<K.getCrypto(i)<<"\t";
//std::cout<<"} ------->\t";
//for (int i=0; i<num; i++) std::cout<<K.getCrypto(ratings[i].second)<<"("<<ratings[i].first<<")\t";
//std::cout<<std::endl;


        if (shouldEvaluate) evaluation->evaluateUser(item.first, item.second, users.cryptoMentions[item.first], users.averages[item.first]);
    }

    delete search;
    return results;
}





std::vector<std::vector<std::string>> recommend(int num, UserVectorDataset &U, int P, CryptoLexicon &K, enum SuggestionType suggestionType, Evaluation *evaluation){
    return userBasedSuggestions(num, U, nullptr, P, K, suggestionType, evaluation);
}

std::vector<std::vector<std::string>> recommend(int num, UserVectorDataset &U, VirtualUserVectorDataset &C, int P, CryptoLexicon &K, enum SuggestionType suggestionType, Evaluation *evaluation){
    return userBasedSuggestions(num, U, &C, P, K, suggestionType, evaluation);
}





