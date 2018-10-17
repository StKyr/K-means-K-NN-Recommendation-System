#ifndef ERGASIA1_EUCLIDEANSPACELSH_H
#define ERGASIA1_EUCLIDEANSPACELSH_H

#include <queue>
#include <list>
#include <string>
#include <array>
#include <vector>
#include <tuple>
#include <set>
#include <unordered_map>

#include "NDVector.h"
#include "HashTable.h"
#include "LSH.h"

#ifdef DEVELOPMENT
#include <gtest/gtest.h>
#endif




class hEucl : public hFunction {
    public:
        explicit hEucl (int w, int d);
        int operator () (NDVector &p) override;
    private:
        int w;
        int t;
        NDVector v;


#ifdef DEVELOPMENT
    private:
        FRIEND_TEST(testhFunction, check_t_randomness);
        FRIEND_TEST(testhFunction, check_v_randomness);
        FRIEND_TEST(testhFunction, check_equal_t);
        FRIEND_TEST(testhFunction, return_value_constraints);

#endif
};



class EuclideanSpaceLSH : public LSH{
public:
                          EuclideanSpaceLSH(int L, int tableSize, int k, int d, int w);
    void                  insertVector     (NDVector p, std::string vectorId);
    void                  insertDataset    (std::unordered_map<std::string, NDVector> X);
    std::set<std::string> retrieveNeighbors(NDVector p);
    virtual               ~EuclideanSpaceLSH(){for (auto &h_family : H) for (auto &h_i : h_family) delete (hEucl *)h_i;}


private:
    static const int M = 0;

    std::vector<std::vector<int>> R; //TODO: remove that -> change to single

    std::string g(NDVector p, int i);
    unsigned phi(NDVector p, int j);



#ifdef DEVELOPMENT

    std::set<std::string> ids1;

    FRIEND_TEST(testEuclideanLSH, g_values_digits);
    FRIEND_TEST(testEuclideanLSH, g_values_exact);
    FRIEND_TEST(testEuclideanLSH, g_values_nearby);

#endif




};









#endif //ERGASIA1_EUCLIDEANSPACELSH_H
