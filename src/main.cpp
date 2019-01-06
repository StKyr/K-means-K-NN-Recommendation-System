#include <iostream>
#include <vector>
#include "../include/Lexicon.hpp"
#include "../include/Datasets.hpp"
#include "../include/preprocessing.hpp"
#include "../include/ApproximateNeighborSearch/VectorCSVReader.h"
#include "../include/clustering/clustering.h"
#include "../include/recommendations.hpp"
#include "../include/UserVectors.hpp"


namespace  Params{
    int P=20;
    int K=200;
    std::string cryptos_file="../input/coins_queries.csv";
    std::string lexicon_file="../input/vader_lexicon.csv";
    std::string tweets_file="../input/tweets_dataset_small.csv";
    std::string input_file ="../input/twitter_dataset_small_v2.csv";
}



void printSuggestions(std::vector<std::vector<std::string>> &suggestions, Dataset &U, std::string algorithm){
    std::cout << "Method: "<<algorithm<<std::endl;
    int i=0;
    for (auto &item: U){
        std::cout << "User: "<<item.first<<" | ";
        for (auto &crypto: suggestions[i]){
            std::cout << crypto << ", ";
        }
        std::cout<<std::endl;
    }
}

void printUsers(Dataset &U, CryptoLexicon &K);

int main() {
    TokenizedDataset T(Params::tweets_file,'\t');
    TfidfDataset     X(Params::input_file,',');
    SentimentLexicon A(Params::lexicon_file,'\t');
    CryptoLexicon    K(Params::cryptos_file,'\t');
    Dataset         *U = createUsersVectors(T, A, K);
    Dataset         *U2 = filterOutZeros(*U);
    Dataset         *U_normalized = normalizeDataset(*U2);

    UserVectorDataset uvd(T, A, K);
    uvd.filterOutZeros();
    uvd.subtract_average();


   printUsers(*U_normalized, K);

/*    auto             clusters = vectorizedDatasetClustering(X, Params::K);
    Dataset         *C = createCentroidsVectors(clusters, T, A, K);*/



    std::vector<std::vector<std::string>> suggestions;


    suggestions = userBasedSuggestions(5,uvd, Params::P, K);
 //   suggestions = userBasedSuggestions(5, *U2, *U_normalized, Params::P, K);
//    printSuggestions(suggestions, *U, "P-range Neighbor Search of U");

/*
    suggestions = userBasedSuggestions(2, U, "kmeans", Params::K);
    printSuggestions(suggestions, "K-means clustering search of U");
*/

    return 0;
}


void printUsers(Dataset &U, CryptoLexicon &K){

    for (auto &item: U){

        std::cout << "User "<< item.first << "\t|Cryptos/Sentiments: ";

        for (int i=0; i<item.second.dim(); i++){
            if (item.second[i] != 0){
                std::cout << K.getCrypto(i)<<"("<<i<<") / "<< item.second[i]<<",\t";
            }
        }
        std::cout << std::endl;
    }
}