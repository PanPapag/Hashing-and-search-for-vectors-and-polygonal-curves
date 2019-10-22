#ifndef BRUTE_FORCE
#define BRUTE_FORCE

#include <limits>
#include <tuple>
#include "../../headers/metric/metric.h"

using namespace std::chrono;

namespace search {
  namespace curves {

    template <typename T, typename U>
    class BruteForce {
      private:
        const std::vector<std::pair<T,T>>& input_curves;
        const std::vector<U>& input_curves_ids;
        const std::vector<int>& input_curves_lengths;
        const std::vector<int>& input_curves_offsets;
      public:
        /** \brief class BruteForce constructor
          @par const std::vector<T>& curves - Pass by reference given curves
          @par const std::vector<U>& ids - Pass by reference curves' ids
          @par const std::vector<int>& lengths - Pass by reference curves' lengths
          @par const std::vector<int>& curves_offsets - Pass by reference offsets
            to given curves to have access on them
        */
        BruteForce(const std::vector<std::pair<T,T>>& curves,
          const std::vector<U>& ids, const std::vector<int>& lenghts,
          const std::vector<int>& offsets)
          : input_curves(curves), input_curves_ids(ids),
            input_curves_lengths(lenghts), input_curves_offsets(offsets) {};
        /**
          \brief class BruteForce default construct
        */
        ~BruteForce() = default;
        /** \brief Executes exact Nearest Neighbor for curves
          @par const std::vector<std::pair<T,T>>& query_curves - Pass by reference
            query curves
          @par const std::vector<int>& query_lengths - Pass by reference query
            curves' lengths
          @par const std::vector<int> query_offsets - Pass by reference offsets
            to given query curves to have access on them
          @par const int idx - Index to current query curve
        */
        std::tuple<T,U,double> NearestNeighbor(
          const std::vector<std::pair<T,T>>& query_curves,
          const std::vector<int>& query_lengths,
          const std::vector<int>& query_offsets, const int idx) {

          auto start = high_resolution_clock::now();
          /* Initialize min_dist to max value of type T */
          T min_dist = std::numeric_limits<T>::max();
          /* Initialize correspodent min_id using the C++11 way */
          U min_id{};
          /* Run NearestNeighbor for all points in the input */
          for (size_t i = 0; i < input_curves_ids.size(); ++i) {
            T dist = metric::DTWDistance<T>(
              std::next(input_curves.begin(),input_curves_offsets[i]),
              std::next(input_curves.begin(),input_curves_offsets[i] + input_curves_lengths[i]),
              std::next(query_curves.begin(),query_offsets[idx]),
              std::next(query_curves.begin(),query_offsets[idx] + query_lengths[idx]));
            if (dist < min_dist) {
              min_dist = dist;
              min_id = input_curves_ids[i];
            }
          }
          auto stop = high_resolution_clock::now();
          duration <double> total_time = duration_cast<duration<double>>(stop - start);
          /* return result as a tuple of min_dist, min_id and total_time */
          return std::make_tuple(min_dist,min_id,total_time.count());
        };
    };
  }
}

#endif
