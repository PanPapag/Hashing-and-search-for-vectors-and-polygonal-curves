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
#include "../headers/search/hypercube.h"
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
  input_info.output_file = "../../results/vectors/small_cube";
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
    input_info.D, query_points, query_ids, status);  
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
  search::BruteForce<T,U> bf{input_info.N, input_info.D, dataset_points, dataset_ids};
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Building Brute Force completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;

  /* Executing Exact Nearest Neighbor using BruteForce */
  start = high_resolution_clock::now();
  std::cout << "\nExecuting Nearest Neighbor using Brute Force.." << std::endl;
  for (int i = 0; i < input_info.Q; ++i) {
    bf_nn_results[i] = bf.NearestNeighbor(query_points, i);
  }
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Executing Nearest Neighbor using Brute Force completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;

  /* Computing radius */
  start = high_resolution_clock::now();
  std::cout << "\nComputing radius.." << std::endl;
  double radius = utils::ComputeRadius(bf_nn_results);
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Computing radius completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;
  std::cout << "\nRadius: " << radius << std::endl;

  /* Executing Radius Nearest Neighbor using BruteForce*/
  std::vector<std::vector<std::pair<T,U>>> bf_radius_nn_results(input_info.Q);
  start = high_resolution_clock::now();
  std::cout << "\nExecuting Radius Nearest Neighbor using Brute Force.." << std::endl;
  for (int i = 0; i < input_info.Q; ++i) {
    bf_radius_nn_results[i] = bf.RadiusNearestNeighbor(query_points, i, radius);
  }
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Executing Radius Nearest Neighbor using Brute Force completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;

  /* Creating LSH class object and a vector to store approx-NN results */
  start = high_resolution_clock::now();
  std::cout << "\nBuilding HyperCube.." << std::endl;
  std::vector<std::tuple<T,U,double>> hypercube_nn_results(input_info.Q);
  search::HyperCube<T,U> cube{input_info.k, input_info.M, input_info.D,
                       input_info.N, radius, dataset_points, dataset_ids};
  //cube.Print();
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Building HyperCube completed successfully." << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;

  // /* Executing approximate Nearest Neighbor using LSH */
  // start = high_resolution_clock::now();
  // std::cout << "\nExecuting Nearest Neighbor using HyperCube.." << std::endl;
  // for (int i = 0; i < input_info.Q; ++i) {
  //   //hypercube_results[i] = hypercube.NearestNeighbor(query_points, i);
  // }
  // stop = high_resolution_clock::now();
  // total_time = duration_cast<duration<double>>(stop - start);
  // std::cout << "Executing Nearest Neighbor using LSH completed successfully." << std::endl;
  // std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;  

  return EXIT_SUCCESS;
}
