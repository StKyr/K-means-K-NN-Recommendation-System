#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include "../../include/utils.hpp"
#include <tuple>
#include "../../include/parameters.hpp"

namespace HyperParams{

    size_t k_LSH = 2;
    size_t L_LSH = 3;

    int P=0;

    int K_tweets = 150;
    int K_Cj=0;
    std::string cryptos_file ="../input/coins_queries.csv";
    std::string lexicon_file ="../input/vader_lexicon.csv";
    std::string tfidf_dataset = "../input/twitter_dataset_small_v2.csv";

    int sample=0;
    bool validate=false;
};


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
    typedef void(*assign_fun)(std::string);

    std::vector<value_check_fun> check_functions;
    std::vector<assign_fun> assign_functions;
    std::vector<std::string> option_names;

    void insert(std::string option_name, value_check_fun check, assign_fun assign){
        option_names.push_back(option_name);
        check_functions.push_back(check);
        assign_functions.push_back(assign);
    }

    void parse_option(std::string& name, std::string& value){
        for (int i=0; i<option_names.size(); i++){
            if (name == option_names[i]){
                if (! check_functions[i](value)) throw std::runtime_error("Invalid value "+value+" for option "+name);
                assign_functions[i](value);
                return;
            }
        }
        throw std::runtime_error("Invalid option "+name);
    }
};

bool check_value_k(std::string v)                               {return str_convert<int>(v)>0;}
void assign_k_tweets(std::string v)                             {HyperParams::K_tweets = str_convert<int>(v);}

bool check_k_LSH(std::string v)                                 {return str_convert<int>(v) > 0;}
void assign_k_LSH(std::string v)                                {HyperParams::k_LSH = str_convert<size_t >(v);}

bool check_L_LSH(std::string v)                                 {return str_convert<int>(v)>0;}
void assign_L_LSH(std::string v)                                {HyperParams::L_LSH = str_convert<size_t >(v);}

bool check_k_cj(std::string v)                                  {return str_convert<int>(v)>0;}
void assign_k_cj(std::string v)                                 {HyperParams::K_Cj = str_convert<int>(v);}

bool check_lexicon(std::string v)                               {return !v.empty();}
void assign_lexicon(std::string v)                              {HyperParams::lexicon_file = v;}

bool check_tfidf(std::string v)                                 {return !v.empty();}
void assign_tfidf(std::string v)                                {HyperParams::tfidf_dataset = v;}

bool check_coins(std::string v)                                 {return !v.empty();}
void assign_coins(std::string v)                                {HyperParams::cryptos_file = v;}

bool check_sample(std::string v)                                {return str_convert<int>(v) >0;}
void assign_sample(std::string v)                               {HyperParams::sample = str_convert<int>(v);}

bool check_P(std::string v)                                     {return str_convert<int>(v) >0;}
void assign_P(std::string v)                                    {HyperParams::P = str_convert<int>(v);}

void parse_config_params(std::string config_filename){

    std::ifstream ifconfig;
    ifconfig.open(config_filename);

    std::string line;
    std::string line_without_comments;
    std::vector<std::string> parts;
    std::string option;
    std::string value;

    Options options;
    options.insert("number_of_tweet_clusters"        , check_value_k      , assign_k_tweets);
    options.insert("number_of_virtual_users_clusters", check_k_cj         , assign_k_cj);
    options.insert("number_of_hash_functions"        , check_k_LSH        , assign_k_LSH);
    options.insert("number_of_hash_tables"           , check_L_LSH        , assign_L_LSH);
    options.insert("lexicon_filename"                , check_lexicon      , assign_lexicon);
    options.insert("tfidf_dataset"                   , check_tfidf        , assign_tfidf);
    options.insert("coins_filename"                  , check_coins        , assign_coins);
    options.insert("sample"                          , check_sample       , assign_sample);
    options.insert("P"                               , check_P            , assign_P);

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
                options.parse_option(option, value);
            }
        } catch(std::exception& e) {
            throw std::runtime_error("Invalid format of config file");
        }
    }else{
        throw std::runtime_error("Cannot open config file");
    }
    ifconfig.close();
}