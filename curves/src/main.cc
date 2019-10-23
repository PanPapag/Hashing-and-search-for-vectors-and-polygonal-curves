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
#include "../headers/search/lsh.h"
#include "../headers/search/brute_force.h"
#include "../headers/utils/utils.h"
#include "../headers/utils/args_utils.h"
#include "../headers/utils/report_utils.h"
#include "../headers/utils/io_utils.h"
#include "../headers/vectorization/vectorization.h"


#define T double
#define U int

using namespace std::chrono;

int main(int argc, char **argv) {
  utils::InputInfo input_info;
  utils::ExitCode status;
  double r, delta;
  int exit_code;

  /* Get arguments */
  exit_code = utils::args::ReadArguments(argc, argv, input_info, status);
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

  /* Preprocessing query file to get number of query curves */
  start = high_resolution_clock::now();
  std::cout << "\nGetting number of query curves.." << std::endl;
  exit_code = utils::io::GetDataCurves(input_info.query_file, input_info.Q, status);
  if (exit_code != utils::SUCCESS) {
    utils::report::ReportError(status);
  }
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Getting number of query curves completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;

  /* Reading query file */
  start = high_resolution_clock::now();
  std::cout << "\nReading query file.." << std::endl;
  std::vector<std::pair<T,T>> query_curves;
  std::vector<U> query_ids(input_info.Q);
  std::vector<int> query_lengths(input_info.Q);
  std::vector<int> query_offsets(input_info.Q);
  exit_code = utils::io::ReadFile<T,U>(input_info.query_file, input_info.Q,
    query_curves, query_ids, query_lengths, query_offsets, status);
  if (exit_code != utils::SUCCESS) {
    utils::report::ReportError(status);
  }
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Reading query file completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;

  /* Print input info */
  input_info.Print();

  /* Create BruteForce class object and a vector to store exact-NN results */
  start = high_resolution_clock::now();
  std::cout << "\nBuilding Brute Force.." << std::endl;
  std::vector<std::tuple<T,U,double>> bf_nn_results(input_info.Q);
  search::curves::BruteForce<T,U> bf{dataset_curves, dataset_ids,
                                     dataset_lengths, dataset_offsets};
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Building Brute Force completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;

  /* Executing Exact Nearest Neighbor using BruteForce */
  start = high_resolution_clock::now();
  std::cout << "\nExecuting Nearest Neighbor using Brute Force.." << std::endl;
  for (int i = 0; i < input_info.Q; ++i) {
    bf_nn_results[i] = bf.NearestNeighbor(query_curves, query_lengths,
                                          query_offsets, i);
  }
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Executing Nearest Neighbor using Brute Force completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;

  /* Computing delta parameter for grid */
  start = high_resolution_clock::now();
  std::cout << "\nComputing delta.." << std::endl;
  delta = utils::ComputeDelta(dataset_curves, dataset_lengths, dataset_offsets);
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Computing delta completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;

  vectorization::Grid<T,U> grid{dataset_curves, dataset_ids,
                                dataset_lengths, dataset_offsets, delta};
                                grid.Vectorize();

  /* Comptuing window parameter as k * R used by LSH and HyperCube */
  start = high_resolution_clock::now();
  std::cout << "\nComputing window parameter.." << std::endl;
  r = utils::ComputeParameterR(bf_nn_results);
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Computing window parameter completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;

  std::cout << "Delta: " << delta << std::endl;
  std::cout << "Window: " << r << std::endl;

  /* for (int i = 0; i < input_info.Q ;++i) {
    std::cout << std::get<0>(bf_nn_results[i]) << " " << std::get<1>(bf_nn_results[i]) << std::endl;
  } */
  /* for (int i = 0; i < dataset_ids.size(); ++i) {
    std::cout << "----> Id: " << dataset_ids[i] << " Length: " << dataset_lengths[i] << std::endl;
    for (int j = 0; j < dataset_lengths[i]; ++j) {
      std::cout << "(" << std::get<0>(dataset_curves[dataset_offsets[i] + j]);
      std::cout << "," << std::get<1>(dataset_curves[dataset_offsets[i] + j])
                << ")" << std::endl;
    }
  } */
  /*
  std::cout << "QUERY" << std::endl;
  for (int i = 0; i < query_ids.size(); ++i) {
    std::cout << "----> Id: " << query_ids[i] << " Length: " << query_lengths[i] << std::endl;
    for (int j = 0; j < query_lengths[i]; ++j) {
      std::cout << "(" << std::get<0>(query_curves[query_offsets[i] + j]);
      std::cout << "," << std::get<1>(query_curves[query_offsets[i] + j])
                << ")" << std::endl;
    }
  } */
  //vectorization::Projection<T,U> test {dataset_curves, dataset_offsets, dataset_lengths, dataset_ids};

  return EXIT_SUCCESS;
}
