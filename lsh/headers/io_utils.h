#ifndef IO_UTILS
#define IO_UTILS

#include <iostream>
#include <fstream>
#include <vector>

namespace utils {
  namespace io {
    /** \brief ReadInputFile - Reads dataset file provided by user
      @par std::vector<T> &points - Pass by reference a vector type T which
           represent the N vectors of dimension D
      @par std::vector<K> &ids - Pass by reference a vector type K which stores
           vectos' ids
      @par const struct InputInfo &input_info - Pass by reference the input parameters
      @par const char delim Delimiter - of the dataset file
      @par ExitCode &statues - enumerated ExitCode provided from namespace utils
      return: SUCCESS or FAIL
    */
    template <typename T, typename K>
    int ReadInputFile(std::vector<T> &points, std::vector<K> &ids,
        const struct InputInfo &input_info, const char delim, utils::ExitCode &status) {

      // Open file
      std::ifstream infile;
      infile.open(input_info.input_file);
      // Check if file exists
      if (!infile) {
        status = INVALID_DATASET;
        return FAIL;
      }
      // Read file
      for (size_t i = 0; i < input_info.N && infile; ++i) {
        infile >> ids[i];
        for (size_t j = 0; j < input_info.D; ++j) {
          infile >> points[i * input_info.D + j];
        }
      }
      // close the file
      infile.close();
      return SUCCESS;
    }
    /** \brief ReadQueryFile - Reads query file provided by user
      @par const char *file_name - The relative path for the query file
      @par const char delim - Delimiter of the query file
      @par double &radius - Pass by reference the radius provided in the first
           line of the query file
      @par std::vector<T> &v - Pass by reference a vector type T which
           represent the N vectors of dimension D
      @par ExitCode &statues - enumerated ExitCode provided from namespace utils
      return: SUCCESS or FAIL
    */
    template <typename T>
    int ReadQueryFile(const char *file_name, const char delim, double &radius,
                      std::vector<T> &v, ExitCode &status);
    /** \brief GetN - Get the number of vectors
      @par struct InputInfo &input_info - Pass by reference the input parameters
      @par ExitCode &statues - enumerated ExitCode provided from namespace utils
      return: SUCCESS or FAIL
    */
    int GetN(struct InputInfo &input_info, ExitCode &status);
    /** \brief GetD - Get the dimension of a vector
      @par const char delim - File delimiter
      @par struct InputInfo &input_info - Pass by reference the input parameters
      @par ExitCode &statues - enumerated ExitCode provided from namespace utils
      return: SUCCESS or FAIL
    */
    int GetD(const char delim, struct InputInfo &input_info, ExitCode &status);
  }
}

#endif
