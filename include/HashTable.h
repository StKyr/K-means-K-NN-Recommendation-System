#ifndef ERGASIA1_HASHTABLE_H
#define ERGASIA1_HASHTABLE_H


#include <string>
#include <vector>
#include <list>


/**
 * A Placeholder to store vector IDs along with their g(p) key
 */
typedef struct bucket_t{
    std::string g_key;
    std::string VectorId;
} Bucket;



typedef std::list<Bucket> BucketList;


/**
 * A very basic hash table for the LSH family. It is implemnted by a vector of lists of Buckets (i.e. key-ID pairs).
 * Hashing is done by client code and the class provides imediate (and filtered) read access to its table entries.
 */
class HashTable {
public:

    /**
     * Construct a HashTable object with a specific table size
     * @param size The size of the table
     */
    explicit HashTable(int size);

    /**
     * Traverse the list of stored buckets ina certain table position and retrieve those with a specified g(p) key
     * @param position The position in the table of to search
     * @param key The g(p) key to be search for
     * @return All elements in the bucket list with {@code g_key} equal to {@code key}
     */
    std::vector<std::string>  getVectorIds(int position, std::string key);

    /**
     * Insert a new entry in a certain position
     * @param position The index of the table to be inserted to
     * @param bucket A bucket of data to be inserted in the list
     */
    void insert(int position, Bucket bucket);

    ~HashTable() = default;

private:
    int size;
    std::vector<BucketList> array;
};


#endif //ERGASIA1_HASHTABLE_H
