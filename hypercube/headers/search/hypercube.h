#ifndef HYPERCUBE
#define HYPERCUBE

#include <iterator>
#include <random>
#include <tuple>
#include <unordered_map>

#include "../../headers/hash/hash_function.h"
#include "../../headers/metric/metric.h"
#include "../../headers/utils/utils.h"

using namespace std::chrono;

namespace search {

  template <typename T, typename U>
  class HyperCube {
		uint32_t m;
		uint32_t M;
		double w;
		const uint8_t K;
		const uint8_t L;
		const uint16_t D;
		const uint32_t N;
		const double R;
		const std::vector<T>& feature_vector;
		const std::vector<U>& feature_vector_ids;
		std::vector<hash::AmplifiedHashFunction<T>> hash_functions;
    public:
		/** \brief class LSH constructor
		*/
		HyperCube(const uint8_t K, const uint8_t L, const uint16_t D, const uint32_t N,
			const std::vector<T>& points, const std::vector<T>& ids)
			: K(K), L(L), D(D), N(N), R(1.0), feature_vector(points), feature_vector_ids(ids) {
			w = 4 * R;
			m = (1ULL << 32) - 5;
			M = 1ULL << (32 / K);
		
			// Preprocess step
			// 1) Randomly select L amplified hash functions g1 , . . . , gL .
			for (int i = 0; i < L; ++i) {
				hash_functions.push_back(hash::AmplifiedHashFunction<T>(K,D,m,M,w));
			}
		};
	};
}

#endif
