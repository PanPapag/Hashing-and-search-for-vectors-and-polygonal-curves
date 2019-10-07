#ifndef PARSER
#define PARSER

namespace utils {
  namespace parser {
    /** \brief ReadDatasetFile - Reads dataset file provided by user
      @par const char *file_name - The relative path for the dataset file
      @par const char delim Delimiter - of the dataset file
      @par std::vector<Vector> &vectors - Pass by reference a vector of defined
           class Vector
      return: ExitCode
    */
    int ReadDatasetFile(const char *file_name, const char delim, std::vector<int> &vectors);
    /** \brief ReadQueryFile - Reads query file provided by user
      @par const char *file_name The relative path for the query file
      @par const char delim Delimiter of the query file
      @par double &radius Pass by reference the radius provided in the first
           line of the query file
      @par std::vector<Vector> &vectors Pass by reference a vector of defined
           class Vector
      return: ExitCode
    */
    int ReadQueryFile(const char *file_name, const char delim, double &radius, std::vector<int> &vectors);
  }
}

#endif
