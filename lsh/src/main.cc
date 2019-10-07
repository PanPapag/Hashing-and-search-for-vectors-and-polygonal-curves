#include <iostream>
#include <string>

#include "../headers/utils.h"
#include "../headers/args_utils.h"
#include "../headers/xvector.h"
#include "../headers/hash_function.h"

int main(int argc, char **argv) {
  utils::InputInfo input_info;
  //utils::ShowUsage(argv[0], input_info);
  /* int exit_code = utils::args::ScanArguments(input_info);
   if (exit_code == utils::SUCCESS) {
    std:: cout << "good" << std::endl;
  } else if (exit_code == utils::INVALID_L) {
    std:: cout << "L" << std::endl;
  } else if (exit_code == utils::INVALID_k) {
    std:: cout << "k" << std::endl;
  }
  input_info.print(); */
  /* int exit_code = utils::args::ReadArguments(argc, argv, input_info);
  if (exit_code == utils::SUCCESS) {
   std:: cout << "good" << std::endl;
  } else if (exit_code == utils::INVALID_L) {
   std:: cout << "L" << std::endl;
  } else if (exit_code == utils::INVALID_k) {
   std:: cout << "k" << std::endl;
  }
  input_info.print(); */

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
