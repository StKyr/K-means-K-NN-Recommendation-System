#ifndef ERGASIA1_HASHTABLE_H
#define ERGASIA1_HASHTABLE_H


#include <string>
#include <vector>
#include <list>

typedef struct bucket_t{
    int         g_key;
    std::string VectorId;
} Bucket;


Bucket make_Bucket(int g_key, std::string vectorId) {
    return Bucket{
            g_key    : g_key,
            VectorId : std::move(vectorId)
    };
}


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


#endif //ERGASIA1_HASHTABLE_H
