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

#ifdef DEVELOPMENT
#include <gtest/gtest.h>
#endif

class hFunction;
class HashTable;


class EuclideanSpaceLSH {
public:
                          EuclideanSpaceLSH(int L, int tableSize, int M, int k, int d, int w);
    void                  insertVector     (NDVector p, std::string vectorId);
    void                  insertDataset    (std::unordered_map<std::string, NDVector> X);
    std::set<std::string> retrieveNeighbors(NDVector p);


private:
    int L;
    int tableSize;
    int M;

    std::vector<std::vector<hFunction>> H;
    std::vector<std::vector<int>>       R;
    std::vector<HashTable>              hashTables;


    int g  (NDVector p, int j);
    int phi(NDVector p, int j);


#ifdef DEVELOPMENT
    FRIEND_TEST(testEuclideanLSH, g_values_digits);
    FRIEND_TEST(testEuclideanLSH, g_values_exact);
    FRIEND_TEST(testEuclideanLSH, g_values_nearby);

#endif
};


class hFunction {
public:
    explicit hFunction (int w, int d);
    int operator () (NDVector p);
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



typedef struct bucket_t{
    int         g_key;
    std::string VectorId;
} Bucket;

/*
Bucket make_a_Bucket(int g_key, std::string vectorId) {
    return Bucket{
            g_key    : g_key,
            VectorId : std::move(vectorId)
    };
}
*/

typedef std::list<Bucket> BucketList;

class HashTable {
public:
    explicit   HashTable      (int size);
    std::vector<std::string>   getVectorIds(int position, int originalKey);
    void       insert         (int position, Bucket bucket);
    ~HashTable      ()     = default;          //                             {for (BucketList *pList : this->array) delete pList;}

private:
    int size;
    std::vector<BucketList > array;
};



#endif //ERGASIA1_EUCLIDEANSPACELSH_H
