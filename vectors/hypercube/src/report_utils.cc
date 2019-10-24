#include <iostream>

#include "../includes/utils.h"
#include "../includes/report_utils.h"

void utils::report::ReportError(const utils::ExitCode &code) {
  switch (code) {
    case INVALID_k:
      std::cout << "\n[ERROR]: " << "Invalid value of k (number of reduced dimensions)"
                << std::endl;
      break;
    case INVALID_probes:
      std::cout << "\n[ERROR]: " << "Invalid value of probes (max number of probes to be checked)"
                << std::endl;
      break;
    case INVALID_M:
      std::cout << "\n[ERROR]: " << "Invalid value of M (max number of candidate points to be checked)"
                << std::endl;
      break;
    case INVALID_DATASET:
      std::cout << "\n[ERROR]: " << "Invalid dataset file name" << std::endl;
      break;
    case INVALID_QUERY:
      std::cout << "\n[ERROR]: " << "Invalid query file name" << std::endl;
      break;
    case INVALID_OUTPUT:
      std::cout << "\n[ERROR]: " << "Invalid output file name" << std::endl;
      break;
    case DATASET_ERROR:
      std::cout << "\n[ERROR]: " << "Invalid dataset file format" << std::endl;
      break;
    case QUERY_ERROR:
      std::cout << "\n[ERROR]: " << "Invalid query file format" << std::endl;
      break;
    case MEMORY_ERROR:
      std::cout << "\n[ERROR]: " << "System ran out of memory" << std::endl;
      break;
    default:
      abort();
  }
  exit(EXIT_FAILURE);
}
