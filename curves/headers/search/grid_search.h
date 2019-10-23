#ifndef GRID_SEARCH
#define GRID_SEARCH

#include <tuple>

#include "../../headers/search/lsh.h"

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
    std::tuple<T,U,double> lsh_grid_search(const uint8_t L_grid,
      std::vector<search::curves::LSH<T,U>>& lsh_structures,
      const std::vector<std::pair<T,T>>& query_curves,
      const std::vector<int>& query_curves_lengths,
      const std::vector<int>& query_curves_offsets,
      const std::vector<std::vector<double>>& L_grid_query_vectors,
      const int idx) {

        auto start = high_resolution_clock::now();
        /* Initialize min_dist to max value of type T */
        T min_dist = std::numeric_limits<T>::max();
        /* Initialize correspodent min_id using the C++11 way */
        U min_id{};
        /* Lsh returns a pair of min dist and min id found */
        std::pair<T,U> lsh_result{};
        for (size_t i = 0; i < L_grid; ++i) {
          lsh_result = lsh_structures[i].NearestNeighbor(L_grid_query_vectors[i],
                                        idx, query_curves, query_curves_lengths,
                                        query_curves_offsets);
          T dist = std::get<0>(lsh_result);
          U id = std::get<1>(lsh_result);
          if (dist < min_dist) {
            min_dist = dist;
            min_id = id;
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
