#ifndef ERGASIA2_PARAMETERS_HPP
#define ERGASIA2_PARAMETERS_HPP


#include <string>

namespace HyperParams{

    extern size_t k_LSH;
    extern size_t L_LSH;

    extern int P;

    extern int K_tweets;
    extern int K_Cj;
    extern std::string cryptos_file;
    extern std::string lexicon_file;
    extern std::string tfidf_dataset;

    extern int sample;
};

void parse_config_params(std::string config_filename);


#endif //ERGASIA2_PARAMETERS_HPP
