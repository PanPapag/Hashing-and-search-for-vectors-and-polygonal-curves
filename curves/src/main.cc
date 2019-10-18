#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <iterator>
#include <random>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "../headers/hash/hash_function.h"
#include "../headers/metric/metric.h"
#include "../headers/utils/utils.h"
#include "../headers/utils/args_utils.h"
#include "../headers/utils/report_utils.h"
#include "../headers/utils/io_utils.h"
#include "../headers/search/lsh.h"
#include "../headers/search/brute_force.h"

#define T double
#define U int

using namespace std::chrono;

int main(int argc, char **argv) {
  utils::InputInfo input_info;
  utils::ExitCode status;
  int exit_code;

  input_info.input_file = "../../datasets/curves/trajectories_dataset.txt";
  //input_info.query_file = "../../datasets/vectors/query_small_id";
  //input_info.output_file = "../../results/vectors/small";

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
  }

  /* Preprocessing input file to get number of dataset curves */
  auto start = high_resolution_clock::now();
  std::cout << "\nGetting number of dataset curves.." << std::endl;
  exit_code = utils::io::GetDataCurves(input_info.input_file, input_info.N, status);
  if (exit_code != utils::SUCCESS) {
    utils::report::ReportError(status);
  }
  auto stop = high_resolution_clock::now();
  duration <double> total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Getting number of dataset curves completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;

  /*
    Read dataset and create 1D vector of pairs which stores sequentially each curve
    of lenth m_i. Also create 1D vector that stores curves' ids and curves' length.
    1D vector of curves representation support cache efficiency and as a result
    faster computations
  */
  start = high_resolution_clock::now();
  std::cout << "\nReading input file.." << std::endl;
  std::vector<std::pair<T,T>> dataset_curves;
  std::vector<U> dataset_ids(input_info.N);
  std::vector<int> dataset_lengths(input_info.N);
  std::vector<int> dataset_offsets(input_info.N);
  exit_code = utils::io::ReadFile<T,U>(input_info.input_file, input_info.N,
    dataset_curves, dataset_ids, dataset_lengths, dataset_offsets, status);
  if (exit_code != utils::SUCCESS) {
    utils::report::ReportError(status);
  }
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Reading input file completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;

  input_info.Print();

  /*
  std::cout << metric::DTWDistance<T>(std::next(dataset_curves.begin(),dataset_offsets[0]),
    std::next(dataset_curves.begin(),dataset_offsets[0] + dataset_lengths[0]),
    std::next(dataset_curves.begin(),dataset_offsets[1]),
    std::next(dataset_curves.begin(),dataset_offsets[1] + dataset_lengths[1]))
    << std::endl;
  */
   /* for (int i = 0; i < input_info.N; ++i) {
    std::cout << "----> Id: " << dataset_ids[i] << " Length: " << dataset_lengths[i] << std::endl;
    for (int j = 0; j < dataset_lengths[i]; ++j) {
      std::cout << "(" << std::get<0>(dataset_curves[dataset_offsets[i] + j]);
      std::cout << "," << std::get<1>(dataset_curves[dataset_offsets[i] + j])
                << ")" << std::endl;
    }
  } */

  return EXIT_SUCCESS;
}
