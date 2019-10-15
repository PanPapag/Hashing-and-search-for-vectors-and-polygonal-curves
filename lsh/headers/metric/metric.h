#ifndef METRIC
#define METRIC

#include <cmath>
#include <tuple>

namespace metric {
  /** \brief Computes Manhattan Distance of 2 points in R^d
    @par iterator p - iterator of the dataset point
    @par iterator q - iterator of the query point
    @par iterator end - end iterator of query point
      (each points is of dimension D so end of query point signals the
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
    @par iterator end - end iterator of query point
      (each points is of dimension D so end of query point signals the
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
