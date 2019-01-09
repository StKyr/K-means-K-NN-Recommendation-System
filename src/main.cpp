#include <iostream>
#include <vector>
#include <getopt.h>
#include <chrono>
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



void printSuggestions(std::vector<std::vector<std::string>> &suggestions, Dataset &U, std::string algorithm, std::string outputFilename, long time, bool delete_previous=false){

    std::ofstream fout;

    if (delete_previous) fout.open(outputFilename, std::ios::out | std::ios::trunc);
    else                 fout.open(outputFilename, std::ios::out | std::ios::app);

    if (fout.is_open()) {
        fout << algorithm << std::endl;
        int i = 0;
        for (auto &item: U) {
            fout << item.first << "\t";
            for (auto &crypto: suggestions[i]) {
                fout << crypto << "\t";
            }
            fout << std::endl;
        }
        fout << "Execution Tie: " << time << std::endl;
        fout << std::endl;
        fout.close();
    }else{
        throw std::runtime_error("Error opening output file");
    }
}

void printUsers(Dataset &U, CryptoLexicon &K);
void parseCommandLineArguments(int argc, char* argv[]);


int main(int argc, char* argv[]) {
    parseCommandLineArguments(argc, argv);
    parse_config_params("./config");

    TokenizedDataset T(InputArguments::inputFilename,'\t');
    if (! HyperParams::P) HyperParams::P = T.P;
    TfidfDataset     X(HyperParams::tfidf_dataset,',');
    SentimentLexicon A(HyperParams::lexicon_file,'\t');
    CryptoLexicon    K(HyperParams::cryptos_file,'\t');

    UserVectorDataset users(T, A, K);
    users.filterOutZeros();
    users.subtract_average();

    std::vector<std::vector<std::string>> suggestions;

    Evaluation *evaluation;

    std::cout<<"Read and parsed input files, created users and preprocessed them."<<std::endl;

    // ------------------------------------------------- A --------------------------------------------//

    // 1.A
    evaluation = (HyperParams::validate) ? new Evaluation() : nullptr;
    auto begin = std::chrono::steady_clock::now();
    suggestions = recommend(5, users, HyperParams::P, K, nearest_neighbor, evaluation);
    printSuggestions(suggestions, users.U, "Cosine LSH (amongst users)", InputArguments::outputFilename,
                     std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - begin).count(), true);
    if (HyperParams::validate) std::cout<<"Cosine LSH Recommendation MAE (amongst users): "<<evaluation->getMAE()<<std::endl;
    delete evaluation;

    // 2.A
    evaluation = (HyperParams::validate) ? new Evaluation() : nullptr;
    begin = std::chrono::steady_clock::now();
    suggestions = recommend(5, users, HyperParams::P, K, clustering, evaluation);
    printSuggestions(suggestions, users.U, "Clustering (amongst users)", InputArguments::outputFilename,
                     std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - begin).count());
    if (HyperParams::validate) std::cout<<"Clustering Recommendation MAE (amongst users): "<<evaluation->getMAE()<<std::endl;
    delete evaluation;


     // ---------------------------------------------------- B --------------------------------------//
    auto clusters = vectorizedDatasetClustering(X, HyperParams::K_tweets);
    VirtualUserVectorDataset virtualUsers(clusters, T, A, K);
    virtualUsers.filterOutZeros();
    virtualUsers.subtract_average();

    std::cout<<"Created virtual users and preprocessed them." << std::endl;

    // 1.B
    evaluation = (HyperParams::validate) ? new Evaluation() : nullptr;
    begin = std::chrono::steady_clock::now();
    suggestions = recommend(2, users, virtualUsers, HyperParams::P, K, nearest_neighbor, evaluation);
    printSuggestions(suggestions, users.U, "Cosine LSH (amongst virtual users)", InputArguments::outputFilename,
                     std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - begin).count());
    if (HyperParams::validate) std::cout<<"Cosine LSH Recommendation MAE (amongst virtual users): "<<evaluation->getMAE()<<std::endl;
    delete evaluation;


    // 2.B
    evaluation = (HyperParams::validate) ? new Evaluation() : nullptr;
    begin = std::chrono::steady_clock::now();
    suggestions = recommend(2, users, virtualUsers, HyperParams::P, K, clustering, evaluation);
    printSuggestions(suggestions, users.U, "Clustering (amongst virtual users)", InputArguments::outputFilename,
                     std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - begin).count());
    if (HyperParams::validate) std::cout<<"Clustering Recommendation MAE (amongst virtual users): "<<evaluation->getMAE()<<std::endl;
    delete evaluation;

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
    if (validate) HyperParams::validate = true;
}


