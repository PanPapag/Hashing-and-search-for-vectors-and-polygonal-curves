#ifndef IO_UTILS
#define IO_UTILS

#include <iostream>
#include <fstream>
#include <vector>

namespace utils {
  namespace io {
    /** \brief ReadFile - Reads file provided by user and
      @par std::string file_name - Path to file
      @par std::vector<T> &points - Pass by reference a vector type T which
           represent the N points of dimension D
      @par std::vector<K> &ids - Pass by reference a vector type K which stores
           points' ids
      @par const int no_points - Number of point in file
      @par const int dim - Points' dimension
      @par ExitCode &statues - enumerated ExitCode provided from namespace utils
      return: SUCCESS or FAIL
    */
    template <typename T, typename K>
    int ReadFile(std::string file_name, const int no_points, const int dim,
       std::vector<T> &points, std::vector<K> &ids, utils::ExitCode &status) {
      // Open file
      std::ifstream infile;
      infile.open(file_name);
      // Check if file exists
      if (!infile) {
        status = INVALID_DATASET;
        return FAIL;
      }
      // Read file
      for (size_t i = 0; i < no_points && infile; ++i) {
        infile >> ids[i];
        for (size_t j = 0; j < dim; ++j) {
          infile >> points[i * dim + j];
        }
      }
      // close the file
      infile.close();
      return SUCCESS;
    }
    /** \brief GetDataPoints - Get the number of file data points
      @par std::string &file_name - Path to file
      @par int &no_vectors - Pass by reference the number of vectors to be returned
      @par ExitCode &statues - enumerated ExitCode provided from namespace utils
      return: SUCCESS or FAIL
    */
    int GetDataPoints(std::string &file_name, int &no_vectors, ExitCode &status);
    /** \brief GetPointsDim - Get the dimension of the points
      @par std::string &file_name - Path to file
      @par int &dim - Pass by reference the points' dimension
      @par ExitCode &statues - enumerated ExitCode provided from namespace utils
      return: SUCCESS or FAIL
    */
    int GetPointsDim(std::string &file_name, int &dim, ExitCode &status);
  }
}

#endif
