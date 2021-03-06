#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <getopt.h>
#include <unistd.h>
#include <string>
#include <string.h>

#include "../includes/utils.h"
#include "../includes/args_utils.h"

int utils::args::ScanArguments(struct InputInfo& input_info,
  utils::ExitCode& status) {

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
    std::cout << "Do you want to provide hyperparameters for HyperCube (y/n)? : ";
    std::cin >> input_buffer;
    if (input_buffer != "y" && input_buffer != "n") {
      std::cout << "Wrong input! Try again." << std::endl;
    }
  } while (input_buffer != "y" && input_buffer != "n");

  if (input_buffer != "n") {
    std::cout << "Provide the number of reduced dimensions: ";
    std::cin >> input_buffer;
    try {
      input_info.k_hypercube = stoi(input_buffer);
    } catch (...) {
      status = INVALID_k_hypercube;
      return FAIL;
    }

    std::cout << "Provide the max number of candidate probes to be checked: ";
    std::cin >> input_buffer;
    try {
      input_info.probes = stoi(input_buffer);
    } catch (...) {
      status = INVALID_probes;
      return FAIL;
    }

    std::cout << "Provide the max number of candidate points to be checked: ";
    std::cin >> input_buffer;
    try {
      input_info.M = stoi(input_buffer);
    } catch (...) {
      status = INVALID_M;
      return FAIL;
    }
  }

  return SUCCESS;
}

int utils::args::ReadArguments(int argc, char **argv,
  struct InputInfo& input_info, utils::ExitCode& status) {

  if (argc == 1) {
    status = NO_ARGS;
    return FAIL;
  }

  if (argc == 2) {
    if (!strcmp(argv[1],"--help")) {
      ShowUsage(argv[0], input_info);
    }
  }

  const char * const short_opts = "d:q:k:M:p:L:e:o:";
  const option long_opts[] = {
           {"input", required_argument, nullptr, 'd'},
           {"query", required_argument, nullptr, 'q'},
           {"k_hypercube", optional_argument, nullptr, 'k'},
           {"M", optional_argument, nullptr, 'M'},
           {"probes", optional_argument, nullptr, 'p'},
           {"L_vec", optional_argument, nullptr, 'L'},
           {"e", optional_argument, nullptr, 'e'},
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
        if (optarg != 0) {
          try {
            input_info.k_hypercube = atoi(optarg);
          } catch (...) {
            status = INVALID_k_hypercube;
            return FAIL;
          }
        }
        break;
      }
      case 'M': {
        try {
          input_info.M = atoi(optarg);
        } catch (...) {
          status = INVALID_M;
          return FAIL;
        }
        break;
      }
      case 'p': {
        if (optarg != 0) {
          try {
            input_info.probes = atoi(optarg);
          } catch (...) {
            status = INVALID_probes;
            return FAIL;
          }
        }
        break;
      }
      case 'L': {
        if (optarg != 0) {
          try {
            input_info.L_vec = atoi(optarg);
          } catch (...) {
            status = INVALID_L_vec;
            return FAIL;
          }
        }
        break;
      }
      case 'e': {
        if (optarg != 0) {
          try {
            input_info.e = atof(optarg);
          } catch (...) {
            status = INVALID_e;
            return FAIL;
          }
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
