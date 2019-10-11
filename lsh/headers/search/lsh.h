#ifndef LSH_H_
#define LSH_H_

#include <unordered_map>

#include "../../headers/metric/metric.h"
#include "../../headers/utils/utils.h"
#include "../../headers/hash/hash_table.h"

//template <class T>
//class HashTable;

namespace search {

  template <typename T, typename U>
  class LSH {
    private:
      const uint8_t K;
      const uint8_t L;
      const uint16_t D;
      const uint32_t N;
      const uint32_t m;
      const uint32_t M;
      const double w;
      const double R;
      const std::vector<T> &feature_vector;
      const std::vector<U> &feature_vector_ids;
      //std::vector<AmplifiedHashFunction<T> *> hash_functions;
      std::vector<std::unordered_map<int,std::vector<U>>> hash_tables;
      //std::vector<HashTable<T>*> hash_tables;
    public:
      /** \brief class LSH constructor
      */
      LSH(const uint8_t K, const uint8_t L, const uint16_t D, const uint32_t N,
          const double R, const std::vector<T> &points, const std::vector<T> &ids)
        : K(K), L(L), D(D), N(N), R(R), feature_vector(points), feature_vector_ids(ids) {
          w = 2 * R + 1000;
          m = (1ULL << 32) - 5;
          M = pow(2, 32 / K);
          //hash_tables = CreateHashTables(input_info);
      };
      /**
        \brief class LSH default construct
      */
      ~LSH() = default;
      //std::vector<HashTable<T>*>& CreateHashTables(utils::InputInfo&);
  };
}



/*
template<class T>
std::vector<HashTable<T>*>& LSH_<T>::CreateHashTables(utils::InputInfo& info) {
  for (size_t i; i<info.L; i++) {
    HashTable<T> *ht = new HashTable<T>(info, w, m, M);
    hash_tables.push_back(ht);
  }
  return hash_tables;
}
*/

#endif
