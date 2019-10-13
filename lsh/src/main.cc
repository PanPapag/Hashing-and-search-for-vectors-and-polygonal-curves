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
#include "../headers/hash/hash_table.h"
#include "../headers/metric/metric.h"
#include "../headers/utils/utils.h"
#include "../headers/utils/args_utils.h"
#include "../headers/utils/report_utils.h"
#include "../headers/utils/io_utils.h"
#include "../headers/search/lsh.h"
#include "../headers/search/brute_force.h"

#define T int
#define U int

using namespace std::chrono;

int main(int argc, char **argv) {
  utils::InputInfo input_info;
  utils::ExitCode status;
  int exit_code;

  input_info.input_file = "../../datasets/vectors/input_small_id";
  input_info.query_file = "../../datasets/vectors/query_small_id";
  input_info.R = 2500.00;
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
  duration <double> total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Getting number of dataset points completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;

  start = high_resolution_clock::now();
  std::cout << "\nGetting dataset points' dimension.." << std::endl;
  exit_code = utils::io::GetPointsDim(input_info.input_file,input_info.D, status);
  if (exit_code != utils::SUCCESS) {
    utils::report::ReportError(status);
  }
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Getting dataset points' dimension completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;

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
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Reading input file completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;

  /* Preprocessing query file */
  start = high_resolution_clock::now();
  std::cout << "\nGetting number of query points.." << std::endl;
  exit_code = utils::io::GetDataPoints(input_info.query_file, input_info.Q, status);
  if (exit_code != utils::SUCCESS) {
    utils::report::ReportError(status);
  }
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Getting number of query points completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;

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
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Reading query file completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;

  /* Print all input info */
  input_info.Print();

  /* Create BruteForce class object and a vector to store exact-NN results */
  start = high_resolution_clock::now();
  std::cout << "\nBuilding Brute Force.." << std::endl;
  std::vector<std::pair<T,U>> bf_nn_results(input_info.Q);
  search::BruteForce<T,U> bf{input_info.N, input_info.D, input_info.R,
                             dataset_points, dataset_ids};
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Building Brute Force completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;

  /* Executing Exact Nearest Neighbor */
  std::vector<std::vector<std::pair<T,U>>> bf_radius_nn_results(input_info.Q);
  start = high_resolution_clock::now();
  std::cout << "\nExecuting Nearest Neighbor using Brute Force.." << std::endl;
  for (int i = 0; i < input_info.Q; ++i) {
    bf_nn_results[i] = bf.NearestNeighbor(query_points, i);
  }
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Executing Nearest Neighbor using Brute Force completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;

  /* Executing Radius Nearest Neighbor */
  start = high_resolution_clock::now();
  std::cout << "\nExecuting Radius Nearest Neighbor using Brute Force.." << std::endl;
  for (int i = 0; i < input_info.Q; ++i) {
    bf_radius_nn_results[i] = bf.RadiusNearestNeighbor(query_points, i);
  }
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Executing Radius Nearest Neighbor using Brute Force completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;

  /* Algorithm to compute w (window) */
  // Pick at random N / 1000 vectors from the input dataset
  std::vector<U> random_dataset_ids(dataset_ids);
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  shuffle(random_dataset_ids.begin(), random_dataset_ids.end(),
          std::default_random_engine(seed));

  std::vector<double> avg_cord(10);
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 128; ++j) {
      avg_cord[i] += dataset_points[random_dataset_ids[i] * 128 + i];
    }
    avg_cord[i] /= 128;
  }
  double w = 0.0;
  for (int i = 0; i < 10; ++i) {
    w += avg_cord[i];
  }
  w /= double(10);
  //std::cout << w << std::endl;
  // HashFunction example
  hash::AmplifiedHashFunction<T> g(input_info.K, 128, (1ULL << 32) - 5, 256, w);
  int i = 0;
  //for(int i = 0; i < input_info.N; ++i) {
      std::cout << g.Hash(dataset_points,i) << std::endl;
      std::cout << g.Hash(dataset_points,i)  << std::endl;

  //}


  //uint64_t largeword = ((uint16_t) 10 << 32) + ((uint16_t) 2 << 16) + ((uint16_t) 3);
  //LSH_ <int> *HashTables = new LSH_ <int>(input_info);

  // print bf nn
  /*for (int i = 0; i < input_info.Q; ++i) {
    std::cout << "Query: " << i << " -- ";
    std::cout << "Distance: " << std::get<0>(bf_nn_results[i]) << " - "
              << "Id: " << std::get<1>(bf_nn_results[i]) << std::endl;
  } */
  // print bf radius nn
  /*
  for (int i = 0; i < input_info.Q; ++i) {
    std::cout << "---------- Query " << i << " ---------- " << std::endl;
    for (int j = 0; j < bf_radius_nn_results[i].size(); ++j) {
      std::cout << "Distance: " << std::get<0>(bf_radius_nn_results[i][j])
                << " - " << "Id: " << std::get<1>(bf_radius_nn_results[i][j])
                << std::endl;
    }
    std::cout << std::endl;
  } */
  //print dataset
  /*start = high_resolution_clock::now();
  for (int i = 0; i < input_info.Q; ++i)  {
    std::cout << query_ids[i] << " ";
    for (int j = 0; j < input_info.D; ++j)
      std::cout << query_points[i * input_info.D + j] << " ";
    std::cout << std::endl;
  }
  stop = high_resolution_clock::now();
  duration = duration_cast<microseconds>(stop - start);
  std::cout << "Time elapsed: " << duration.count() << " ms" << std::endl; */

  return EXIT_SUCCESS;
}
