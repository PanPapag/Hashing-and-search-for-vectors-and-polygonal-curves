#ifndef GRID_HYPERCUBE_UTILS
#define GRID_HYPERCUBE_UTILS

#include <cmath>
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
    INVALID_k_hypercube,
    INVALID_M,
    INVALID_probes,
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
    uint8_t M = 10;              // max number of HyperCube's candidate points to be checked
    uint8_t probes = 2;          // max number of HyperCube's verteces to be checked
    uint16_t k_hypercube = 3;    // reduces dimensional space
    uint8_t L_grid = 5;          // number of HyperCube Structures
    uint32_t N;                  // number of dataset curves
    uint32_t Q;                  // number of query curves
    void Print(void);            // print method of the InputInfo struct
  };
  /** \brief ShowUsage - Prints the usage of the program
    @par const std::string& name - Pass by reference the name of the program
    @par const struct InputInfo &input_info - Pass by reference the input parameters
  */
  void ShowUsage(const std::string& name, const struct InputInfo& input_info);
}

#endif
