#include <stdexcept>
#include "HashTable.h"







HashTable::HashTable(int size)
        :size(size){

    this->array.reserve((unsigned)size);
    for (int i=0; i<size; i++) {
        this->array.emplace_back(BucketList());
    }
}




void HashTable::insert(int position, Bucket bucket) {
    if (position >= this->size || position < 0) throw std::runtime_error("hash table overflow");

    this->array[position].emplace_back(bucket);

}


std::vector<std::string> HashTable::
getVectorIds(int position, std::string key){
    std::vector<std::string> vectorIds;

    for (auto item : array[position]){
        if (item.g_key == key){
            vectorIds.push_back(item.VectorId);
        }
    }
    return vectorIds;
}


