#ifndef METRIC
#define METRIC

#include <utility>
#include <cmath>
#include <tuple>

#include "../utils/utils.h"

namespace metric {
  /** \brief Computes Dynamic Time Warping between two curves
    @par iterator p - iterator of the dataset curve
    @par iterator p_end - end iterator of dataset curve
    @par iterator q - iterator of the query curve
    @par iterator q_end - end iterator of query curve
      (each curve represents as a vector of pairs in which a pair
      represents a 2D-point)
  */
  template <typename T, typename iterator>
  T DTWDistance(iterator p, iterator p_end, iterator q, iterator q_end) {
    T dtw_distance{};
    // save up start iterator of query curve
    iterator q_start = q;
    // Get correspodent lengths of the two curves
    int N = std::distance(p,p_end);
    int M = std::distance(q,q_end);
    int dtw_size = (N + 1) * (M + 1);
    // Initialize the dtw 2D array using 1D notation
    auto dtw_array = new T [dtw_size];
    dtw_array[0] = 0;
    for (int i = 1; i < N + 1; ++i) {
      dtw_array[i] = std::numeric_limits<T>::max();
    }
    for (int i = 1; i < M + 1; ++i) {
      dtw_array[i * (M + 1)] = std::numeric_limits<T>::max();
    }
    // Compute the dtw distance using dynamic programming
    for (int i = 1; i < N + 1, p < p_end; ++i, ++p) {
      for (int j = 1; j < M + 1, q < q_end; ++j, ++q) {
        dtw_array[i * (M + 1) + j] =
            std::abs(std::get<0>(*p) - std::get<0>(*q))         // Manhattan
          + std::abs(std::get<1>(*p) - std::get<1>(*q))         // Distance
          + utils::min(dtw_array[(i - 1) * (M + 1) + j],        // increment
                       dtw_array[i * (M + 1) + j - 1],          // deletion
                       dtw_array[(i - 1) * (M + 1) + j - 1]);   // match
      }
      // reset query curve iterator
      q = q_start;
    }
    // Set result to a new variable and free memory allocated
    dtw_distance = dtw_array[dtw_size - 1];
    delete[] dtw_array;
    // Return dynamic time warping distance
    return dtw_distance;
  }

  /** \brief Computes Manhattan Distance of 2 points in R^d
    @par iterator p - iterator of the dataset point
    @par iterator q - iterator of the query point
    @par iterator q_end - end iterator of query point
      (each point is of dimension D so end of query point signals the
       final coordinate of dataset point as well)
    returns: manhattan_distance type of T
  */
  template <typename T, typename iterator>
  T ManhattanDistance(iterator p, iterator q, iterator q_end) {
    T manhattan_distance{};
    for (; q < q_end; ++q, ++p) {
      manhattan_distance += std::abs(*q - *p);
    }
    return manhattan_distance;
  }
  /** \brief Computes Euclidian Distance of 2 points in R^d
    @par iterator p - iterator of the dataset point
    @par iterator q - iterator of the query point
    @par iterator q_end - end iterator of query point
      (each point is of dimension D so end of query point signals the
       final coordinate of dataset point as well)
    returns: euclidian_distance type of T
  */
  template <typename T, typename iterator>
  T SquaredEuclidianDistance(iterator p, iterator q, iterator q_end) {
    T manhattan_distance{};
    for (; q < q_end; ++q, ++p) {
      manhattan_distance += (*q - *p) * (*q - *p);
    }
    return manhattan_distance;
  }
  /**
    \brief Computes average and max distance ratio appox_dists / exact_dists
  */
  template <typename T, typename U>
  std::pair<double,double> EvaluationMetric(std::vector<std::tuple<T,U,double>> &exact,
    std::vector<std::tuple<T,U,double>> &approx) {

    int N = exact.size();
    double distance_error;
    double af{};
    double avg_af{};
    double max_af = std::numeric_limits<double>::min();

    for (int i = 0; i < N; ++i) {
      distance_error = (double) std::get<0>(approx[i]) / std::get<0>(exact[i]);
      if (distance_error > max_af) {
        max_af = distance_error;
      }
      af += distance_error;
    }
    avg_af = af / N ;

    return std::make_pair(max_af,avg_af);
  }
}

#endif
