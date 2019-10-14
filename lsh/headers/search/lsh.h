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
      const std::vector<T> &feature_vector;
      const std::vector<U> &feature_vector_ids;
      std::vector<hash::AmplifiedHashFunction<T>> hash_functions;
      std::vector<std::unordered_map<int,std::vector<int>>> hash_tables;
    public:
      /** \brief class LSH constructor
      */
      LSH(const uint8_t K, const uint8_t L, const uint16_t D, const uint32_t N,
          const std::vector<T> &points, const std::vector<T> &ids)
        : K(K), L(L), D(D), N(N), feature_vector(points), feature_vector_ids(ids) {
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
            std::unordered_map<int,std::vector<int>> ht;
            for (int j = 0; j < N; ++j) {
              ht[hash_functions[i].Hash(feature_vector,j) % table_size].push_back(j);
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
          for (int j = 0; j < D; ++j) {
            avg_coord[i] += points[v[i] * D + i];
          }
          avg_coord[i] /= D;
        }
        // average them to compute window
        for (int i = 0; i < sample_n; ++i) {
          w += avg_coord[i];
        }
        w /= double(sample_n);

        return w;
      }
      /** \brief Executes approximate Nearest tNeighbor
        @par const std::vector<T> &query_points - Pass by reference query points
        @par const int offset - Offset to get correspodent point
      */
      std::tuple<T,U,double> NearestNeighbor(const std::vector<T> &query_points,
        const int offset) {

        auto start = high_resolution_clock::now();
        /* Initialize min_dist to max value of type T */
        T min_dist = std::numeric_limits<T>::max();
        /* Initialize correspodent min_id using the C++11 way */
        U min_id{};
        for (int i = 0; i < L; ++i) {
          // get i_th hashtable
          std::unordered_map<int,std::vector<int>> &ht_i = hash_tables[i];
          // get all points in the same bucket
          std::vector<int> &bucket = ht_i[hash_functions[i].Hash(query_points,offset) % table_size];
          // if large number of retrieved items then continue to the next hashtable
          if (bucket.size() > 5 * L) continue;
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
      std::vector<std::pair<T,U>> RadiusNearestNeighbor(const std::vector<T> &query_points,
        const int offset, const double R) {

        /* Define result vector */
        std::vector<std::pair<T,U>> result;
        /* Initialize min_dist to max value of type T */
        T min_dist = std::numeric_limits<T>::max();
        /* Initialize correspodent min_id using the C++11 way */
        U min_id{};
        for (int i = 0; i < L; ++i) {
          // get i_th hashtable
          std::unordered_map<int,std::vector<int>> &ht_i = hash_tables[i];
          // get all points in the same bucket
          std::vector<int> &bucket = ht_i[hash_functions[i].Hash(query_points,offset) % table_size];
          // if large number of retrieved items then continue to the next hashtable
          if (bucket.size() > 5 * L) continue;
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
