
#include <iterator>
#include <cassert>
#include "../include/recommendations.hpp"
#include "../include/ApproximateNeighborSearch/VectorCSVReader.h"
#include "../include/ApproximateNeighborSearch/CosineSimilarityLSH.h"
#include "../include/Lexicon.hpp"
#include "../include/ApproximateNeighborSearch/NearestNeighborSearch.h"
#include "../include/ProximitySearch.hpp"

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
    if ( !v.empty() ) {
        out << '[';
        std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
        out << "\b\b]";
    }
    return out;
}



std::vector<std::pair<double, int>> rate(std::vector<std::pair<std::string, double>> &corpus, NDVector &u, Dataset &U){

    std::vector<std::pair<double,int>> ratings;

    /*double z = 0;
    for (auto &pair: corpus){
        double curr = fabs(1-pair.second);
        if (curr == NAN || curr ==-NAN){
            ;
        }
        z += curr;
    }
    z = 1/z;*/
    double z = 1;


    for (int i=0; i<u.dim(); i++){
        //if (u[i] != std::numeric_limits<double>::infinity()) continue;
        if (u[i] != 0) continue;

        double R=0;
        for (auto &pair: corpus) {
            double sim = 1-pair.second;
            NDVector v = U[pair.first];
            double r = v[i];

            if (r != 0 && sim != 0){
               // std::cout <<".";
            }

            double R_ = r;
            R += sim*R_;


            //R += (1 - pair.second) * U[pair.first][i];
        }
        R /= z;
        ratings.emplace_back(std::make_pair(R,i));
    }

    return ratings;
}

bool compareDesc(std::pair<double, int> first, std::pair<double, int> second){
    if (first.first == NAN || first.first == -NAN ) return false;
    if (second.first == NAN || second.second == -NAN) return true;
    return first.first > second.first;
}




std::vector<std::vector<std::string>> userBasedSuggestions(int num, Dataset &U, Dataset &U_normed, int P, CryptoLexicon &K){
    std::vector<std::vector<std::string>> results;

    /*
     * 1. For each user
     * 2.  Find P closest users
     * 3.  Perform a weighted sum on their sentiments per crypto
     * 4.  Find unmentioned cryptos from user's vector
     * 5.  Sort the cryptos
     * 6.  For each crypto:
     * 7.   If it is not mentioned, then SUGGEST
     * 8.  Repeat until num suggestions
     */

    CosineSimilarityLSH                         lsh1(2,8,U.begin()->second.dim()); //todo: find parameters
    CosineSimilarityLSH                         lsh2(2,8,U.begin()->second.dim()); //todo: find parameters

    std::set<std::string>                       neighbors;
    std::vector<std::pair<std::string, double>> nearest_neighbors;
    std::vector<std::pair<double,int>>          ratings;
    std::vector<std::string>                    userResults;
    Dataset neighborVectors;




    Dataset U_normed_non_zero;
    for (auto &it: U_normed) if (it.second != NDVector::zero_vector(it.second.dim())) U_normed_non_zero[it.first] = it.second;



    NearesrNeighborProximitySearch searchNormalized(P, U_normed_non_zero);
    NearesrNeighborProximitySearch searchOriginal(P, U);

/*
    P++;
    lsh1.insertDataset(U);
    lsh2.insertDataset(U_normed_non_zero);*/

    for (auto &it:  U_normed){
        //CosineSimilarityLSH *lsh;
        Dataset *dataset;
        NDVector *vector;


        if (U_normed_non_zero.find(it.first) != U.end()) { // a non 0 vector
            //lsh = &lsh2;
            dataset = &U_normed_non_zero;
            vector = &(U_normed_non_zero[it.first]);
            nearest_neighbors = searchNormalized(U_normed_non_zero[it.first]);
        }else{                                            // a 0 vector
            //lsh = &lsh1;
            dataset = &U;
            vector = &(U[it.first]);
            nearest_neighbors = searchOriginal(U[it.first]);
        }
/*
        neighbors = lsh->retrieveNeighbors(*vector);
        for (auto& id: neighbors){
            assert(dataset->find(id) != dataset->end());
            NDVector &v = (*dataset)[id];
            neighborVectors[id] = v;
        }*/


        //nearest_neighbors = k_nearestNeighbors(P, *vector, neighborVectors, metrics::cosine_distance);
        ratings = rate(nearest_neighbors, U[it.first], *dataset);
        std::sort(ratings.begin(), ratings.end(), compareDesc);


std::cout << "User " << it.first << ": {" << neighbors.size() << " similar users, mentioned cryptos:\t"; for(int i_=0;i_<it.second.dim();i_++)if((*vector)[i_]!= 0) std::cout<<K.getCrypto(i_)<<",\t"; std::cout<< "} ---->\t";
        userResults.clear();
        int i=0;
        for (auto &pair: ratings){
            if (i++ >= num) break;
            int cryptoIndex = pair.second;
            userResults.push_back(K.getCrypto(cryptoIndex));

std::cout<<K.getCrypto(cryptoIndex)<<" ("<<pair.first<<"),\t";
        }
        results.emplace_back(userResults);
std::cout<<std::endl;
    }
    return results;
}









