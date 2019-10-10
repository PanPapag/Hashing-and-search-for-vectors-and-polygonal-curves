#ifndef LSH
#define LSH

#include "../../headers/utils.h"
#include "../../headers/hash_table.h"

template <class T>
class HashTable;

template <class T>
class LSH_ {
  private:
    uint32_t w;                             // size of every cell (window)
    uint32_t m;                             // m > max a_i
    uint32_t M;                             // used for modulation
    std::vector<HashTable<T>*> hash_tables;
  public:
    LSH_(utils::InputInfo&);
    std::vector<HashTable<T>*>& CreateHashTables(utils::InputInfo&);
    ~LSH_();
};

template<class T>
LSH_<T>::LSH_(utils::InputInfo& input_info) {
  this->w = 1000*input_info.R;
  this->m = (1ULL << 32) - 5;
  this->M = input_info.N/4;
  hash_tables = CreateHashTables(input_info);
}

template<class T>
std::vector<HashTable<T>*>& LSH_<T>::CreateHashTables(utils::InputInfo& info) {
  for (size_t i; i<info.L; i++) {
    HashTable<T> *ht = new HashTable<T>(info, w, m, M);
    hash_tables.push_back(ht);
  }
  return hash_tables;
}

template<class T>
LSH_<T>::~LSH_() {}

#endif