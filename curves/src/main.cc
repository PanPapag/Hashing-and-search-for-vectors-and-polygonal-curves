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
#include "../headers/search/grid_search.h"
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
  uint32_t D_vec;
  const uint8_t factor = 10;
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
  exit_code = utils::io::GetDataCurves(input_info.input_file,
                                       input_info.N, status);
  if (exit_code != utils::SUCCESS) {
    utils::report::ReportError(status);
  }
  auto stop = high_resolution_clock::now();
  duration <double> total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Getting number of dataset curves completed successfully."
            << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds"
            << std::endl;

  /*
    Read dataset and create 1D vector of pairs which stores sequentially each
    curve of lenth m_i. Also create 1D vector that stores curves' ids and
    curves' length. 1D vector of curves representation support cache efficiency
    and as a result, faster computations
  */
  start = high_resolution_clock::now();
  std::cout << "\nReading input file.." << std::endl;
  std::vector<std::pair<T,T>> dataset_curves;
  std::vector<U> dataset_curves_ids(input_info.N);
  std::vector<int> dataset_curves_lengths(input_info.N);
  std::vector<int> dataset_curves_offsets(input_info.N);
  exit_code = utils::io::ReadFile<T,U>(input_info.input_file, input_info.N,
    dataset_curves, dataset_curves_ids, dataset_curves_lengths,
    dataset_curves_offsets, status);
  if (exit_code != utils::SUCCESS) {
    utils::report::ReportError(status);
  }
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Reading input file completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds"
            << std::endl;

  /* Preprocessing query file to get number of query curves */
  start = high_resolution_clock::now();
  std::cout << "\nGetting number of query curves.." << std::endl;
  exit_code = utils::io::GetDataCurves(input_info.query_file,
                                       input_info.Q, status);
  if (exit_code != utils::SUCCESS) {
    utils::report::ReportError(status);
  }
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Getting number of query curves completed successfully."
            << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds"
            << std::endl;

  /* Reading query file */
  start = high_resolution_clock::now();
  std::cout << "\nReading query file.." << std::endl;
  std::vector<std::pair<T,T>> query_curves;
  std::vector<U> query_curves_ids(input_info.Q);
  std::vector<int> query_curves_lengths(input_info.Q);
  std::vector<int> query_curves_offsets(input_info.Q);
  exit_code = utils::io::ReadFile<T,U>(input_info.query_file, input_info.Q,
    query_curves, query_curves_ids, query_curves_lengths,
    query_curves_offsets, status);
  if (exit_code != utils::SUCCESS) {
    utils::report::ReportError(status);
  }
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Reading query file completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds"
            << std::endl;

  /* Print input info */
  input_info.Print();

  /* Create BruteForce class object and a vector to store exact-NN results */
  start = high_resolution_clock::now();
  std::cout << "\nBuilding Brute Force.." << std::endl;
  std::vector<std::tuple<T,U,double>> bf_nn_results(input_info.Q);
  search::curves::BruteForce<T,U> bf{dataset_curves, dataset_curves_ids,
                                     dataset_curves_lengths,
                                     dataset_curves_offsets};
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Building Brute Force completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds"
            << std::endl;

  /* Executing Exact Nearest Neighbor using BruteForce */
  start = high_resolution_clock::now();
  std::cout << "\nExecuting Nearest Neighbor using Brute Force.." << std::endl;
  for (int i = 0; i < input_info.Q; ++i) {
    bf_nn_results[i] = bf.NearestNeighbor(query_curves, query_curves_lengths,
                                          query_curves_offsets, i);
  }
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Executing Nearest Neighbor using Brute Force completed successfully."
            << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds"
            << std::endl;

  /* Computing delta parameter for grid */
  start = high_resolution_clock::now();
  std::cout << "\nComputing grid hyperparameters.." << std::endl;
  delta = utils::ComputeDelta(dataset_curves, dataset_curves_lengths,
                              dataset_curves_offsets);
  D_vec = 2 * *max_element(std::begin(dataset_curves_lengths),
                           std::end(dataset_curves_lengths));
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Computing grid hyperparameters completed successfully."
            << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds"
            << std::endl;

  /* Building L_grid Grids */
  start = high_resolution_clock::now();
  std::cout << "\nBuilding " << static_cast<unsigned int>(input_info.L_grid)
            << " grids.." << std::endl;
  std::vector<vectorization::Grid<T>> grids;
  for (size_t i = 0; i < input_info.L_grid; ++i) {
    grids.push_back(vectorization::Grid<T>(dataset_curves,
                                           dataset_curves_lengths,
                                           dataset_curves_offsets,
                                           input_info.N, D_vec, factor * delta));
  }
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Building " << static_cast<unsigned int>(input_info.L_grid)
            << " grids completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds"
            << std::endl;

  /* For each curve find an equivalent vector. Do this for L_grid grids */
  start = high_resolution_clock::now();
  std::cout << "\nVectorizing dataset curves using grid method.." << std::endl;
  std::vector<std::vector<double>> L_grid_dataset_vectors(input_info.L_grid);
  for (size_t i = 0; i < input_info.L_grid; ++i) {
    L_grid_dataset_vectors[i] = grids[i].Vectorize();
  }
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Vectorizing dataset curves using grid method completed."
            << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds"
            << std::endl;

  /* Comptuing window parameter as k * R used by LSH and HyperCube */
  start = high_resolution_clock::now();
  std::cout << "\nComputing window parameter.." << std::endl;
  r = utils::ComputeParameterR(bf_nn_results);
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Computing window parameter completed successfully."
            << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds"
            << std::endl;

  /* Bulding LSH structures */
  start = high_resolution_clock::now();
  std::cout << "\nBuilding LSH structures.." << std::endl;
  std::vector<search::curves::LSH<T,U>> lsh_structures;
  for (size_t i = 0; i < input_info.L_grid; ++i) {
    lsh_structures.
      push_back(search::curves::LSH<T,U>(input_info.K_vec, 1, D_vec,
                                         input_info.N, r, dataset_curves,
                                         dataset_curves_ids,
                                         dataset_curves_lengths,
                                         dataset_curves_offsets,
                                         L_grid_dataset_vectors[i]));
  }
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Building LSH structures completed successfully."
            << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds"
            << std::endl;

  /* Vectorizing query curves */
  start = high_resolution_clock::now();
  std::cout << "\nVectorizing query curves using grid method.." << std::endl;
  std::vector<std::vector<double>> L_grid_query_vectors(input_info.L_grid);
  for (size_t i = 0; i < input_info.L_grid; ++i) {
    L_grid_query_vectors[i] = grids[i].Vectorize(input_info.Q, query_curves,
                                                 query_curves_lengths,
                                                 query_curves_offsets);
  }
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Vectorizing query curves using grid method completed."
            << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds"
            << std::endl;

  /* Executing approximate Nearest Neighbor using LSH */
  start = high_resolution_clock::now();
  std::cout << "\nExecuting Nearest Neighbor using LSH.." << std::endl;
  std::vector<std::tuple<T,U,double>> approx_nn_results(input_info.Q);
  for (size_t i = 0; i < input_info.Q; ++i) {
    approx_nn_results[i] = search::curves::lsh_grid_search(input_info.L_grid,
                                                lsh_structures, query_curves,
                                                query_curves_lengths,
                                                query_curves_offsets,
                                                L_grid_query_vectors, i);
  }
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Executing Nearest Neighbor using LSH completed successfully."
            << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds"
            << std::endl;

  /* Compute Max and Average ratio lsh_nn_results / bf_nn_results */
  start = high_resolution_clock::now();
  std::cout << "\nCalculating evaluation metric.." << std::endl;
  std::tuple<double,double,int> metric_res = metric::EvaluationMetric(bf_nn_results,
                                                            approx_nn_results);
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Calculating evaluation metric completed successfully."
            << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds"
            << std::endl;
  std::cout << "\nMax Af: " << std::get<0>(metric_res) << std::endl;
  std::cout << "Average Af: " << std::get<1>(metric_res) << std::endl;
  std::cout << "Not found: " << std::get<2>(metric_res) << std::endl;

  /* Writing results to the output file */
  start = high_resolution_clock::now();
  std::cout << "\nWriting results to the output file.." << std::endl;
  exit_code = utils::io::WriteFile<T,U>(input_info.output_file,
    query_curves_ids, bf_nn_results, approx_nn_results, status);
  if (exit_code != utils::SUCCESS) {
    utils::report::ReportError(status);
  }
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Writing results to the output file completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;

  return EXIT_SUCCESS;
}
