#ifndef HASH_TABLE
#define HASH_TABLE

#include <iostream>
#include <string>
#include <iterator>
#include <cmath>
#include <vector>

#include "../../headers/utils/utils.h"
#include "../../headers/search/lsh.h"
#include "../../headers/hash/hash_function.h"

template <class T>
class HashTable {
  private:
    std::vector<T*> *table;
    std::vector<HashFunction<T>*> g;
  public:
    HashTable(utils::InputInfo&, uint32_t, uint32_t, uint32_t);
    void PrintVector();
    ~HashTable();
};

template <class T>
HashTable<T>::HashTable(utils::InputInfo& input_info, uint32_t w, uint32_t m, uint32_t M) {
  for(size_t i=0; i<input_info.K; i++) {
    HashFunction<T> *hi = new HashFunction<T>(input_info.R,w,input_info.D);
    g.push_back(hi);
  }
  table = new std::vector<T*>[input_info.N/4];
}

template <class T>
HashTable<T>::~HashTable() {}

#endif
