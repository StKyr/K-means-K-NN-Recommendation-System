#ifndef ERGASIA2_PARAMETERS_HPP
#define ERGASIA2_PARAMETERS_HPP


#include <string>

struct HyperParams{

    size_t K;

    size_t k_LSH;
    size_t L_LSH;
    double tau_LSH;
    size_t w_LSH;
    double tableSize_LSH;

    size_t k_CUBE;
    size_t M_CUBE;
    size_t probes_CUBE;


};

struct HyperParams parse_config_params(std::string config_filename);


#endif //ERGASIA2_PARAMETERS_HPP
