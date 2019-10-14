#ifndef BRUTE_FORCE
#define BRUTE_FORCE

#include <limits>
#include "../../headers/metric/metric.h"

namespace search {

  template <typename T, typename U>
  class BruteForce {
    private:
      const uint32_t N;
      const uint16_t D;
      const double R;
      const std::vector<T>  &feature_vector;
      const std::vector<U>  &feature_vector_ids;
    public:
      /** \brief class BruteForce constructor
        @par const std::vector<T> &points - Pass by reference given points
        @par const std::vector<T> &ids - Pass by reference points' ids
        @par const int N  - Number of points
        @par const int D - Points' dimension
        @par const int R - Radius value
      */
      BruteForce(const uint32_t N, const uint16_t D, const double R,
                 const std::vector<T> &points, const std::vector<T> &ids)
        : N(N), D(D), R(R), feature_vector(points), feature_vector_ids(ids) {};
      /**
        \brief class BruteForce default construct
      */
      ~BruteForce() = default;
      /** \brief Executes exact Nearest tNeighbor
        @par const std::vector<T> &query_points - Pass by reference query points
        @par const int offset - Offset to get correspodent point
      */
      std::pair<T,U> NearestNeighbor(const std::vector<T> &query_points,
        const int offset) {

        /* Initialize min_dist to max value of type T */
        T min_dist = std::numeric_limits<T>::max();
        /* Initialize correspodent min_id using the C++11 way */
        U min_id{};
        /* Run NearestNeighbor for all points in the dataset */
        for (int i = 0; i < N; ++i) {
          T dist = metric::ManhattanDistance<T>(
            std::next(feature_vector.begin(), i * D),
            std::next(query_points.begin(), offset * D),
            std::next(query_points.begin(), offset * D + D));
          if (dist < min_dist) {
            min_dist = dist;
            min_id = feature_vector_ids[i];
          }
        }
        /* return result as a pair of min_dist and min_id */
        return std::make_pair(min_dist,min_id);
      };
      /** \brief Executes (r,c)-Nearest tNeighbor
        @par const std::vector<T> &query_points - Pass by reference query points
        @par const int offset - Offset to get correspodent point
      */
      std::vector<std::pair<T,U>> RadiusNearestNeighbor(
        const std::vector<T> &query_points,
        const int offset) {

        /* Define result vector */
        std::vector<std::pair<T,U>> result;
        /* Run (r,c)-NearestNeighbor for all points in the dataset */
        for (int i = 0; i < N; ++i) {
          T dist = metric::ManhattanDistance<T>(
            std::next(feature_vector.begin(), i * D),
            std::next(query_points.begin(), offset * D),
            std::next(query_points.begin(), offset * D + D));
          if (dist <= R) {
            result.push_back(std::make_pair(dist,feature_vector_ids[i]));
          }
        }
        return result;
      };
  };
}

#endif
