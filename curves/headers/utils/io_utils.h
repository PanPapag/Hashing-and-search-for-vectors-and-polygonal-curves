#ifndef IO_UTILS
#define IO_UTILS

#include <algorithm>
#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>

namespace utils {
  namespace io {
    /** \brief ReadFile - Reads file provided by user
      @par std::string& file_name - Pass by reference the path to the input file
      @par std::vector<T>& curves - Pass by reference a vector type T which
           represent the N curves of length m_i, i = 1..N
      @par std::vector<K>& ids - Pass by reference a vector type K which stores
           curves' ids
      @par std::vector<T>& lengths - Pass by reference a vector type int which
           stores curves' lenths
      @par std::vector<T>& offsets - Pass by reference a vector type int which
           stores offesets to the std::vector<std::pair<T,T>>& curves
      @par const int no_curves - Number of point in file
      @par ExitCode& statues - enumerated ExitCode provided from namespace utils
      return: SUCCESS or FAIL
    */
    template <typename T, typename K>
    int ReadFile(std::string& file_name, const int no_curves,
      std::vector<std::pair<T,T>>& curves, std::vector<K>& ids,
      std::vector<int>& lengths, std::vector<int>& offsets, utils::ExitCode& status) {

      // Open file
      std::ifstream infile;
      infile.open(file_name);
      // Check if file exists
      if (!infile) {
        status = INVALID_DATASET;
        return FAIL;
      }
      std::string buffer_x;
      std::string buffer_y;
      int offset{};
      // Read file
      for (size_t i = 0; i < no_curves && infile; ++i) {
        infile >> ids[i];
        infile >> lengths[i];
        for (size_t j = 0; j < lengths[i]; ++j) {
          infile >> buffer_x;
          infile >> buffer_y;
          // remove noise characters such as '(', to convert successuflly to T
          buffer_x.erase(std::remove(buffer_x.begin(),buffer_x.end(),'('), buffer_x.end());
          T point_x = convert_to<T>(buffer_x);
          T point_y = convert_to<T>(buffer_y);
          curves.push_back(std::make_pair(point_x,point_y));
        }
        offsets[i] = offset;
        offset += lengths[i];
      }
      // close the file
      infile.close();
      return SUCCESS;
    }
    /** \brief GetDataCurves - Get the number of file data curves
      @par std::string& file_name - Path to file
      @par int& no_vectors - Pass by reference the number of curves to be returned
      @par ExitCode& statues - enumerated ExitCode provided from namespace utils
      return: SUCCESS or FAIL
    */
    int GetDataCurves(std::string& file_name, uint32_t& no_vectors, ExitCode& status);
  }
}

#endif
