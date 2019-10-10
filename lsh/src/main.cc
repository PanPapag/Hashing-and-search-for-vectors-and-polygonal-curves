#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "../headers/utils.h"
#include "../headers/args_utils.h"
#include "../headers/report_utils.h"
#include "../headers/io_utils.h"
#include "../headers/metric.h"
#include "../headers/hash_function.h"
#include "../headers/hash_table.h"
#include "../headers/search/lsh.h"

#define T int
#define U int

using namespace std::chrono;

int main(int argc, char **argv) {
  utils::InputInfo input_info;
  utils::ExitCode status;
  int exit_code;

  input_info.input_file = "../../datasets/vectors/input_small_id";
  input_info.query_file = "../../datasets/vectors/query_small_id";

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

  /* Preprocessing input file to get number of dataset points and their dimension */
  auto start = high_resolution_clock::now();
  std::cout << "\nGetting number of dataset points.." << std::endl;
  exit_code = utils::io::GetDataPoints(input_info.input_file, input_info.N, status);
  if (exit_code != utils::SUCCESS) {
    utils::report::ReportError(status);
  }
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  std::cout << "Getting number of dataset points completed successfully." << std::endl;
  std::cout << "Time elapsed: " << duration.count() << " ms" << std::endl;

  start = high_resolution_clock::now();
  std::cout << "\nGetting dataset points' dimension.." << std::endl;
  exit_code = utils::io::GetPointsDim(input_info.input_file,input_info.D, status);
  if (exit_code != utils::SUCCESS) {
    utils::report::ReportError(status);
  }
  stop = high_resolution_clock::now();
  duration = duration_cast<microseconds>(stop - start);
  std::cout << "Getting dataset points' dimension completed successfully." << std::endl;
  std::cout << "Time elapsed: " << duration.count() << " ms" << std::endl;

  /*
    Read dataset and create 1D vector which represents the d-dimensional points
    of N vectors. Also create 1D vector that stores vectors' ids.
    1D vector of points representation support cache efficiency and as a result
    faster computations
  */
  start = high_resolution_clock::now();
  std::cout << "\nReading input file.." << std::endl;
  std::vector<T> dataset_points(input_info.N * input_info.D);
  std::vector<U> dataset_ids(input_info.N);
  exit_code = utils::io::ReadFile<T,U>(input_info.input_file, input_info.N,
    input_info.D, dataset_points, dataset_ids, status);
  if (exit_code != utils::SUCCESS) {
    utils::report::ReportError(status);
  }
  stop = high_resolution_clock::now();
  duration = duration_cast<microseconds>(stop - start);
  std::cout << "Reading input file completed successfully." << std::endl;
  std::cout << "Time elapsed: " << duration.count() << " ms" << std::endl;

  /* Preprocessing query file */
  start = high_resolution_clock::now();
  std::cout << "\nGetting number of query points.." << std::endl;
  exit_code = utils::io::GetDataPoints(input_info.query_file, input_info.Q, status);
  if (exit_code != utils::SUCCESS) {
    utils::report::ReportError(status);
  }
  stop = high_resolution_clock::now();
  duration = duration_cast<microseconds>(stop - start);
  std::cout << "Getting number of query points completed successfully." << std::endl;
  std::cout << "Time elapsed: " << duration.count() << " ms" << std::endl;

  /* Reading query file */
  start = high_resolution_clock::now();
  std::cout << "\nReading query file.." << std::endl;
  std::vector<T> query_points(input_info.Q * input_info.D);
  std::vector<U> query_ids(input_info.Q);
  exit_code = utils::io::ReadFile<T,U>(input_info.query_file, input_info.Q,
    input_info.D, query_points, query_ids, status);  if (exit_code != utils::SUCCESS) {
    utils::report::ReportError(status);
  }
  stop = high_resolution_clock::now();
  duration = duration_cast<microseconds>(stop - start);
  std::cout << "Reading query file completed successfully." << std::endl;
  std::cout << "Time elapsed: " << duration.count() << " ms" << std::endl;

  input_info.Print();
  //print dataset
  /* for (int i = 0; i < input_info.N; i++)  {
    std::cout << dataset_ids[i] << " ";
    for (int j = 0; j < input_info.D; j++)
      std::cout << dataset_points[i * input_info.D + j] << " ";
    std::cout << std::endl;
  } */

  /*std::cout << std::endl;
  start = high_resolution_clock::now();
  std::cout << "\nDistance: " << metric::SquaredEuclidianDistance<T>(dataset_points.begin(),
    dataset_points.begin() + 2*input_info.D, (dataset_points.begin() + 2*input_info.D) + input_info.D)
    << std::endl;
    stop = high_resolution_clock::now();
  duration = duration_cast<microseconds>(stop - start);
  std::cout << "Time elapsed to manhattan_distance: " << duration.count() << " ms" << std::endl;
  */

  //uint64_t largeword = ((uint16_t) 10 << 32) + ((uint16_t) 2 << 16) + ((uint16_t) 3);
  LSH_ <int> *HashTables = new LSH_ <int>(input_info);
  return EXIT_SUCCESS;
}
