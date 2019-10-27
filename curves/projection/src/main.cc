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
#include "../../../core/search/lsh.h"
#include "../../../core/utils/utils.h"

#include "../../core/vectorization/vectorization.h"
#include "../../core/search/projection_search.h"

#include "../includes/args_utils.h"
#include "../includes/io_utils.h"
#include "../includes/report_utils.h"
#include "../includes/utils.h"

#define T double
#define U int

using namespace std::chrono;

int main(int argc, char **argv) {
  utils::InputInfo input_info;
  utils::ExitCode status;
  uint32_t D_vec;
  int K;
  std::string input_buffer;
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
  do {
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

    /* Create Random Projection class object and a vector to store exact-NN results */
    start = high_resolution_clock::now();
    std::cout << "\nBuilding Random Projection.." << std::endl;
    std::cout << float(input_info.e) << std::endl;
    K = 2 * (-1) * log2(input_info.e) / (input_info.e*input_info.e);
    std::vector<std::tuple<T,U,double>> rp_nn_results(input_info.Q);
    vectorization::Projection<T,U> rp{dataset_curves, dataset_curves_offsets,
                                      dataset_curves_lengths, dataset_curves_ids, 
                                      input_info.N, K};
    stop = high_resolution_clock::now();
    total_time = duration_cast<duration<double>>(stop - start);
    std::cout << "Building Random Projection completed successfully." << std::endl;
    std::cout << "Time elapsed: " << total_time.count() << " seconds"
              << std::endl;

    start = high_resolution_clock::now();
    std::cout << "\n\nVectorizing dataset curves using Random Projection method.." << std::endl;
    rp.Vectorize();
    stop = high_resolution_clock::now();
    total_time = duration_cast<duration<double>>(stop - start);
    std::cout << "Vectorizing dataset curves using Random Projection method completed." << std::endl;
    std::cout << "Time elapsed: " << total_time.count() << " seconds"
              << std::endl;

    start = high_resolution_clock::now();
    std::cout << "\n\nVectorizing query curves using Random Projection method.." << std::endl;
    rp.Vectorize(input_info.Q, query_curves, query_curves_lengths, query_curves_offsets, query_curves_ids);
    stop = high_resolution_clock::now();
    total_time = duration_cast<duration<double>>(stop - start);
    std::cout << "Vectorizing dataset curves using Random Projection method completed." << std::endl;
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

    std::unordered_map<std::tuple<int,int,int>,
        std::vector<double>> vectors = rp.GetVectors();
    const std::unordered_map<std::tuple<int,int,int>,
        std::vector<int>>& vectors_length = rp.GetVectorsLengths();
    const std::unordered_map<std::tuple<int,int,int>,
        std::vector<int>>& vectors_offsets = rp.GetVectorsOffsets();
    const std::unordered_map<std::tuple<int,int,int>,
        std::vector<U>>& vectors_ids = rp.GetVectorsIds();

    std::unordered_map<U,
        std::vector<double>> qvectors = rp.qGetVectors();
    const std::unordered_map<U,
        std::vector<int>>& qvectors_length = rp.qGetVectorsLengths();
    const std::unordered_map<U,
        std::vector<int>>& qvectors_offsets = rp.qGetVectorsOffsets();
    const std::unordered_map<U,
        std::vector<U>>& qvectors_ids = rp.qGetVectorsIds();

    /* Bulding LSH structures */
    start = high_resolution_clock::now();
    std::cout << "\nBuilding LSH structures.." << std::endl;
    std::unordered_map<int,std::vector<search::curves::LSH<T,U>>> lsh_structures;
    for(auto& bucket:vectors) {
      std::tuple<int,int,int> key = std::make_tuple(std::get<0>(bucket.first),
                                                    std::get<1>(bucket.first),
                                                    std::get<2>(bucket.first));
      
      lsh_structures[std::get<1>(bucket.first)]
        .push_back(search::curves::LSH<T,U>(input_info.K_vec, 1, K,
                                            bucket.second.size(), r, dataset_curves,
                                            vectors_ids.at(key),
                                            vectors_length.at(key),
                                            vectors_offsets.at(key),
                                            bucket.second));
    }

    /* Executing approximate Nearest Neighbor using LSH */
    start = high_resolution_clock::now();
    std::cout << "\nExecuting Nearest Neighbor using LSH.." << std::endl;
    std::vector<std::tuple<T,U,double>> approx_nn_results(input_info.Q);
    for (size_t i = 0; i < input_info.Q; ++i) {
      U id = query_curves_ids[i];
      approx_nn_results[i] = search::curves::projection_search(vectors.size(),
                                                  lsh_structures, 
                                                  query_curves,
                                                  qvectors_length,
                                                  qvectors_offsets,
                                                  qvectors, i, 10, id);
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
    std::tuple<double,double,int> metric_res{};
    metric_res = metric::EvaluationMetric(bf_nn_results, approx_nn_results);
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