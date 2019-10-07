#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>

#include "../headers/utils.h"
#include "../headers/args_utils.h"

int utils::args::ScanArguments(struct InputInfo &input_info, utils::ExitCode &status) {

  std::string input_buffer;

  std::cout << "Provide the relative path for the input file: ";
  std::cin >> input_buffer;
  input_info.input_file = input_buffer;

  std::cout << "Provide the relative path for the query file: ";
  std::cin >> input_buffer;
  input_info.query_file = input_buffer;

  std::cout << "Provide the relative path for the output file: ";
  std::cin >> input_buffer;
  input_info.output_file = input_buffer;

  do {
    std::cout << "Do you want to provide hyperparameters for LSH (y/n)? : ";
    std::cin >> input_buffer;
    if (input_buffer != "y" && input_buffer != "n") {
      std::cout << "Wrong input! Try again." << std::endl;
    }
  } while (input_buffer != "y" && input_buffer != "n");

  if (input_buffer != "n") {
    std::cout << "Provide the number of LSH hash functions: ";
    std::cin >> input_buffer;
    try {
      input_info.K = stoi(input_buffer);
    } catch (...) {
      status = INVALID_k;
      return FAIL;
    }

    std::cout << "Provide the number of LSH hash tables: ";
    std::cin >> input_buffer;
    try {
      input_info.L = stoi(input_buffer);
    } catch (...) {
      status = INVALID_L;
      return FAIL;
    }
  }
  return SUCCESS;
}

int utils::args::ReadArguments(int argc, char **argv,
  struct InputInfo &input_info, utils::ExitCode &status) {

  if (argc == 2) {
    if (argv[1] == "-help") {
      ShowUsage(argv[0], input_info);
    }
  }

  if (argc != 11) {
    status = INVALID_PARARAMETERS;
    return FAIL;
  }

  int c;
  while ((c = getopt(argc, argv, "d:q:k:L:o:")) != -1) {
    switch (c) {
      case 'd': {
        input_info.input_file = optarg;
        break;
      }
      case 'q' : {
        input_info.query_file = optarg;
        break;
      }
      case 'k': {
        try {
          input_info.K = atoi(optarg);
        } catch (...) {
          status = INVALID_k;
          return FAIL;
        }
        break;
      }
      case 'L': {
        try {
          input_info.L = atoi(optarg);
        } catch (...) {
          status = INVALID_L;
          return FAIL;
        }
        break;
      }
      case 'o': {
        input_info.output_file = optarg;
        break;
      }
      case '?':
        break;
      default:
        abort();
    }
  }
  return SUCCESS;
}
