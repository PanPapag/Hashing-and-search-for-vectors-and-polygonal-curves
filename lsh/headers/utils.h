#ifndef UTILS
#define UTILS

namespace utils {
  /**
    input_info_t - Groups all input parameters of LSH in a struct
  */
  struct InputInfo {
    std::string input_file;      // name of the relative path to the input file
    std::string query_file;      // name of the relative path to the query file
    std::string output_file;     // name of the relative path to the output file
    int k = 4;                   // number of LSH hash functions
    int L = 5;                   // number of LSH hash tables
  };
  /**
    Usage - Prints the usage of the program
    @par const std::string &name Pass by reference the name of the program
    @par const struct InputInfo &input_info Pass by reference the input parameters
  */
  [[noreturn]]
  void ShowUsage(const std::string &name, const struct InputInfo &input_info);
}

#endif
