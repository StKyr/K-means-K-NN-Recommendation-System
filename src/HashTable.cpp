#include "HashTable.h"







HashTable::HashTable(int size)
        :size(size){

    this->array.reserve((unsigned)size);
    for (int i=0; i<size; i++) {
        this->array.emplace_back(BucketList());
    }
}




void HashTable::insert(int position, Bucket bucket) {
    this->array[position].emplace_back(bucket);

}


std::vector<std::string> HashTable::
getVectorIds(int position, int originalKey){
    std::vector<std::string> vectorIds;

    for (auto item : array[position]){
        if (item.g_key == originalKey){
            vectorIds.push_back(item.VectorId);
        }
    }
    return vectorIds;
}


