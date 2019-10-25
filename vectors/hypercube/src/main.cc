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

#include "../../../core/hash/hash_function.h"
#include "../../../core/metric/metric.h"
#include "../../../core/search/brute_force.h"
#include "../../../core/search/hypercube.h"
#include "../../../core/utils/utils.h"

#include "../includes/args_utils.h"
#include "../includes/io_utils.h"
#include "../includes/report_utils.h"
#include "../includes/utils.h"

#define T int
#define U int

using namespace std::chrono;

int main(int argc, char **argv) {
  utils::InputInfo input_info;
  utils::ExitCode status;
  std::string input_buffer;
  double radius = 0.0;
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

  /* Preprocessing input file to get number of dataset points and their dimension */
  auto start = high_resolution_clock::now();
  std::cout << "\nGetting number of dataset points.." << std::endl;
  exit_code = utils::io::GetDataPoints(input_info.input_file,
                                       input_info.N, status);
  if (exit_code != utils::SUCCESS) {
    utils::report::ReportError(status);
  }
  auto stop = high_resolution_clock::now();
  duration <double> total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Getting number of dataset points completed successfully."
            << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds"
            << std::endl;

  start = high_resolution_clock::now();
  std::cout << "\nGetting dataset points' dimension.." << std::endl;
  exit_code = utils::io::GetPointsDim(input_info.input_file,
                                      input_info.D, status);
  if (exit_code != utils::SUCCESS) {
    utils::report::ReportError(status);
  }
  stop = high_resolution_clock::now();
  total_time = duration_cast<duration<double>>(stop - start);
  std::cout << "Getting dataset points' dimension completed successfully."
            << std::endl;
  std::cout << "Time elapsed: " << total_time.count() << " seconds"
            << std::endl;

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
  std::cout << "Time elapsed: " << total_time.count() << " seconds"
            << std::endl;

  do {
    /* Preprocessing query file */
    start = high_resolution_clock::now();
    std::cout << "\nGetting number of query points.." << std::endl;
    exit_code = utils::io::GetDataPoints(input_info.query_file,
                                         input_info.Q, status);
    if (exit_code != utils::SUCCESS) {
      utils::report::ReportError(status);
    }
    stop = high_resolution_clock::now();
    total_time = duration_cast<duration<double>>(stop - start);
    std::cout << "Getting number of query points completed successfully."
              << std::endl;
    std::cout << "Time elapsed: " << total_time.count() << " seconds"
              << std::endl;

    /* Reading query file */
    start = high_resolution_clock::now();
    std::cout << "\nReading query file.." << std::endl;
    std::vector<T> query_points(input_info.Q * input_info.D);
    std::vector<U> query_ids(input_info.Q);
    exit_code = utils::io::ReadFile<T,U>(input_info.query_file, input_info.Q,
                                         input_info.D, query_points,
                                         query_ids, status);
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
    search::vectors::BruteForce<T,U> bf{input_info.N, input_info.D,
                                        dataset_points, dataset_ids};
    stop = high_resolution_clock::now();
    total_time = duration_cast<duration<double>>(stop - start);
    std::cout << "Building Brute Force completed successfully." << std::endl;
    std::cout << "Time elapsed: " << total_time.count() << " seconds"
              << std::endl;

    /* Executing Exact Nearest Neighbor using BruteForce */
    start = high_resolution_clock::now();
    std::cout << "\nExecuting Nearest Neighbor using Brute Force.." << std::endl;
    for (size_t i = 0; i < input_info.Q; ++i) {
      bf_nn_results[i] = bf.NearestNeighbor(query_points, i);
    }
    stop = high_resolution_clock::now();
    total_time = duration_cast<duration<double>>(stop - start);
    std::cout << "Executing Nearest Neighbor using Brute Force completed successfully."
              << std::endl;
    std::cout << "Time elapsed: " << total_time.count() << " seconds"
              << std::endl;

    /* Computing window */
    start = high_resolution_clock::now();
    std::cout << "\nComputing window parameter.." << std::endl;
    double r = utils::ComputeParameterR(bf_nn_results);
    stop = high_resolution_clock::now();
    total_time = duration_cast<duration<double>>(stop - start);
    std::cout << "Computing window completed successfully." << std::endl;
    std::cout << "Time elapsed: " << total_time.count() << " seconds" << std::endl;

    /* Creating HyperCube class object and a vector to store approx-NN results */
    start = high_resolution_clock::now();
    std::vector<std::tuple<T,U,double>> cube_nn_results(input_info.Q);
    std::cout << "\nBuilding HyperCube.." << std::endl;
    std::vector<std::tuple<T,U,double>> hypercube_nn_results(input_info.Q);
    search::vectors::HyperCube<T,U> cube{input_info.k, input_info.M, input_info.D,
                                         input_info.N, input_info.probes, r,
                                         dataset_points, dataset_ids};

    stop = high_resolution_clock::now();
    total_time = duration_cast<duration<double>>(stop - start);
    std::cout << "Building HyperCube completed successfully." << std::endl;
    std::cout << "Time elapsed: " << total_time.count() << " seconds"
              << std::endl;

    /* Executing approximate Nearest Neighbor using HyperCube */
    start = high_resolution_clock::now();
    std::cout << "\nExecuting Nearest Neighbor using HyperCube.." << std::endl;
    for (size_t i = 0; i < input_info.Q; ++i) {
      cube_nn_results[i] = cube.NearestNeighbor(query_points, i);
    }
    stop = high_resolution_clock::now();
    total_time = duration_cast<duration<double>>(stop - start);
    std::cout << "Executing Nearest Neighbor using HyperCube completed successfully."
              << std::endl;
    std::cout << "Time elapsed: " << total_time.count() << " seconds"
              << std::endl;

    /* Read radius if user select to run range search */
    do {
      std::cout << "\nDo you want to run range search? (y/n)? : ";
      std::cin >> input_buffer;
      if (input_buffer != "y" && input_buffer != "n") {
        std::cout << "Wrong input! Try again." << std::endl;
      }
    } while (input_buffer != "y" && input_buffer != "n");
    /* Define vectors to store radius NN results */
    std::vector<std::vector<std::pair<T,U>>> bf_radius_nn_results(input_info.Q);
    std::vector<std::vector<std::pair<T,U>>> cube_radius_nn_results(input_info.Q);
    /* Check input */
    if (input_buffer != "n") {
      std::cout << "Provide the radius: ";
      std::cin >> input_buffer;
      try {
        radius = stoi(input_buffer);
      } catch (...) {
        return EXIT_FAILURE;
      }
      /* Executing Radius Nearest Neighbor using BruteForce*/
      start = high_resolution_clock::now();
      std::cout << "\nExecuting Radius Nearest Neighbor using Brute Force.."
                << std::endl;
      for (size_t i = 0; i < input_info.Q; ++i) {
        bf_radius_nn_results[i] = bf.RadiusNearestNeighbor(query_points, i, radius);
      }
      stop = high_resolution_clock::now();
      total_time = duration_cast<duration<double>>(stop - start);
      std::cout << "Executing Radius Nearest Neighbor using Brute Force completed successfully."
                << std::endl;
      std::cout << "Time elapsed: " << total_time.count() << " seconds"
                << std::endl;

      /* Executing Radius Nearest Neighbor using HyperCube*/
      start = high_resolution_clock::now();
      std::cout << "\nExecuting Radius Nearest Neighbor using HyperCube.."
                << std::endl;
      for (size_t i = 0; i < input_info.Q; ++i) {
        cube_radius_nn_results[i] = cube.RadiusNearestNeighbor(query_points, i, radius);
      }
      stop = high_resolution_clock::now();
      total_time = duration_cast<duration<double>>(stop - start);
      std::cout << "Executing Radius Nearest Neighbor using HyperCube completed successfully."
                << std::endl;
      std::cout << "Time elapsed: " << total_time.count() << " seconds"
                << std::endl;
    }

    /* Compute Max and Average ratio cube_nn_results / bf_nn_results */
    start = high_resolution_clock::now();
    std::cout << "\nCalculating evaluation metric.." << std::endl;
    std::tuple<double,double,int> metric_res{};
     metric_res = metric::EvaluationMetric(bf_nn_results, cube_nn_results);
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
    exit_code = utils::io::WriteFile<T,U>(input_info.output_file, bf_nn_results,
      cube_nn_results, radius, bf_radius_nn_results, status);
    if (exit_code != utils::SUCCESS) {
      utils::report::ReportError(status);
    }
    stop = high_resolution_clock::now();
    total_time = duration_cast<duration<double>>(stop - start);
    std::cout << "Writing results to the output file completed successfully."
              << std::endl;
    std::cout << "Time elapsed: " << total_time.count() << " seconds"
              << std::endl;

    /* Ask user if he want to repeat with a different query file */
    do {
      std::cout << "\nDo you want to provide a different query file (y/n)? : ";
      std::cin >> input_buffer;
      if (input_buffer != "y" && input_buffer != "n") {
        std::cout << "Wrong input! Try again." << std::endl;
      }
    } while (input_buffer != "y" && input_buffer != "n");

    if (input_buffer != "n") {
      std::string path_name;
      std::cout << "Provide the relative path for the query file: ";
      std::cin >> path_name;
      input_info.query_file = path_name;

      std::cout << "Provide the relative path for the output file: ";
      std::cin >> path_name;
      input_info.output_file = path_name;
    }
  } while (input_buffer == "y");

  return EXIT_SUCCESS;
}
