#include <iostream>
#include <vector>
#include "../include/Lexicon.hpp"
#include "../include/Datasets.hpp"
#include "../include/analysis.hpp"
#include "../include/ApproximateNeighborSearch/VectorCSVReader.h"
#include "../include/clustering/clustering.h"


namespace  Params{
    int P=30;
    int K=200;
    std::string cryptos_file="../input/coin_queries.csv";
    std::string lexicon_file="../input/vader_lexicon.csv";
    std::string tweets_file="../input/tweets_dataset_small.csv";
    std::string input_file ="../input/tweeter_dataset_small_v2.csv";
}


int main() {
    TokenizedDataset T(Params::tweets_file,'\t');
    TfidfDataset     X(Params::input_file,',');
    SentimentLexicon A(Params::lexicon_file,'\t');
    CryptoLexicon    K(Params::cryptos_file,'\t');
    Dataset          *U = createUsersVectors(T, A, K);
    auto             clusters = vectorizedDatasetClustering(X, Params::K);
    Dataset          *C = createCentroidsVectors(clusters, T, A, K);

    /*

    std::vector<std::vector<std::string>> suggestions;

    suggestions = userBasedSuggestions(5, U, "lsh", Params::P);
    printSuggestions(suggestions, "P-range Neighbor Search of U");

    suggestions = userBasedSuggestions(2, U, "kmeans", Params::K);
    printSuggestions(suggestions, "K-means clustering search of U");
*/

    return 0;
}