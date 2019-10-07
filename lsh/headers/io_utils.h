#ifndef IO_UTILS
#define IO_UTILS

namespace utils {
  namespace io {
    /** \brief GetN - Get the number of vectors
      @par const char *file_name - The relative path for the dataset file
      @par const struct InputInfo &input_info - Pass by reference the input parameters
      @par ExitCode &statues - enumerated ExitCode provided from namespace utils
      return: SUCCESS or FAIL
    */
    int GetN(const char *file_name, struct InputInfo &input_info, ExitCode &status);
    /** \brief GetD - Get the dimension of a vector
      @par const char *file_name - The relative path for the dataset file
      @par char delim - File delimiter
      @par const struct InputInfo &input_info - Pass by reference the input parameters
      @par ExitCode &statues - enumerated ExitCode provided from namespace utils
      return: SUCCESS or FAIL
    */
    int GetD(const char *file_name, char delim,
             struct InputInfo &input_info, ExitCode &status);
    /** \brief ReadDatasetFile - Reads dataset file provided by user
      @par const char *file_name - The relative path for the dataset file
      @par const char delim Delimiter - of the dataset file
      @par std::vector<Vector> &vectors - Pass by reference a vector of defined
           class Vector
      @par ExitCode &statues - enumerated ExitCode provided from namespace utils
      return: SUCCESS or FAIL
    */
    int ReadDatasetFile(const char *file_name, const char delim,
                        std::vector<int> &vectors, ExitCode &status);
    /** \brief ReadQueryFile - Reads query file provided by user
      @par const char *file_name - The relative path for the query file
      @par const char delim - Delimiter of the query file
      @par double &radius - Pass by reference the radius provided in the first
           line of the query file
      @par std::vector<Vector> &vectors - Pass by reference a vector of defined
           class Vector
      @par ExitCode &statues - enumerated ExitCode provided from namespace utils
      return: SUCCESS or FAIL
    */
    int ReadQueryFile(const char *file_name, const char delim, double &radius,
                      std::vector<int> &vectors, ExitCode &status);
  }
}

#endif
