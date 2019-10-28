#include <iostream>

#include "../includes/utils.h"

void utils::ShowUsage(const std::string &program_name,
  const struct InputInfo &input_info) {
  std::cerr << "usage: " << program_name << " [--help] [-d INPUT FILE]"
            << " [-q QUERY FILE] [--k_hypercube INT] [-M INT]"
            << " [--probes INT] [-L_grid INT] [-o OUTPUT FILE]"
            << "\n\n"
            << "optional arguments:\n"
            << "  --help\tshow this help message and exit\n"
            << "  -d\tdefine the input file\n"
            << "  -q\tdefine the query file\n"
            << "  --k_hypercube\tdefine the number of reduced dimensions (default = "
                  << input_info.k_hypercube << ")\n"
            << "  -M\tdefine max number of candidate points to be checked (default = "
                  << input_info.M << ")\n"
            << "  --probes\tdefine max number of probes to be checked (default = "
                  << input_info.probes << ")\n"
            << "  -L_vec\tdefine the number of LSH hash tables (default = "
                  << input_info.L_vec << ")\n"
            << "  -o\tdefine the output file\n"
            << std::endl;
  exit(EXIT_FAILURE);
}

void utils::InputInfo::Print(void) {
  std::cout << std::endl;
  std::cout << "Input file: " << input_file << std::endl;
  std::cout << "Query file: " << query_file << std::endl;
  std::cout << "Output file: " << output_file << std::endl;
  std::cout << "Number of dataset points: "
            << static_cast<unsigned int>(N) << std::endl;
  std::cout << "Number of query points: "
            << static_cast<unsigned int>(Q) << std::endl;
  std::cout << "Number of reduced dimensions: "
            << static_cast<unsigned int>(k_hypercube) << std::endl;
  std::cout << "Number of candidate probes to be checked: "
            << static_cast<unsigned int>(probes) << std::endl;
  std::cout << "Number of candidate points to be checked: "
            << static_cast<unsigned int>(M) << std::endl;
  std::cout << "Number of HyperCube structures: "
            << static_cast<unsigned int>(L_vec) << std::endl;
}
