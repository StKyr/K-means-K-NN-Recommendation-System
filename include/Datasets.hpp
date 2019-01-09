#ifndef ERGASIA3_DATASETS_HPP
#define ERGASIA3_DATASETS_HPP

#include <string>
#include <unordered_map>
#include <fstream>
#include <map>
#include <iostream>
#include "ApproximateNeighborSearch/NDVector.h"
#include "utils.hpp"
#include <algorithm>

typedef std::vector<std::string> TokensList;

class TokenizedDataset {
public:

    explicit TokenizedDataset(std::string fileName, char del=','){
        std::ifstream fin;
        std::string line, userId, tweetId, token, P_str, _;


        fin.open(fileName);
        if (fin.is_open()){
            fin >> _;
            fin >> P_str;
            this->P = str_convert<int>(trim(P_str));


            while (getline(fin, line)){

                std::stringstream ss(line);
                getline(ss, userId, del);
                getline(ss, tweetId, del);
                //this->userTweets.insert(std::make_pair(userId, tweetId));
                if (userTweets.find(userId) != userTweets.end()){
                    userTweets[userId].push_back(tweetId);
                }else{
                    std::vector<std::string> vector;
                    vector.push_back(tweetId);
                    userTweets[userId] = vector;
                }


                try {
                    TokensList tokensList;
                    while (getline(ss, token, del)) {
                        token.erase(std::remove(token.begin(), token.end(),'\r'), token.end());
                        token.erase(std::remove(token.begin(), token.end(),'\n'), token.end());
                        token.erase(std::remove(token.begin(), token.end(),'\t'), token.end());
                        token.erase(std::remove(token.begin(), token.end(),' '), token.end());
                        tokensList.push_back(token);
                    }
                    this->tweets[tweetId] = tokensList;
                }catch (std::exception &e){ // possible '\r' character before new line
                    if (token != "\r"){
                        throw e;
                    }
                }
            }
        }else{
            throw std::runtime_error(fileName+" file not found.");
        }

        fin.close();
    }

    std::vector<TokensList> getUserTweets(std::string userId) {
        std::vector<TokensList> tweets;

        /*auto result = this->userTweets.equal_range(userId);
        for (auto it=result.first; it != result.second; ++it){
            tweets.push_back(getTweet(it->second));
        }*/
        if (userTweets.find(userId) != userTweets.end()){
            for (auto &tweetId : userTweets[userId]){
                tweets.emplace_back(this->tweets[tweetId]);
            }
        }
        return tweets;
    }

    TokensList getTweet(std::string id){ return this->tweets[id];}

    std::vector<std::string> getUserIds(){
        std::vector<std::string> users;

/*        for (auto it=userTweets.begin(), end=userTweets.end(); it != end; it=userTweets.upper_bound(it->first)){
           users.push_back(it->first);
        }*/
        for (auto& pair : userTweets){
            users.push_back(pair.first);
        }
        return users;
    }

    int P;
private:
    //std::multimap<std::string, std::string> userTweets;
    std::unordered_map<std::string, std::vector<std::string>> userTweets;
    std::unordered_map<std::string, TokensList> tweets;
};





class TfidfDataset {
public:
    explicit TfidfDataset(std::string fileName, char del=','){
        std::ifstream fin;
        std::string line, vectorId, part;
        double x;
        int cnt=0;

        fin.open(fileName);
        if (fin.is_open()){
            while (getline(fin, line)){
                std::stringstream ss(line);
                getline(ss, vectorId, del);
                try {
                    std::vector<double> v;
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
        fin.close();
    }

    int dim(){ return dataset.begin()->second.dim(); } //todo: error here

    std::unordered_map<std::string, NDVector> dataset;
};

#endif //ERGASIA3_DATASETS_HPP
