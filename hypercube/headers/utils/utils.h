#ifndef UTILS
#define UTILS

#include <cmath>
#include <string>
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
    uint8_t M = 10;              // max number of HyperCube's candidate points to be checked
    uint8_t probes = 2;          // max number of HyperCube's verteces to be checked
    uint32_t N;                  // number of dataset points
    uint32_t Q;                  // number of query points
    uint16_t D;                  // dimension of the space
    uint16_t k;                  // reduces dimensional space
    void Print(void);            // print method of the InputInfo struct
  };
  /** \brief ShowUsage - Prints the usage of the program
    @par const std::string &name - Pass by reference the name of the program
    @par const struct InputInfo &input_info - Pass by reference the input parameters
  */
  void ShowUsage(const std::string& name, const struct InputInfo& input_info);
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
  /**
    \brief Returns a vector of strings with max hamming distance 1
    @par key - Given string from which we extract the ones with hamming distance 1
  */
  std::vector<std::string> GetToggledBitStrings(const std::string& key);
  /** \brief Computes the radius as the average of the distances of each point
    to its nearest neighbor
  */
  template <typename T, typename U>
  double ComputeRadius(std::vector<std::tuple<T,U,double>>& exact) {

    /* Get number of points */
    int N = exact.size();
    /* Sum up distances from the nearest neighbor */
    double distance_to_nn{};
    for (int i = 0; i < N; ++i) {
      distance_to_nn += std::get<0>(exact[i]);
    }
    /* Return its average */
    return distance_to_nn / N;
  }
}

#endif
