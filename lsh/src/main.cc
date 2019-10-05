#include <iostream>

#include "../headers/utils.h"
#include "../headers/args_utils.h"

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
  return EXIT_SUCCESS;
}
