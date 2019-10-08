#ifndef METRIC
#define METRIC

#include <cmath>

namespace metric {
  /** \brief Computes Manhattan Distance of 2 points in R^d
    @par iterator p - iterator of the dataset point
    @par iterator p - iterator of the query point
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
    @par iterator p - iterator of the query point
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
}

#endif
