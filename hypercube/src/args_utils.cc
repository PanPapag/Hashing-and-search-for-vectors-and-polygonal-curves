#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <getopt.h>
#include <unistd.h>
#include <string>
#include <string.h>

#include "../headers/utils/utils.h"
#include "../headers/utils/args_utils.h"

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

  return SUCCESS;
}

int utils::args::ReadArguments(int argc, char **argv,
  struct InputInfo& input_info, utils::ExitCode& status) {

  if (argc == 1) {
    status = NO_ARGS;
    return FAIL;
  }

  if (argc == 2) {
    if (!strcmp(argv[1],"-help")) {
      ShowUsage(argv[0], input_info);
    }
  }

  if (argc != 13) {
    status = INVALID_PARARAMETERS;
    return FAIL;
  }

  const char* const short_opts = "d:q:k:M:p:o:";
  const option long_opts[] = {
           {"input", required_argument, nullptr, 'd'},
           {"query", required_argument, nullptr, 'q'},
           {"k", required_argument, nullptr, 'k'},
           {"M", required_argument, nullptr, 'M'},
           {"probes", required_argument, nullptr, 'p'},
           {"output", required_argument, nullptr, 'o'},
           {nullptr, no_argument, nullptr, 0}
  };

  while (true) {

    const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

    if (-1 == opt) {
      break;
    }

    switch (opt) {
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
          input_info.k = atoi(optarg);
        } catch (...) {
          status = INVALID_k;
          return FAIL;
        }
        break;
      }
      case 'M': {
        try {
          input_info.M = atoi(optarg);
        } catch (...) {
          status = INVALID_L;
          return FAIL;
        }
        break;
      }
      case 'p': {
        try {
          input_info.probes = atoi(optarg);
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
