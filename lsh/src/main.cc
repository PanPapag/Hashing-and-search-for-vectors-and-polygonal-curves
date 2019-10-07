#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "../headers/utils.h"
#include "../headers/args_utils.h"
#include "../headers/report_utils.h"
#include "../headers/io_utils.h"
#include "../headers/xvector.h"
#include "../headers/hash_function.h"

#define T double
#define K int

using namespace std::chrono;

int main(int argc, char **argv) {
  utils::InputInfo input_info;
  utils::ExitCode status;
  const char delim = '\t';
  int exit_code;

  input_info.input_file = "../../datasets/vectors/search_test";

  /* Get arguments */
  /*exit_code = utils::args::ReadArguments(argc, argv, input_info, status);
  switch (exit_code) {
    case utils::SUCCESS:
      std::cout << "\nArguments provided correctly" << std::endl;
      break;
    case utils::FAIL:
      if (status == utils::NO_ARGS) {
        std::cout << "\nNo arguments provided" << std::endl;
        std::cout << "Proceding to input them.." << std::endl;
        exit_code = utils::args::ScanArguments(input_info, status);
        switch (exit_code) {
          case utils::SUCCESS:
            std::cout << "Arguments provided correctly" << std::endl;
            break;
          case utils::FAIL:
            utils::report::ReportError(status);
            break;
          default:
            break;
        }
      } else {
        utils::report::ReportError(status);
      }
      break;
    default:
      break;
  } */

  /* Preprocessing file to get number of vectors and their dimension */
  auto start = high_resolution_clock::now();
  std::cout << "\nGetting number of vectors.." << std::endl;
  exit_code = utils::io::GetN(input_info, status);
  if (exit_code != utils::SUCCESS) {
    utils::report::ReportError(status);
  }
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  std::cout << "Getting number of vectors completed successfully." << std::endl;
  std::cout << "Time elapsed: " << duration.count()
            << " microseconds" << std::endl;

  start = high_resolution_clock::now();
  std::cout << "\nGetting vectors' dimension.." << std::endl;
  exit_code = utils::io::GetD(delim, input_info, status);
  if (exit_code != utils::SUCCESS) {
    utils::report::ReportError(status);
  }
  stop = high_resolution_clock::now();
  duration = duration_cast<microseconds>(stop - start);
  std::cout << "Getting vectors' dimension completed successfully." << std::endl;
  std::cout << "Time elapsed: " << duration.count()
            << " microseconds" << std::endl;

  input_info.Print();
  /*
    Read dataset and create 1D vector which represents the d-dimensional points
    of N vectors. Also create 1D vector that stores vectors' ids.
    1D vector of points representation support cache efficiency and as a result
    faster computations
  */
  std::vector<T> input_points(input_info.N * input_info.D);
  std::vector<K> input_ids(input_info.N);
  exit_code = utils::io::ReadInputFile<T,K>(input_points, input_ids, input_info, delim, status);


  return EXIT_SUCCESS;
}
