#ifndef UTILS
#define UTILS

#include <tuple>
#include <vector>

namespace utils {
  /* enumerated exit codes */
  typedef enum ExitCode{
    SUCCESS,
    FAIL,
    NO_ARGS,
    INVALID_k,
    INVALID_L,
    INVALID_R,
    INVALID_PARARAMETERS,
    INVALID_DATASET,
    INVALID_QUERY,
    INVALID_OUTPUT,
    DATASET_ERROR,
    QUERY_ERROR,
    MEMORY_ERROR
  } ExitCode;
  /**
    InputInfo - Group all input parameters of LSH in a struct
  */

  struct InputInfo {
    std::string input_file;      // name of the relative path to the input file
    std::string query_file;      // name of the relative path to the query file
    std::string output_file;     // name of the relative path to the output file
    uint8_t K = 4;               // number of LSH hash functions for each hashTable
    uint8_t L = 5;               // number of LSH hash tables
    uint32_t N;                  // number of dataset points
    uint32_t Q;                  // number of query points
    uint16_t D;                  // dimension of the space
    double R = 0.0;              // radius for (r,c) - NN
    void Print(void);            // print method of the InputInfo struct
  };
  /** \brief ShowUsage - Prints the usage of the program
    @par const std::string &name - Pass by reference the name of the program
    @par const struct InputInfo &input_info - Pass by reference the input parameters
  */
  void ShowUsage(const std::string &name, const struct InputInfo &input_info);
  /** \brief Computes both negative and positive modulos
    @par int a - Dividend
    @par int b - modulo divisor
  */
  constexpr int mod(int a, int b) { return (a % b + b) % b; }
  /** \brief Computes modular exponentiation
    @par base - base
    @par exp - expotent
    @par mod - modulo divisor
  */
  uint64_t mod_exp(uint32_t base, uint16_t exp, uint32_t mod);
  /** \brief Computes average distance error between the results provided by
    exeuting the exact Nearest Neighbor and the results provided by executing the
    approximate Nearest NearestNeighbor
  */
  template <typename T, typename U>
  double ComputeAvgDistanceError(std::vector<std::pair<T,U>> &exact,
    std::vector<std::pair<T,U>> &approx) {

    /* Get number of queries executed */
    int n = exact.size();
    /* Sum up difference approx_i - exact_i while exact_i s always >= approx_i */
    double distance_error{};
    for (int i = 0; i < n; ++i) {
      distance_error += std::get<0>(approx[i]) - std::get<0>(exact[i]);
    }
    /* Return its average */
    return distance_error / n;
  }
}

#endif
