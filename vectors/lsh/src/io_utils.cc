#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>

#include <stdio.h>
#include <stdlib.h>

#include "../includes/utils.h"
#include "../includes/io_utils.h"
#include "../includes/report_utils.h"

int utils::io::GetDataPoints(std::string &file_name, uint32_t &no_vectors,
  utils::ExitCode &status) {

  FILE *fp;                // To opem the file for reading
  unsigned int count = 0;  // Line counter (result)
  char c;                  // To store a character read from file
  // Open the file
  fp = fopen(file_name.c_str(), "r");
  // Check if file exists
  if (!fp) {
    status = INVALID_DATASET;
    return FAIL;
  }
  // Extract characters from file and store in character c
  for (c = getc(fp); c != EOF; c = getc(fp)) {
    if (c == '\n') {// Increment count if this character is newline
      count = count + 1;
    }
  }
  // Pass info
  no_vectors = count;
  // Close the file
  fclose(fp);
  // everything ok, return SUCCESS
  return SUCCESS;
}

int utils::io::GetPointsDim(std::string &file_name, uint16_t &dim,
  utils::ExitCode &status) {

  std::ifstream file;
  file.open(file_name);
  std::string line;
  // Check if file exists
  if (!file) {
    status = INVALID_DATASET;
    return FAIL;
  }
  else {
    // Get the first line
    if (getline(file, line)) {
      // Get total number of elements
      int count =  std::distance(std::istream_iterator<std::string>(
                    std::istringstream(line) >> std::ws),
                    std::istream_iterator<std::string>());
      // The first one is the vector's id. The remaining determines the dimension
      dim = count - 1;
    }
  }
  // close the file
  file.close();
  // everything ok, return SUCCESS
  return SUCCESS;
}
