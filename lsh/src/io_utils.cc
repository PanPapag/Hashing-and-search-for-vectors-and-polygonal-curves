#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>

#include "../headers/utils.h"
#include "../headers/io_utils.h"
#include "../headers/report_utils.h"

int utils::io::ReadDatasetFile(const char *file_name, const char delim,
  std::vector<int> &vectors, utils::ExitCode &status) {

  FILE *fp = fopen(file_name, "r");
  if (fp == NULL) // TODO add report
    exit(EXIT_FAILURE);

  char *line = NULL;
  size_t len = 0;

  while ((getline(&line, &len, fp)) != -1) {
    // convert char *line to string
    std::string vector_record(line);
    // process string
    std::string vector_id = vector_record.substr(0, vector_record.find('\t'));
    std::cout << vector_record;
  }
  // close file pointer
  fclose(fp);
  // clear any memory left
  if (line)
    free(line);
}

int utils::io::GetN(const char *file_name, struct InputInfo &input_info,
  utils::ExitCode &status) {

  FILE *fp;                // To opem the file for reading
  unsigned int count = 0;  // Line counter (result)
  char c;                  // To store a character read from file
  // Open the file
  fp = fopen(file_name, "r");
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
  // Pass number of vectors into the input_info struct
  input_info.N = count;
  // Close the file
  fclose(fp);
  // everything ok, return SUCCESS
  return SUCCESS;
}

int utils::io::GetD(const char *file_name, char delim,
  struct InputInfo &input_info, utils::ExitCode &status) {

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
      std::stringstream temp(line);
      std::string token;
      unsigned int count = 0;
      while (getline(temp, token, delim)) {
        count++;
      }
      input_info.D = count - 1;
    }
  }
  // close the file
  file.close();
  // everything ok, return SUCCESS
  return SUCCESS;
}
