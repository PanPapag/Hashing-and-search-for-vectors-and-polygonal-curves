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
  namespace curves {

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
        const std::vector<double> &feature_vector;
        const std::vector<std::pair<T,T>>& input_curves;
        const std::vector<U> &input_curves_ids;
        const std::vector<int>& input_curves_lengths;
        const std::vector<int>& input_curves_offsets;
        std::vector<hash::AmplifiedHashFunction<double>> hash_functions;
        std::vector<std::unordered_map<int,std::vector<int>>> hash_tables;
      public:
        /**
          \brief class LSH constructor
        */
        LSH(const uint8_t K, const uint8_t L, const uint16_t D, const uint32_t N,
            const double r, const std::vector<std::pair<T,T>>& curves,
            const std::vector<U> &ids, const std::vector<int>& lengths,
            const std::vector<int>& offsets, const std::vector<T> &points)
          : K(K), L(L), D(D), N(N), R(r), input_curves(curves),
            input_curves_ids(ids), input_curves_lengths(lengths),
            input_curves_offsets(offsets), feature_vector(points) {
            w = 4 * R;
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
        /** \brief Executes approximate Nearest tNeighbor
          @par const std::vector<T>& query_points - Pass by reference query points
          @par const int offset - Offset to get correspodent point
        */
        std::pair<T,U> NearestNeighbor(const std::vector<T>& query_points,
          const int offset, const std::vector<std::pair<T,T>>& query_curves,
          const std::vector<int>& query_curves_lengths,
          const std::vector<int>& query_curves_offsets) {

          /* Initialize min_dist to max value of type T */
          T min_dist = std::numeric_limits<T>::max();
          /* Initialize correspodent min_id using the C++11 way */
          U min_id{};
          for (int i = 0; i < L; ++i) {
            // get i_th hashtable
            std::unordered_map<int,std::vector<int>> &ht_i = hash_tables[i];
            // get all curves in the same bucket
            std::vector<int> &bucket = ht_i[hash_functions[i].Hash(query_points,offset) % table_size];
            // iterate over all curves in the bucket
            for (auto const& fv_offset: bucket) {
              T dist =  metric::DTWDistance<T>(
                std::next(input_curves.begin(),input_curves_offsets[fv_offset]),
                std::next(input_curves.begin(),input_curves_offsets[fv_offset] + input_curves_lengths[fv_offset]),
                std::next(query_curves.begin(),query_curves_offsets[offset]),
                std::next(query_curves.begin(),query_curves_offsets[offset] + query_curves_lengths[offset]));
              if (dist < min_dist) {
                min_dist = dist;
                min_id = input_curves_ids[fv_offset];
              }
            }
          }

          /* return result as a tuple of min_dist and min_id */
          return std::make_pair(min_dist,min_id);
        };


    };
  }
}

#endif
