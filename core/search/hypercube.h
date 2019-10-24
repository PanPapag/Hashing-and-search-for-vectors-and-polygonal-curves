#ifndef HYPERCUBE
#define HYPERCUBE

#include <iterator>
#include <random>
#include <tuple>
#include <unordered_map>
#include <bitset>

#include "../../core/hash/hash_function.h"
#include "../../core/metric/metric.h"
#include "../../core/utils/utils.h"

using namespace std::chrono;

namespace search {
  namespace vectors {
    /**
      \brief HyperCube class for vectors
    */
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
    		*/
    		HyperCube(const uint16_t k, const uint16_t M, const uint16_t D, const uint32_t N,
    			const uint8_t pr, const double r, const std::vector<T>& points, const std::vector<T>& ids)
    			: k(k), M(M), D(D), N(N), probes(pr), R(r), feature_vector(points), feature_vector_ids(ids) {
    			w = 5 * R;
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

        /**
          \brief class HyperCube default destructor
        */
        ~HyperCube() = default;
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
          std::vector<std::string> vertices = GetToggledBitStrings(key);
          size_t num_vertices = vertices.size();
          std::vector<size_t> idx = VectorShuffle(num_vertices);
          size_t max_vertices = (num_vertices < probes) ? num_vertices : probes;

          // For each key map to its bucket and search
          for (size_t i = 0; i < max_vertices; ++i) {
            const std::string key = vertices[idx[i]];
            //Get a specific vertex
            std::vector<int>& vertex = p[key];
            size_t num_points = vertex.size();
            // Choose randomly M points from vertex
            std::vector<size_t> points = VectorShuffle(num_points);
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
          std::vector<std::string> vertices = GetToggledBitStrings(key);
          size_t num_vertices = vertices.size();
          std::vector<size_t> idx = VectorShuffle(num_vertices);
          size_t max_vertices = (num_vertices < probes) ? num_vertices : probes;

          // For each key map to its bucket and search
          for (size_t i = 0; i < max_vertices; ++i) {
            const std::string key = vertices[idx[i]];
            //Get a specific vertex
            std::vector<int>& vertex = p[key];
            size_t num_points = vertex.size();
            // Choose randomly M points from vertex
            std::vector<size_t> points = VectorShuffle(num_points);
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
    		/** \brief For each gi,
    		 - fi(gi) maps buckets to {0,1} uniformly.
    		*/
    		void FlipCoin(uint32_t key) {
    			if (bucket_map.find(key) == bucket_map.end()) {
    				bucket_map[key] = f(generator);
    			}
    		};
        /**
          \brief Given a string return all strings with hamming distance 1
        */
        std::vector<std::string> GetToggledBitStrings(const std::string& key) {
          /* Initialize the vector to be returned */
          std::vector<std::string> result;
          /* Toggle each char of the key string to take the one
            with hamming distance 1 */
          for (size_t i = 0; i < key.length(); ++i) {
            std::string temp = key;
            temp[i] = (key[0] == '1') ? 0 : 1;
            result.push_back(temp);
          }
          return result;
        };
        /**
          \brief Given an input n create a vector with number from 1 to n in
            random order
        */
        std::vector<size_t> VectorShuffle(const size_t n) {
          std::vector<size_t> idx;
          for (size_t i = 0; i < n; ++i) {
            idx.push_back(i);
          }
          std::random_shuffle(idx.begin(), idx.end());
          return idx;
        };
    };
  }
}

#endif
