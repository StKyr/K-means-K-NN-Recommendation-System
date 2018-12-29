#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include "../../include/utils.hpp"
#include <tuple>
#include "../../include/parameters.hpp"

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}


struct Options{
    typedef bool(*value_check_fun)(std::string) ;
    typedef void(*assign_fun)(std::string, struct HyperParams&);

    std::vector<value_check_fun> check_functions;
    std::vector<assign_fun> assign_functions;
    std::vector<std::string> option_names;

    void insert(std::string option_name, value_check_fun check, assign_fun assign){
        option_names.push_back(option_name);
        check_functions.push_back(check);
        assign_functions.push_back(assign);
    }

    void parse_option(std::string& name, std::string& value, struct HyperParams& hyperParams){
        for (int i=0; i<option_names.size(); i++){
            if (name == option_names[i]){
                if (! check_functions[i](value)) throw std::runtime_error("Invalid value "+value+" for option "+name);
                assign_functions[i](value, hyperParams);
                return;
            }
        }
        throw std::runtime_error("Invalid option "+name);
    }
};

bool check_value_k(std::string v)                               {return str_convert<int>(v)>0;}
void assign_k(std::string v, struct HyperParams& params)        {params.K = str_convert<size_t>(v);}

bool check_k_LSH(std::string v)                                 {return str_convert<int>(v) > 0;}
void assign_k_LSH(std::string v, struct HyperParams& params)    {params.k_LSH = str_convert<size_t >(v);}

bool check_L_LSH(std::string v)                                 {return str_convert<int>(v)>0;}
void assign_L_LSH(std::string v, struct HyperParams& params)    {params.L_LSH = str_convert<size_t >(v);}

bool check_t_LSH(std::string v)                                 {return str_convert<double>(v)>0;}
void assign_t_LSH(std::string v, struct HyperParams& params)    {params.tau_LSH = str_convert<double>(v);}

bool check_w_LSH(std::string v)                                 {return str_convert<int>(v)>0;}
void assign_w_LSH(std::string v, struct HyperParams& params)    {params.w_LSH = str_convert<size_t >(v);}

bool check_k_CUBE(std::string v)                                 {return str_convert<int>(v)>0;}
void assign_k_CUBE(std::string v, struct HyperParams& params)    {params.k_CUBE = str_convert<size_t >(v);}

bool check_M_CUBE(std::string v)                                 {return str_convert<int>(v)>0;}
void assign_M_CUBE(std::string v, struct HyperParams& params)    {params.M_CUBE = str_convert<size_t >(v);}

bool check_probes_CUBE(std::string v)                              {return str_convert<int>(v)>0;}
void assign_probes_CUBE(std::string v, struct HyperParams& params) {params.probes_CUBE = str_convert<size_t >(v);}

bool check_tablesize_LSH(std::string v)                              {return str_convert<double>(v)>0 && str_convert<double>(v)<1;}
void assign_tablesize_LSH(std::string v, struct HyperParams& params) {params.tableSize_LSH = str_convert<double>(v);}

struct HyperParams parse_config_params(std::string config_filename){

    struct HyperParams hyperParams;
    hyperParams.k_LSH = 4;
    hyperParams.L_LSH = 5;

    std::ifstream ifconfig;
    ifconfig.open(config_filename);

    std::string line;
    std::string line_without_comments;
    std::vector<std::string> parts;
    std::string option;
    std::string value;

    Options options;
    options.insert("number_of_clusters"      , check_value_k      , assign_k);
    options.insert("number_of_hash_functions", check_k_LSH        , assign_k_LSH);
    options.insert("number_of_hash_tables"   , check_L_LSH        , assign_L_LSH);
    options.insert("t"                       , check_t_LSH        , assign_t_LSH);
    options.insert("w"                       , check_w_LSH        , assign_w_LSH);
    options.insert("table_size"              , check_tablesize_LSH, assign_tablesize_LSH);
    options.insert("hypercube_dimension"     , check_k_CUBE       , assign_k_CUBE);
    options.insert("number_of_probes"        , check_probes_CUBE  , assign_probes_CUBE);
    options.insert("max_hypercube_points"    , check_M_CUBE       , assign_M_CUBE);


    if (ifconfig.is_open()){
        try {
            while (getline(ifconfig, line)) {   // get a whole line
                if (trim(line)[0] == '#') continue;
                if (ltrim(line).empty()) continue;
                line_without_comments = split(line, "#").at(0);
                if (trim(line_without_comments).empty()) continue;
                try {
                    option = trim(split(line_without_comments, ":").at(0));
                    value = trim(split(line_without_comments, ":").at(1));

                }catch(std::exception& e){
                    throw std::runtime_error("option without value");
                }
                options.parse_option(option, value, hyperParams);
            }
        } catch(std::exception& e) {
            throw std::runtime_error("Invalid format of config file");
        }
    }else{
        throw std::runtime_error("Cannot open config file");
    }
    ifconfig.close();
    return hyperParams;
}