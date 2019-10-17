#ifndef LSH_H_
#define LSH_H_

#include <iterator>
#include <random>
#include <tuple>
#include <unordered_map>

#include "../../headers/hash/hash_function.h"
#include "../../headers/metric/metric.h"
#include "../../headers/utils/utils.h"

using namespace std::chrono;

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
      const std::vector<T>& feature_vector;
      const std::vector<U>& feature_vector_ids;
      std::vector<hash::AmplifiedHashFunction<T>> hash_functions;
      std::vector<std::unordered_map<int,std::vector<int>>> hash_tables;
    public:
      /** \brief class LSH constructor
      */
      LSH(const uint8_t K, const uint8_t L, const uint16_t D, const uint32_t N,
          const double radius, const std::vector<T>& points, const std::vector<T>& ids)
        : K(K), L(L), D(D), N(N), R(radius), feature_vector(points), feature_vector_ids(ids) {
          w = 4 * R;
          m = (1ULL << 32) - 5;
          M = 1ULL << (32 / K);
          table_size = N / 16;
          // Preprocess step
          // 1) Randomly select L amplified hash functions g1 , . . . , gL .
          for (int i = 0; i < L; ++i) {
            hash_functions.push_back(hash::AmplifiedHashFunction<T>(K,D,m,M,w));
          }
          // 2) Initialize L hash-tables, hash all points to all tables using g
          for (int i = 0; i < L; ++i) {
            std::unordered_map<int,std::vector<int>> ht;
            for (int j = 0; j < N; ++j) {
              ht[hash_functions[i].Hash(feature_vector,j) % table_size].push_back(j);
            }
            hash_tables.push_back(ht);
          }
      };
      /**
        \brief class LSH default destructor
      */
      ~LSH() = default;

      /** \brief Executes approximate Nearest tNeighbor
        @par const std::vector<T>& query_points - Pass by reference query points
        @par const int offset - Offset to get correspodent point
      */
      std::tuple<T,U,double> NearestNeighbor(const std::vector<T>& query_points,
        const int offset) {

        auto start = high_resolution_clock::now();
        /* Initialize min_dist to max value of type T */
        T min_dist = std::numeric_limits<T>::max();
        /* Initialize correspodent min_id using the C++11 way */
        U min_id{};
        for (int i = 0; i < L; ++i) {
          // get i_th hashtable
          std::unordered_map<int,std::vector<int>>& ht_i = hash_tables[i];
          // get all points in the same bucket
          std::vector<int>& bucket = ht_i[hash_functions[i].Hash(query_points,offset) % table_size];
          // iterate over all points in the buck
          for (auto const& fv_offset: bucket) {
            T dist = metric::ManhattanDistance<T>(
              std::next(feature_vector.begin(), fv_offset * D),
              std::next(query_points.begin(), offset * D),
              std::next(query_points.begin(), offset * D + D));
            if (dist < min_dist) {
              min_dist = dist;
              min_id = feature_vector_ids[fv_offset];
            }
          }
        }
        auto stop = high_resolution_clock::now();
        duration <double> total_time = duration_cast<duration<double>>(stop - start);
        /* return result as a tuple of min_dist, min_id and total_time */
        return std::make_tuple(min_dist,min_id,total_time.count());
      };
      
      /** \brief Executes approximate Radius Nearest tNeighbor
        @par const std::vector<T> &query_points - Pass by reference query points
        @par const int offset - Offset to get correspodent point
      */
      std::vector<std::pair<T,U>> RadiusNearestNeighbor(const std::vector<T>& query_points,
        const int offset) {

        /* Define result vector */
        std::vector<std::pair<T,U>> result;
        /* Initialize min_dist to max value of type T */
        T min_dist = std::numeric_limits<T>::max();
        /* Initialize correspodent min_id using the C++11 way */
        U min_id{};
        for (int i = 0; i < L; ++i) {
          // get i_th hashtable
          std::unordered_map<int,std::vector<int>>& ht_i = hash_tables[i];
          // get all points in the same bucket
          std::vector<int> &bucket = ht_i[hash_functions[i].Hash(query_points,offset) % table_size];
          // iterate over all points in the buck
          for (auto const& fv_offset: bucket) {
            T dist = metric::ManhattanDistance<T>(
              std::next(feature_vector.begin(), fv_offset * D),
              std::next(query_points.begin(), offset * D),
              std::next(query_points.begin(), offset * D + D));
            if (dist <= R) {
              result.push_back(std::make_pair(dist,feature_vector_ids[fv_offset]));
            }
          }
        }
        return result;
      };
  };
}

#endif
