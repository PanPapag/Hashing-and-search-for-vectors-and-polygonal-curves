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
    private:
      uint32_t m;
      uint32_t t;
      double w;

      const uint16_t k;
      const uint16_t M;
      const uint16_t D;
      const uint32_t N;
      const uint8_t probes;
      const double R;

      const std::vector<T>& feature_vector;
      const std::vector<U>& feature_vector_ids;

      std::vector<hash::AmplifiedHashFunction<T>> g;
      std::unordered_map<uint32_t, std::bitset<1>> bucket_map;

      std::unordered_map<std::string, std::vector<int>> p;
      std::default_random_engine generator;
      std::uniform_int_distribution<int> f = std::uniform_int_distribution<int>(0,1);

    public:
      /**
        \brief class HyperCube constructor
<<<<<<< HEAD
      */
      HyperCube(const uint16_t k, const uint16_t M, const uint16_t D, const uint32_t N,
        const uint8_t pr, const double r, const std::vector<T>& points, const std::vector<T>& ids)
        : k(k), M(M), D(D), N(N), probes(pr), R(r), feature_vector(points), feature_vector_ids(ids) {
        w = 4 * R;
        m = (1ULL << 32) - 5;
        t = 1ULL << (32 / k);

        // Preprocess step
        // 1) Randomly select k = logD amplified hash functions g1 , . . . , gL .
        for (int i = 0; i < k; ++i) {
          g.push_back(hash::AmplifiedHashFunction<T>(k,D,m,t,w));
        }

        for (int i = 0; i < N; ++i) {
          std::string str;              //TODO(Maria): size
          for (int j = 0; j < k; ++j) {
            uint32_t key = g[j].Hash(feature_vector,i);
            //project points in a cube
            FlipCoin(key);
            str += bucket_map[key].to_string();
          }
          p[str].push_back(i);
        }
      };
=======
  		*/
  		HyperCube(const uint16_t k, const uint16_t M, const uint16_t D, const uint32_t N,
  			const uint8_t pr, const double r, const std::vector<T>& points, const std::vector<T>& ids)
  			: k(k), M(M), D(D), N(N), probes(pr), R(r), feature_vector(points), feature_vector_ids(ids) {
  			w = 10 * R;
  			m = (1ULL << 32) - 5;
  			t = 1ULL << (32 / k);

  			// Preprocess step
  			// 1) Randomly select k = logD amplified hash functions g1 , . . . , gL .
  			for (int i = 0; i < k; ++i) {
  				g.push_back(hash::AmplifiedHashFunction<T>(k,D,m,t,w));
  			}

  			for (int i = 0; i < N; ++i) {
  				std::string str;
  				for (int j = 0; j < k; ++j) {
  					uint32_t key = g[j].Hash(feature_vector,i);
  					//project points in a cube
  					FlipCoin(key);
  					str += bucket_map[key].to_string();
  				}
  				p[str].push_back(i);
  			}
  		};
>>>>>>> 41a80c60464e1f5499183a674d0e4fd600a09ef5

      /**
        \brief class HyperCube default destructor
      */
      ~HyperCube() = default;

<<<<<<< HEAD
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
          std::cout << i.first << " : ";
          for (auto& j: i.second) {
            std::cout << j << " ";
          }
          std::cout << std::endl;
        }
      };

=======
  		/** \brief For each gi,
  		 * fi(gi) maps buckets to {0,1} uniformly.
  		*/
  		void FlipCoin(uint32_t key) {
  			if (bucket_map.find(key) == bucket_map.end()) {
  				bucket_map[key] = f(generator);
  			}
  		};
>>>>>>> 41a80c60464e1f5499183a674d0e4fd600a09ef5
      /** \brief Executes approximate Nearest tNeighbor
        @par const std::vector<T>& query_points - Pass by reference query points
        @par const int offset - Offset to get correspodent point
      */
      std::tuple<T,U,double> NearestNeighbor(const std::vector<T>& query_points,
        const int offset) {

        auto start = high_resolution_clock::now();
        T min_dist = std::numeric_limits<T>::max();
        U min_id{};
        std::string key;
        // Get value from g function
        for (int i = 0; i < k; ++i) {
          uint32_t val = g[i].Hash(query_points,offset);
          FlipCoin(val);
          key += bucket_map[val].to_string();
        }
        //Checking for neighbor in same vertex
        std::vector<int>& vertex = p[key];
        for (auto const& fv_offset: vertex) {
          T dist = metric::ManhattanDistance<T>(
            std::next(feature_vector.begin(), fv_offset * D),
            std::next(query_points.begin(), offset * D),
            std::next(query_points.begin(), offset * D + D)
          );
          if (dist < min_dist) {
            min_dist = dist;
            min_id = feature_vector_ids[fv_offset];
          }
        }

        // Get "probes" random vertices with hamming distance 1
        std::vector<std::string> vertices = utils::GetToggledBitStrings(key);
        size_t num_vertices = vertices.size();
        std::vector<size_t> idx = utils::VectorShuffle(num_vertices);
        size_t max_vertices = (num_vertices < probes) ? num_vertices : probes;

        // For each key map to its bucket and search
        for (size_t i = 0; i < max_vertices; ++i) {
          const std::string key = vertices[idx[i]];
          //Get a specific vertex
          std::vector<int>& vertex = p[key];
          size_t num_points = vertex.size();
          // Choose randomly M points from vertex
          std::vector<size_t> points = utils::VectorShuffle(num_points);
          size_t max_points = (num_points < M) ? num_points : M;
          // Calculate manhattan distance between those points and queries
          for (size_t j = 0; j < max_points; ++j) {
            int fv_offset = vertex[points[j]];
            T dist = metric::ManhattanDistance<T>(
              std::next(feature_vector.begin(), fv_offset * D),
              std::next(query_points.begin(), offset * D),
              std::next(query_points.begin(), offset * D + D)
            );
            if (dist < min_dist) {
              min_dist = dist;
              min_id = feature_vector_ids[fv_offset];
            }
          }
        }

        auto stop = high_resolution_clock::now();
        duration <double> total_time = duration_cast<duration<double>>(stop - start);

        // Return result as a tuple of min_dist, min_id and total_time
        return std::make_tuple(min_dist,min_id,total_time.count());
      };

      /** \brief Executes approximate Radius Nearest tNeighbor
        @par const std::vector<T>& query_points - Pass by reference query points
        @par const int offset - Offset to get correspodent point
      */
      std::vector<std::pair<T,U>> RadiusNearestNeighbor(const std::vector<T>& query_points,
        const int offset) {

        std::vector<std::pair<T,U>> result;
        auto start = high_resolution_clock::now();
        T min_dist = std::numeric_limits<T>::max();
        U min_id{};
        std::string key;
        // Get value from g function
        for (int i = 0; i < k; ++i) {
          uint32_t val = g[i].Hash(query_points,offset);
          FlipCoin(val);
          key += bucket_map[val].to_string();
        }
        //Checking for neighbor in same vertex
        std::vector<int>& vertex = p[key];
        for (auto const& fv_offset: vertex) {
          T dist = metric::ManhattanDistance<T>(
            std::next(feature_vector.begin(), fv_offset * D),
            std::next(query_points.begin(), offset * D),
            std::next(query_points.begin(), offset * D + D)
          );
          if (dist < min_dist) {
            min_dist = dist;
            min_id = feature_vector_ids[fv_offset];
          }
          if (dist <= R) {
            result.push_back(std::make_pair(dist,feature_vector_ids[fv_offset]));
          }
        }

        // Get "probes" random vertices with hamming distance 1
        std::vector<std::string> vertices = utils::GetToggledBitStrings(key);
        size_t num_vertices = vertices.size();
        std::vector<size_t> idx = utils::VectorShuffle(num_vertices);
        size_t max_vertices = (num_vertices < probes) ? num_vertices : probes;

        // For each key map to its bucket and search
        for (size_t i = 0; i < max_vertices; ++i) {
          const std::string key = vertices[idx[i]];
          //Get a specific vertex
          std::vector<int>& vertex = p[key];
          size_t num_points = vertex.size();
          // Choose randomly M points from vertex
          std::vector<size_t> points = utils::VectorShuffle(num_points);
          size_t max_points = (num_points < M) ? num_points : M;
          // Calculate manhattan distance between those points and queries
          for (size_t j = 0; j < max_points; ++j) {
            int fv_offset = vertex[points[j]];
            T dist = metric::ManhattanDistance<T>(
              std::next(feature_vector.begin(), fv_offset * D),
              std::next(query_points.begin(), offset * D),
              std::next(query_points.begin(), offset * D + D)
            );
            if (dist < min_dist) {
              min_dist = dist;
              min_id = feature_vector_ids[fv_offset];
            }
            if (dist <= R) {
              result.push_back(std::make_pair(dist,feature_vector_ids[fv_offset]));
            }
          }
        }

        auto stop = high_resolution_clock::now();
        duration <double> total_time = duration_cast<duration<double>>(stop - start);

        // Return result as a tuple of min_dist, min_id and total_time
        return result;
      };
  };
}

#endif
