#ifndef HASH_FUNCTION
#define HASH_FUNCTION

#include <algorithm>
#include <cmath>
#include <chrono>
#include <iostream>
#include <iterator>
#include <random>
#include <string>
#include <vector>

#include "../../headers/utils/utils.h"

namespace hash {

  template <typename T>
  class HashFunction {
    private:
      std::default_random_engine generator;
      std::uniform_real_distribution<double> distribution;
      const uint16_t D;
      const uint32_t m;
      const uint32_t M;
      const double w;
      std::vector<double> s;
      std::vector<int> a;
    public:
      /** \brief HashFunction class constructor
        This class illustrates the following hash function:
        h(x)= (a_d−1 + m*a_d−2 +···+ m^(d−1)*a_0) modM , m > max a_i
        where a_i = floor((x_i - s_i) / w)
        @par D - Space dimension
        @par m - parameter m in the hash function
        @par M - parameter M in the hash function
        @par w - window size
      */
      HashFunction(const uint16_t D, const uint32_t m, const uint32_t M,
        const double w): D(D), m(m), M(M), w(w), distribution(0,w), s(D), a(D),
        generator(std::chrono::system_clock::now().time_since_epoch().count()) {

        /* Initialize s vector of dimension D using uniform_real_distribution */
        for(size_t i = 0; i < D; ++i) {
          s[i] = distribution(generator);
        }
      };
      /**
        \brief HashFunction class default destructor
      */
      ~HashFunction() = default;
      /*
      /** \brief Hash point as follows:
        1) Compute a_i = floor((x_i - s_i) / w) for i = 0...D-1
        2) Compute h(x) = (a_d−1 + m*a_d−2 +···+ m^(d−1)*a_0) modM
      */
      uint32_t Hash(std::vector<T> &points, int offset) {
        uint32_t hash_value{};
        /* Computing a_i */
        for (size_t i = 0; i < D; ++i) {
          a[i] = floor((points[offset * D + i] - s[i]) / w);
        }
        /* Reverse vector a */
        std::reverse(a.begin(),a.end());
        /* Computing h(x) */
        for (size_t i = 0; i < D; ++i) {
          hash_value += utils::mod(a[i],M) * utils::mod_exp(m,i,M);
        }
        return hash_value % M;
      };
  };

}


#endif
