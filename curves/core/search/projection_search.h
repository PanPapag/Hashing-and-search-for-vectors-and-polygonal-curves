#ifndef PROJECTION_SEARCH
#define PROJECTION_SEARCH

#include <tuple>

#include "../../../core/search/lsh.h"
#include "../../../core/search/hypercube.h"

namespace search {
  namespace curves {
    /** \brief Executes LSH grid search to compute approximate nearest neighbor
      in curves
      @par L_grid - number of grids
      @par lsh_structures - vector of L_grid LSH structures with L = 1 hash tables
      @par L_grid_query_vectors - each query curve corresponds to L_grid query
        vectors
      @par idx - index to current query
    */
    template <typename T, typename U>
    std::tuple<T,U,double> projection_search(const uint8_t L_grid,
      std::unordered_map<int,std::vector<search::curves::LSH<T,U>>>& lsh_structures,
      const std::vector<std::pair<T,T>>& query_curves,
      const std::unordered_map<U,std::vector<int>>& query_curves_lengths,
      const std::unordered_map<U,std::vector<int>>& query_curves_offsets,
      const std::unordered_map<U,std::vector<double>>& qvectors,
      const int idx, const int M, U id) {

        auto start = high_resolution_clock::now();
        /* Initialize min_dist to max value of type T */
        T min_dist = std::numeric_limits<T>::max();
        /* Initialize correspodent min_id using the C++11 way */
        U min_id{};
        /* Lsh returns a pair of min dist and min id found */
        std::pair<T,U> lsh_result{};
        if(lsh_structures.find(idx) != lsh_structures.end()) {
          for(auto& hash_table: lsh_structures.at(idx)) {
            lsh_result = hash_table.NearestNeighbor(qvectors.at(id),
                                          idx, query_curves, query_curves_lengths.at(id),
                                          query_curves_offsets.at(id),id);
          /* get dist and id return by current lsh NN */
            T dist = std::get<0>(lsh_result);
            U id = std::get<1>(lsh_result);
            /* Check if we have found a better neighbor */
            if (dist < min_dist) {
              min_dist = dist;
              min_id = id;
            }
          }
        }
        auto stop = high_resolution_clock::now();
        duration <double> total_time = duration_cast<duration<double>>(stop - start);
        /* return result as a tuple of min_dist, min_id and total_time */
        return std::make_tuple(min_dist,min_id,total_time.count());
    }
    
    template <typename T, typename U>
    std::tuple<T,U,double> projection_search(const uint8_t L_grid,
      std::unordered_map<int,std::vector<search::curves::HyperCube<T,U>>>& lsh_structures,
      const std::vector<std::pair<T,T>>& query_curves,
      const std::unordered_map<U,std::vector<int>>& query_curves_lengths,
      const std::unordered_map<U,std::vector<int>>& query_curves_offsets,
      const std::unordered_map<U,std::vector<double>>& qvectors,
      const int idx, const int M, U id) {

        auto start = high_resolution_clock::now();
        /* Initialize min_dist to max value of type T */
        T min_dist = std::numeric_limits<T>::max();
        /* Initialize correspodent min_id using the C++11 way */
        U min_id{};
        /* Lsh returns a pair of min dist and min id found */
        std::pair<T,U> lsh_result{};
        if(lsh_structures.find(idx) != lsh_structures.end()) {
          for(auto& hash_table: lsh_structures.at(idx)) {
            lsh_result = hash_table.NearestNeighbor(qvectors.at(id),
                                          idx, query_curves, query_curves_lengths.at(id),
                                          query_curves_offsets.at(id),id);
          /* get dist and id return by current lsh NN */
            T dist = std::get<0>(lsh_result);
            U id = std::get<1>(lsh_result);
            /* Check if we have found a better neighbor */
            if (dist < min_dist) {
              min_dist = dist;
              min_id = id;
            }
          }
        }
        auto stop = high_resolution_clock::now();
        duration <double> total_time = duration_cast<duration<double>>(stop - start);
        /* return result as a tuple of min_dist, min_id and total_time */
        return std::make_tuple(min_dist,min_id,total_time.count());
    }
  }
}

#endif
