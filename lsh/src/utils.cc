#include <iostream>

#include "../headers/utils.h"
#include "../headers/constants.h"

inline void utils::ShowUsage(std::string program_name) {
  std::cerr << "usage: " << program_name << " [-help] [-d INPUT FILE] [-q QUERY FILE]"
            << " [-k INT] [-L INT] [-o OUTPUT FILE]"
            << "\n\n"
            << "optional arguments:\n"
            << "  -help\tshow this help message and exit\n"
            << "  -d\tdefine the input file\n"
            << "  -q\tdefine the query file\n"
            << "  -k\tdefine the number of LSH hash functions (default value = "
                  << constants::k << ")\n"
            << "  -L\tdefine the number of LSH hash tables (default value = "
                  << constants::L << ")\n"
            << "  -o\tdefine the output file\n"
            << std::endl;
  exit(EXIT_FAILURE);
}
