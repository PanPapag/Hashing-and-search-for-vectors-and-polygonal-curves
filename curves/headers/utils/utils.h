#ifndef UTILS
#define UTILS

#include <sstream>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace utils {
  /* enumerated exit codes */
  typedef enum ExitCode{
    SUCCESS,
    FAIL,
    NO_ARGS,
    INVALID_k_vec,
    INVALID_L_grid,
    INVALID_DATASET,
    INVALID_QUERY,
    INVALID_OUTPUT,
    DATASET_ERROR,
    QUERY_ERROR,
    MEMORY_ERROR
  } ExitCode;
  /**
    InputInfo - Group all input parameters of curve_grid_lsh in a struct
  */

  struct InputInfo {
    std::string input_file;      // name of the relative path to the input file
    std::string query_file;      // name of the relative path to the query file
    std::string output_file;     // name of the relative path to the output file
    uint8_t K_vec = 4;           // number of LSH hash functions for each hashTable
    uint8_t L_grid = 5;          // number of LSH hash tables
    uint32_t N;                  // number of dataset curves
    uint32_t Q;                  // number of query curves
    void Print(void);            // print method of the InputInfo struct
  };
  /** \brief ShowUsage - Prints the usage of the program
    @par const std::string& name - Pass by reference the name of the program
    @par const struct InputInfo &input_info - Pass by reference the input parameters
  */
  void ShowUsage(const std::string& name, const struct InputInfo &input_info);
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
    \brief Gets a std::string to convert in the specified type T
  */
  template <typename T>
  T convert_to (const std::string& str) {
    std::istringstream ss(str);
    T num;
    ss >> num;
    return num;
  }
  /**
    \brief Variadic templated min function
  */
  template <typename T>
  T min(T&& t) {
    return std::forward<T>(t);
  }

  template<typename T0, typename T1, typename... Ts>
  typename std::common_type<T0, T1, Ts... >
    ::type min(T0&& val1, T1&& val2, Ts&&... vs) {
    if (val2 < val1) {
      return min(val2, std::forward<Ts>(vs)...);
    } else {
      return min(val1, std::forward<Ts>(vs)...);
    }
  }
}

#endif
