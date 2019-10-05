#include <iostream>
#include <string>

#include "../headers/utils.h"
#include "../headers/args_utils.h"

int utils::args::ScanArguments(struct InputInfo &input_info) {
  std::string input_buffer;

  std::cout << "Provide the relative path for the input file: ";
  std::cin >> input_buffer;
  input_info.input_file = input_buffer;

  std::cout << "Provide the relative path for the query file: ";
  std::cin >> input_buffer;
  input_info.query_file = input_buffer;

  std::cout << "Provide the relative path for the output file: ";
  std::cin >> input_buffer;
  input_info.output_file = input_buffer;

  do {
    std::cout << "Do you want to provide hyperparameters for LSH (y/n)? : ";
    std::cin >> input_buffer;
    if (input_buffer != "y" && input_buffer != "n") {
      std::cout << "Wrong input! Try again." << std::endl;
    }
  } while (input_buffer != "y" && input_buffer != "n");

  if (input_buffer != "n") {
    std::cout << "Provide the number of LSH hash functions: ";
    std::cin >> input_buffer;
    try {
      input_info.k = stoi(input_buffer);
    } catch (...) {
       return INVALID_k;;
    }

    std::cout << "Provide the number of LSH hash tables: ";
    std::cin >> input_buffer;
    try {
      input_info.L = stoi(input_buffer);
    } catch (...) {
       return INVALID_L;
    }
  }

  return SUCCESS;
}
