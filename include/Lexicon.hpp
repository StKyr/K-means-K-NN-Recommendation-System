#ifndef ERGASIA3_LEXICON_HPP
#define ERGASIA3_LEXICON_HPP


#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <vector>
#include "utils.hpp"

class SentimentLexicon {
public:
    explicit SentimentLexicon(std::string fileName, char del='\t'){
        std::ifstream fin;
        std::string line, token, sentiment_str;
        double sentiment;

        fin.open(fileName);
        if (fin.is_open()){
            while(getline(fin, line)){
                token = trim(line.substr(0, line.find(del)));
                sentiment = stod(line.substr(line.find(del)+1, line.size())); //todo: check that

                sentiments[token] = sentiment;
            }
        }else{
            throw std::runtime_error(fileName+" file not found.");
        }
        fin.close();
    }

    double getSentiment(std::string token){
        return sentiments[token];
    }

    bool exists(std::string token){
        return sentiments.find(token) != sentiments.end();
    }

private:
    std::unordered_map<std::string, double> sentiments;
};


class CryptoLexicon {
public:
    explicit CryptoLexicon(std::string fileName, char del=',');
    std::vector<double> createCryptoVector(std::vector<std::string>& tweet);
    std::vector<bool> createBoolVector(std::vector<std::string>& tweet);
    bool exists(const std::string word);
    bool exists(const char* word);
    std::string getCrypto(int index);
    std::string say_hi();
    void print();
    int dim(){return cryptos.size();}
private:
    std::vector<std::string> cryptos;
    std::unordered_map<std::string, int> abbreviations;
};


#endif //ERGASIA3_LEXICON_HPP
