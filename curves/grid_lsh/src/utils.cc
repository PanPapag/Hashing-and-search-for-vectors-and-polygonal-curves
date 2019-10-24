#include <iostream>

#include "../includes/utils.h"

void utils::ShowUsage(const std::string &program_name,
  const struct InputInfo &input_info) {
  std::cerr << "usage: " << program_name << " [-help] [-d INPUT FILE]"
            << " [-q QUERY FILE] [-k_vec INT] [-L_grid INT] [-o OUTPUT FILE]"
            << "\n\n"
            << "optional arguments:\n"
            << "  -help\tshow this help message and exit\n"
            << "  -d\tdefine the input file\n"
            << "  -q\tdefine the query file\n"
            << "  -k_vec\tdefine the number of LSH hash functions (default = "
                  << input_info.K_vec << ")\n"
            << "  -L_grid\tdefine the number of LSH hash tables (default = "
                  << input_info.L_grid << ")\n"
            << "  -o\tdefine the output file\n"
            << std::endl;
  exit(EXIT_FAILURE);
}

void utils::InputInfo::Print(void) {
  std::cout << std::endl;
  std::cout << "Input file: " << input_file << std::endl;
  std::cout << "Query file: " << query_file << std::endl;
  std::cout << "Output file: " << output_file << std::endl;
  std::cout << "Number of LSH hash functions: "
            << static_cast<unsigned int>(K_vec) << std::endl;
  std::cout << "Number of LSH hash tables: "
            << static_cast<unsigned int>(L_grid) << std::endl;
  std::cout << "Number of dataset curves: "
            << static_cast<unsigned int>(N) << std::endl;
  std::cout << "Number of query curves: "
            << static_cast<unsigned int>(Q) << std::endl;
}
