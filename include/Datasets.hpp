#ifndef ERGASIA3_DATASETS_HPP
#define ERGASIA3_DATASETS_HPP

#include <string>
#include <unordered_map>
#include <fstream>
#include <map>
#include <iostream>
#include "ApproximateNeighborSearch/NDVector.h"


typedef std::vector<std::string> TokensList;

class TokenizedDataset {
public:

    explicit TokenizedDataset(std::string fileName, char del=','){
        std::ifstream fin;
        std::string line, userId, tweetId, token;

        TokensList tokensList;
int __cnt=0;
        fin.open(fileName);
        if (fin.is_open()){
            while (getline(fin, line)){

if (__cnt == 4001) {fin.close(); return;}

                std::stringstream ss(line);
                getline(ss, userId, del);
                getline(ss, tweetId, del);
                this->userTweets.insert(std::make_pair(userId, tweetId));


if (++__cnt % 1000 == 0 || (__cnt > 4000 && __cnt % 100 == 0)) std::cerr << "Parsing tokenized dataset: " <<__cnt << std::endl;
                try {
                    while (getline(ss, token, del)) {
                        tokensList.push_back(token);
                    }
                    this->tweets[tweetId] = tokensList;
                }catch (std::exception &e){ // possible '\r' character before new line
                    if (token != "\r"){
//std::cerr << "Exception "<<std::endl;
                        throw e;
                    }
                }
            }
        }else{
            throw std::runtime_error(fileName+" file not found.");
        }

        fin.close();
//std::cerr << "Parsed dataset - "<<__cnt<<" tweets"<<std::endl;
    }

    std::vector<TokensList> getUserTweets(std::string userId) {
        std::vector<TokensList> tweets;

        auto result = this->userTweets.equal_range(userId);
        for (auto it=result.first; it != result.second; ++it){
            tweets.push_back(getTweet(it->second));
        }
        return tweets;
    }

    TokensList getTweet(std::string id){ return this->tweets[id];}

    std::vector<std::string> getUserIds(){
        std::vector<std::string> users;

        for (auto it=userTweets.begin(), end=userTweets.end(); it != end; it=userTweets.upper_bound(it->first)){
           users.push_back(it->first);
        }
        return users;
    }
private:
    std::multimap<std::string, std::string> userTweets;
    std::unordered_map<std::string, TokensList> tweets;
};





class TfidfDataset {
public:
    explicit TfidfDataset(std::string fileName, char del=','){
        std::ifstream fin;
        std::string line, vectorId, part;
        double x;
        std::vector<double> v;
        int cnt=0;

        fin.open(fileName);
        if (fin.is_open()){
            while (getline(fin, line)){
                std::stringstream ss(line);
                getline(ss, vectorId, del);
                try {
                    while (getline(ss, part, del)) {
                        x = stod(part);
                        v.push_back(x);
                    }
                    dataset[vectorId] = NDVector(v,cnt++);
                }catch (std::exception &e){ // possible '\r' character before new line
                    if (part != "\r"){
                        throw e;
                    }
                }
            }
        }else{
            throw std::runtime_error(fileName+" file not found.");
        }
    }

    int dim(){ return dataset.begin()->second.dim(); } //todo: error here

    std::unordered_map<std::string, NDVector> dataset;
};



#endif //ERGASIA3_DATASETS_HPP
