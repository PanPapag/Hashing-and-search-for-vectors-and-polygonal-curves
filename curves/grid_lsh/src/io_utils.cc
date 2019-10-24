#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>

#include <stdio.h>
#include <stdlib.h>

#include "../../../core/utils/utils.h"

#include "../includes/utils.h"
#include "../includes/io_utils.h"
#include "../includes/report_utils.h"

int utils::io::GetDataCurves(std::string &file_name, uint32_t &no_curves,
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
  no_curves = count;
  // Close the file
  fclose(fp);
  // everything ok, return SUCCESS
  return SUCCESS;
}
