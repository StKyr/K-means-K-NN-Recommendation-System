#include <iostream>
#include <vector>
#include <getopt.h>
#include "../include/Lexicon.hpp"
#include "../include/Datasets.hpp"
#include "../include/preprocessing.hpp"
#include "../include/ApproximateNeighborSearch/VectorCSVReader.h"
#include "../include/clustering/clustering.h"
#include "../include/recommendations.hpp"
#include "../include/UserVectors.hpp"
#include "../include/parameters.hpp"



namespace InputArguments{
    std::string inputFilename;
    std::string outputFilename;
    bool validate = false;
};



void printSuggestions(std::vector<std::vector<std::string>> &suggestions, Dataset &U, std::string algorithm, std::string outputFilename){
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
void parseCommandLineArguments(int argc, char* argv[]);


int main(int argc, char* argv[]) {
    parseCommandLineArguments(argc, argv);
    parse_config_params("./config");

    TokenizedDataset T(InputArguments::inputFilename,'\t');
    TfidfDataset     X(HyperParams::tfidf_dataset,',');
    SentimentLexicon A(HyperParams::lexicon_file,'\t');
    CryptoLexicon    K(HyperParams::cryptos_file,'\t');

    UserVectorDataset users(T, A, K);
    users.filterOutZeros();
    users.subtract_average();

    std::vector<std::vector<std::string>> suggestions;

    Evaluation evaluation;

//    std::cout<<"\n\n\n-------------------------- 1 ----------------------------\n\n"<<std::endl;
//    suggestions = recommend(5, users, HyperParams::P, K, nearest_neighbor, &evaluation);
//    std::cout<<"\n\n\n-------------------------- 2 ----------------------------\n\n"<<std::endl;
//    suggestions = recommend(5, users, Params::P, K, clustering, &evaluation);



//    auto clusters = vectorizedDatasetClustering(X, 200);
//    VirtualUserVectorDataset virtualUsers(clusters, T, A, K);
//    virtualUsers.filterOutZeros();
//    virtualUsers.subtract_average();

//    std::cout<<"\n\n\n-------------------------- 3 ----------------------------\n\n"<<std::endl;
//    suggestions = recommend(2, users, virtualUsers, Params::P, K, nearest_neighbor, &evaluation);
//    std::cout<<"\n\n\n-------------------------- 4  ----------------------------\n\n"<<std::endl;
//    suggestions = recommend(2, users, virtualUsers, Params::P, K, clustering, &evaluation);


    std::cout<<evaluation.getMAE()<<std::endl;

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



void parseCommandLineArguments(int argc, char* argv[]){

    int validate=0;
    int c;
    opterr = 0;
    int option_index = 0;
    static struct option long_options[] = {
            {"dataset",     required_argument, 0,           'd'},
            {"output",      required_argument, 0,           'o'},
            {"validate",    no_argument,       &validate,    1 },
            {0, 0, 0, 0}
    };
    while ((c = getopt_long_only(argc, argv, "o:d:", long_options, &option_index)) != -1)
        switch (c) {
            case 'd':
                InputArguments::inputFilename = std::string(optarg);
                break;
            case 'o':
                InputArguments::outputFilename = std::string(optarg);
                break;
            case '?':
                if (optopt == 'd' || optopt == 'o') {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                }
                else if (isprint (optopt)) {
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                }else {
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                }
                break;
            default:
                break;
        }

    if (InputArguments::inputFilename.empty()){
        fprintf(stderr, "Error: Required -d argument.\n");
        exit(EXIT_FAILURE);
    }

    if (InputArguments::outputFilename.empty()){
        fprintf(stderr, "Error: Required -o argument.\n");
        exit(EXIT_FAILURE);
    }

    if (validate) InputArguments::validate = true;
}


