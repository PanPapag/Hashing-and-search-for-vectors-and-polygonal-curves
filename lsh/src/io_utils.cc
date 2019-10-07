#include <iostream>
#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>

#include "../headers/utils.h"
#include "../headers/io_utils.h"

int utils::io::ReadDatasetFile(const char *file_name, const char delim, std::vector<int> &vectors) {
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
  // Return SUCCESS
  return SUCCESS;
}
