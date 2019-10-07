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

using namespace std::chrono;

int main(int argc, char **argv) {
  utils::InputInfo input_info;
  //utils::ShowUsage(argv[0], input_info);
  //utils::report::ReportError(utils::QUERY_ERROR); 
  /* int exit_code = utils::args::ScanArguments(input_info);
   if (exit_code == utils::SUCCESS) {
    std:: cout << "good" << std::endl;
  } else if (exit_code == utils::INVALID_L) {
    std:: cout << "L" << std::endl;
  } else if (exit_code == utils::INVALID_k) {
    std:: cout << "k" << std::endl;
  }
  input_info.Print(); */
  /* int exit_code = utils::args::ReadArguments(argc, argv, input_info);
  if (exit_code == utils::SUCCESS) {
   std:: cout << "good" << std::endl;
  } else if (exit_code == utils::INVALID_L) {
   std:: cout << "L" << std::endl;
  } else if (exit_code == utils::INVALID_k) {
   std:: cout << "k" << std::endl;
  }
  input_info.Print(); */
  std::vector<int> temp;
  const char *fn = "../../datasets/vectors/search_test";
  // Get starting timepoint
  auto start = high_resolution_clock::now();
  int exit_code = utils::io::ReadDatasetFile(fn,' ',temp);
  // Get ending timepoint
  auto stop = high_resolution_clock::now();

  // Get duration. Substart timepoints to
  // get durarion. To cast it to proper unit
  // use duration cast method
  auto duration = duration_cast<microseconds>(stop - start);

  std::cout << "Time taken by function: "
       << duration.count() << " microseconds" << std::endl;

  std::vector<double> vect;
  vect.push_back(2.0);
  vect.push_back(3.0);

  xVector<double> obj = xVector<double>("lala",vect);
  const double k = 0.35;
  obj.PrintVector();
  vect.push_back(4.0);
  obj.PrintVector();
  HashFunction<double> hi = HashFunction<double>(k,obj); 

  return EXIT_SUCCESS;
}
