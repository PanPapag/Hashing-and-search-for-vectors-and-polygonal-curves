#ifndef LSH_H_
#define LSH_H_

#include <iterator>
#include <random>
#include <unordered_map>

#include "../../headers/metric/metric.h"
#include "../../headers/utils/utils.h"
#include "../../headers/hash/hash_function.h"

//template <class T>
//class HashTable;

namespace search {

  template <typename T, typename U>
  class LSH {
    private:
      uint32_t table_size;
      uint32_t m;
      uint32_t M;
      double w;
      const uint8_t K;
      const uint8_t L;
      const uint16_t D;
      const uint32_t N;
      const double R;
      const std::vector<T> &feature_vector;
      const std::vector<U> &feature_vector_ids;
      std::vector<hash::AmplifiedHashFunction<T>> hash_functions;
      std::vector<std::unordered_map<int,std::vector<U>>> hash_tables;
      //std::vector<HashTable<T>*> hash_tables;
    public:
      /** \brief class LSH constructor
      */
      LSH(const uint8_t K, const uint8_t L, const uint16_t D, const uint32_t N,
          const double R, const std::vector<T> &points, const std::vector<T> &ids)
        : K(K), L(L), D(D), N(N), R(R), feature_vector(points), feature_vector_ids(ids) {
          w = ComputeWindow(points, N);
          m = (1ULL << 32) - 5;
          M = pow(2, 32 / K);
          table_size = N / 16;
          // Preprocess step
          // 1) Randomly select L amplified hash functions g1 , . . . , gL .
          for (int i = 0; i < L; ++i) {
            hash_functions.push_back(hash::AmplifiedHashFunction<T>(K,D,m,M,w));
          }
          // 2) Initialize L hash-tables, hash all points to all tables using g
          for (int i = 0; i < L; ++i) {
            std::unordered_map<int,std::vector<U>> ht;
            for (int j = 0; j < N; ++j) {
              ht[hash_functions[i].Hash(feature_vector,j) % table_size].push_back(feature_vector_ids[j]);
            }
            hash_tables.push_back(ht);
          }
      };
      /**
        \brief class LSH default construct
      */
      ~LSH() = default;
      //std::vector<HashTable<T>*>& CreateHashTables(utils::InputInfo&);
      /** \brief Computes window by selecting randomly n / 1000 points from the
          dataset and averaging its coordinates. Then window equals the average
          of these n / 1000 points' coordinates average
          @par const std::vector<T> &points - dataset points
          @par int N - Number of points
      */
      double ComputeWindow(const std::vector<T> &points, int N) {
        double w{};
        // vector with sample_n ints
        int sample_n = N / 1000;
        std::vector<int> v(N);
        // Fill with 0, 1, ..., N - 1
        std::iota(std::begin(v), std::end(v), 0);
        // shuffle vector end execute the algo
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        shuffle(v.begin(), v.end(), std::default_random_engine(seed));
        // compute average coordinates for the sample_n points selected randomly
        std::vector<double> avg_coord(sample_n);
        for (int i = 0; i < sample_n; ++i) {
          for (int j = 0; j < 128; ++j) {
            avg_coord[i] += points[v[i] * 128 + i];
          }
          avg_coord[i] /= 128;
        }
        // average them to compute window
        for (int i = 0; i < sample_n; ++i) {
          w += avg_coord[i];
        }
        w /= double(sample_n);

        return w;
      }
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
