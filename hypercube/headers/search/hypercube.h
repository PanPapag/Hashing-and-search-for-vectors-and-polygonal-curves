#ifndef HYPERCUBE
#define HYPERCUBE

#include <iterator>
#include <random>
#include <tuple>
#include <unordered_map>
#include <bitset>

#include "../../headers/hash/hash_function.h"
#include "../../headers/metric/metric.h"
#include "../../headers/utils/utils.h"

using namespace std::chrono;

namespace search {

  template <typename T, typename U>
  class HyperCube {
		uint32_t m;
		uint32_t t;
		double w;
		
		const uint16_t k;
		const uint16_t M;
		const uint16_t D;
		const uint32_t N;
		const double R;

		const std::vector<T>& feature_vector;
		const std::vector<U>& feature_vector_ids;
		
		std::vector<hash::AmplifiedHashFunction<T>> g;
		std::unordered_map<uint32_t, std::bitset<1>> bucket_map;
		std::vector<std::vector<std::bitset<1>>> p;
    
		std::default_random_engine generator;
  	std::uniform_int_distribution<int> f = std::uniform_int_distribution<int>(0,1);
		
		public:
		/** \brief class HyperCube constructor
		*/
		HyperCube(const uint16_t k, const uint16_t M, const uint16_t D, const uint32_t N,
			const double r, const std::vector<T>& points, const std::vector<T>& ids)
			: k(k), M(M), D(D), N(N), R(r), feature_vector(points), feature_vector_ids(ids) {
			w = 4 * R;
			m = (1ULL << 32) - 5;
			t = 1ULL << (32 / k);
		
			// Preprocess step
			// 1) Randomly select k = logD amplified hash functions g1 , . . . , gL .
			for (int i = 0; i < k; ++i) {
				g.push_back(hash::AmplifiedHashFunction<T>(k,D,m,t,w));
			}

			for (int i = 0; i < N; ++i) {
				std::vector<std::bitset<1>> temp;
				for (int j = 0; j < k; ++j) {
					uint32_t key = g[j].Hash(feature_vector,i); 
					//project points in a cube
					FlipCoin(key);
					temp.push_back(bucket_map[key]);
				}
				p.push_back(temp);
				//std::cout << s << " ";
				//std::cout << s << " " << num << std::endl;
			}
			// for (auto& it: bucket_map) {
    	// 	std::cout << it.first << " " << it.second << std::endl;
			// }
			// if(bucket_map.size() == k*N) {
			// 	std::cout << "Oh No" << std::endl;
			// } else {
			// 	std::cout << "Bucket Size:" << bucket_map.size() << " | k*N:" << k*N << std::endl;
			// }
			// for (auto& i: p) {
			// 	for (auto& j: i) {
			// 		std::cout << j;
			// 	}
			// 	std::cout << std::endl;
			// }
		};
		
		/** \brief For each gi, 
		 * fi(gi) maps buckets to {0,1} uniformly.
		*/
		void FlipCoin(uint32_t key) {
			if (bucket_map.find(key) == bucket_map.end()) {
				bucket_map[key] = f(generator);
			}
		};

		void Print() {
			for (auto& i: p) {
				for (auto& j: i) {
					std::cout << j;
				}
				std::cout << std::endl;
			}
		}
	};
}

#endif
