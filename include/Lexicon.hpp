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
    explicit CryptoLexicon(std::string fileName, char del=','){
        std::ifstream fin;
        std::string line, abbr, crypto;

        int i=0;
        fin.open(fileName);
        if (fin.is_open()){

            while(getline(fin, line)){
                std::stringstream ss(line);
                getline(ss, crypto, del);
                this->cryptos.push_back(crypto);
                this->abbreviations[crypto] = i;
                while(getline(ss, abbr, del)){
                    this->abbreviations[abbr] = i;
                }
                i++;
            }
        }else{
            throw std::runtime_error(fileName+" not found.");
        }
        fin.close();
    }

    std::vector<bool> createCryptoVector(std::vector<std::string>& tweet){
        std::vector<bool> cryptoVector(this->cryptos.size(), false);
        for (auto& token: tweet){
            if (this->abbreviations.find(token) != this->abbreviations.end()){
                cryptoVector.at(this->abbreviations[token]) = true;
            }
        }
        return cryptoVector;
    }

    std::string getCrypto(int index){
        return this->cryptos[index];
    }
private:
    std::vector<std::string> cryptos;
    std::unordered_map<std::string, int> abbreviations;
};


#endif //ERGASIA3_LEXICON_HPP
