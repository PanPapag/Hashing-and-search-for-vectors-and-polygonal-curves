#ifndef LSH_UTILS
#define LSH_UTILS

#include <tuple>
#include <vector>

namespace utils {
  /* enumerated general exit codes */
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
    uint8_t K = 4;               // number of LSH hash functions for each hashTable
    uint8_t L = 5;               // number of LSH hash tables
    uint32_t N;                  // number of dataset points
    uint32_t Q;                  // number of query points
    uint16_t D;                  // dimension of the space
    void Print(void);            // print method of the InputInfo struct
  };
  /** \brief ShowUsage - Prints the usage of the program
    @par const std::string &name - Pass by reference the name of the program
    @par const struct InputInfo &input_info - Pass by reference the input parameters
  */
  void ShowUsage(const std::string &name, const struct InputInfo &input_info);
}

#endif
