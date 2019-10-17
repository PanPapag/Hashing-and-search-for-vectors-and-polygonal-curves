#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>        
#include <cstdlib>

#include "../headers/utils/utils.h"

void utils::ShowUsage(const std::string& program_name, const struct InputInfo& input_info) {
  std::cerr << "usage: " << program_name << " [-help] [-d INPUT FILE]"
            << " [-q QUERY FILE] [-k INT] [-M INT] [-probes INT] [-o OUTPUT FILE]"
            << "\n\n"
            << "optional arguments:\n"
            << "  -help\tshow this help message and exit\n"
            << "  -d\tdefine the input file\n"
            << "  -q\tdefine the query file\n"
            << "  -k\tdefine the number of reduced dimensions (default = "
                  << input_info.k << ")\n"
            << "  -M\tdefine max number of candidate points to be checked (default = "
                  << input_info.M << ")\n"
            << "  -M\tdefine max number of probes (default = "
                  << input_info.probes << ")\n"
            << "  -o\tdefine the output file\n"
            << std::endl;
  exit(EXIT_FAILURE);
}

void utils::InputInfo::Print(void) {
  std::cout << std::endl;
  std::cout << "Input file: " << input_file << std::endl;
  std::cout << "Query file: " << query_file << std::endl;
  std::cout << "Output file: " << output_file << std::endl;
  std::cout << "Number of probes: "
            << static_cast<unsigned int>(probes) << std::endl;
  std::cout << "Number of dataset points: "
            << static_cast<unsigned int>(N) << std::endl;
  std::cout << "Number of query points: "
            << static_cast<unsigned int>(Q) << std::endl;
  std::cout << "Space dimension: "
            << static_cast<unsigned int>(D) << std::endl;
  std::cout << "Number of reduced dimensions: "
            << static_cast<unsigned int>(k) << std::endl;
}

uint64_t utils::mod_exp(uint32_t base, uint16_t exp, uint32_t mod) {
  uint64_t res = 1;
  while (exp > 0) {
    if (exp % 2 == 1)
      res = (res * base) % mod;
    exp = exp >> 1;
    base = (base * base) % mod;
  }
  return res;
}

std::vector<std::string> utils::GetToggledBitStrings(std::string key) {
  /* Initialize the vector to be returned */
  std::vector<std::string> result;
  /* Toggle each char of the key string to take the one with hamming distance 1*/
  for (int i = 0; i < key.length(); ++i) {
    std::string temp = key;
    temp[i] = (key[0] == '1') ? 0 : 1;
    result.push_back(temp);
  }
  return result;
}

std::vector<size_t> utils::VectorShuffle(size_t n) {
  std::vector<size_t> idx; 
  for(size_t i=0; i<n; i++) {
    idx.push_back(i);
  }
  std::random_shuffle(idx.begin(), idx.end());
  return idx;
}
