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

template<typename T1, typename T2, typename T3>
using triple = std::tuple<T1, T2, T3>;



class EuclideanSpaceLSH {
public:
                          EuclideanSpaceLSH(int L, int tableSize, int M, int k, int d, int w);
    void                  insertVector     (NDVector p, std::string vectorId);
    void                  insertDataset    (std::unordered_map<std::string, NDVector> X);
    std::set<std::string> retireveNeighbors(NDVector p);


private:
    int L;
    int tableSize;
    static int M;

    class hFunction;
    class HashTable;

    std::vector<std::vector<hFunction>> H;
    std::vector<std::vector<int>>       R;
    std::vector<HashTable>              hashTables;


    int g  (NDVector p, int j);
    int phi(NDVector p, int j);




    class hFunction {
    public:
        explicit hFunction (int w, int d);
        int operator () (NDVector p);
    private:
        int w;
        int t;
        NDVector v;
    };

    typedef struct bucket_t{
        int         g_key;
        std::string VectorId;
    } Bucket;
    Bucket make_Bucket(int g_key, std::string vectorId) {return Bucket{.VectorId=vectorId, .g_key=g_key};}

    typedef std::list<Bucket>             BucketList;

    class HashTable {
    public:
        explicit   HashTable      (int size)                                   : size(size) {array.reserve((unsigned long)size);}
        std::vector<std::string>   getVectorIds(int position, int originalKey);
        void       insert         (int position, Bucket bucket)                { array[position].emplace_back(bucket);}

    private:
        int size;
        std::vector<BucketList> array;
    };

};


#endif //ERGASIA1_EUCLIDEANSPACELSH_H
