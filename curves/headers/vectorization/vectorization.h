#ifndef VECTORIZATION
#define VECTORIZATION

#include <cmath>
#include <iostream>
#include <tuple>
#include <vector>
#include <random>
#include <set>

namespace vectorization {

  template <typename T>
  class Grid {
    private:
      uint32_t N;
      uint32_t D;
      const std::vector<std::pair<T,T>>& input_curves;
      const std::vector<int>& input_curves_lengths;
      const std::vector<int>& input_curves_offsets;
      const double delta;
      std::pair<double,double> t;
      std::default_random_engine generator;
      std::uniform_real_distribution<double> distribution;
    public:
      /** \brief Grid class constructor
        Initializing private members
      */
      Grid(const std::vector<std::pair<T,T>>& curves,
        const std::vector<int>& lengths, const std::vector<int>& offsets,
        const uint32_t N, const uint32_t D, double delta) :
          N(N), D(D), delta(delta), distribution(0,delta),
          generator(std::chrono::system_clock::now().time_since_epoch().count()),
          input_curves(curves), input_curves_lengths(lengths),
          input_curves_offsets(offsets) {
          // t is selected uniformly between 0 - delta
          t.first = distribution(generator);
          t.second = distribution(generator);
      }
      /**
        \brief class Grid default constructor
      */
      ~Grid() = default;
      /** \brief Given the input curves transform each one to an equivalent
        vector. Each vector is computed as follows:
        1) Gδ = {(a1δ,...,adδ)|∀ai ∈ Z}.
        2) Define family of shifted grids parameterized by t ∈ Rd :
            Gδt ={p+t|p∈Gδ}.
        3) Choose real vector t uniformly ∈ [0,delta)^d.
            For curve P = p1, . . . , pm, hash-function hδt (P) is:
              - foreach pi set pi′ = argminq∈Gδt||pi−q||∈Gδt(snap),
              - removeconsecutiveduplicatesinp1′,...,pm′
        4) Then hδt (P) = the resulting polygonal curve
      */
      std::vector<double> Vectorize(void) {
        /**
          Vectorize each input curve and store the corresponding vector to
          an 1D array. Each vector is of dimension D (max curve length) and
          we have in total_time N curves, and so N vectors.
        */
        std::vector<double> result(D * N);
        /* Iterave over each curve to compute its vector */
        for (size_t i = 0; i < N; ++i) {
          size_t idx = 0;
          for (size_t j = 0; j < input_curves_lengths[i]; ++j) {
            double x_1 = std::get<0>(input_curves[input_curves_offsets[i] + j]);
            double t_1 = std::get<0>(t);
            double x_2 = std::get<1>(input_curves[input_curves_offsets[i] + j]);
            double t_2 = std::get<1>(t);
            double a_1 = round((x_1 - t_1) / delta);
            double a_2 = round((x_2 - t_2) / delta);
            double s_1 = a_1 * delta + t_1;
            double s_2 = a_1 * delta + t_1;
            result[i * D + (idx++)] = s_1;
            result[i * D + (idx++)] = s_2;
          }
          // remove duplicates
          std::vector<double>::iterator ip;
          ip = std::unique(result.begin() + i * D, result.begin() + i * D + idx);
          result.resize(std::distance(result.begin() + i * D, ip));
          // Fill with pading coordinates to have equal length vectors
          size_t start = std::distance(result.begin() + i * D, ip);
          for (size_t j = start; j < D; ++j) {
            result[i * D + j] = std::numeric_limits<T>::max();
          }
        }
        return result;
      }
      /**
        \brief Given the query curves perform vectorization as above
      */
      std::vector<double> Vectorize(const int Q,
        const std::vector<std::pair<T,T>>& query_curves,
        const std::vector<int>& query_curves_lengths,
        const std::vector<int>& query_curves_offsets) {
        /**
          Vectorize each query curve and store the corresponding vector to
          an 1D array. Each vector is of dimension D (max curve length) and
          we have in total_time Q query curves, and so Q vectors.
        */
        std::vector<double> result(D * Q);
        /* Iterave over each curve to compute its vector */
        for (size_t i = 0; i < Q; ++i) {
          size_t idx = 0;
          for (size_t j = 0; j < query_curves_lengths[i]; ++j) {
            double x_1 = std::get<0>(query_curves[query_curves_offsets[i] + j]);
            double t_1 = std::get<0>(t);
            double x_2 = std::get<1>(query_curves[query_curves_offsets[i] + j]);
            double t_2 = std::get<1>(t);
            double a_1 = round((x_1 - t_1) / delta);
            double a_2 = round((x_2 - t_2) / delta);
            double s_1 = a_1 * delta + t_1;
            double s_2 = a_1 * delta + t_1;
            result[i * D + (idx++)] = s_1;
            result[i * D + (idx++)] = s_2;
          }
          // remove duplicates
          std::vector<double>::iterator ip;
          ip = std::unique(result.begin() + i * D, result.begin() + i * D + idx);
          result.resize(std::distance(result.begin() + i * D, ip));
          // Fill with pading coordinates to have equal length vectors
          size_t start = std::distance(result.begin() + i * D, ip);
          // Fill with pading coordinates to have eqaal length vectors
          for (size_t j = start; j < D; ++j) {
            result[i * D + j] = std::numeric_limits<T>::max();
          }
        }
        return result;
      }
  };

  template <typename T, typename U>
  class Projection  {
    private:
      int d;
      uint32_t M;
      float eps;
      int K;
      std::vector<double> G;
      std::default_random_engine generator;
      std::normal_distribution<double> distr = std::normal_distribution<double> {0,1};
    public:
      /* \brief
          Just a constructor
      */
      Projection(std::vector<std::pair<T,T>>& dataset_curves, std::vector<int>& dataset_offsets,
                std::vector<int>& dataset_lengths, std::vector<U>& dataset_ids) : eps(0.5), d(2) {
        // Get max length from all curves and
        // store all relevant traversals at an M * M array
        //M = *max_element(std::begin(dataset_lengths), std::end(dataset_lengths));
        M = 10;
        K = d * (-1) * log(eps) / (eps*eps);

        // Computing relevant traversals
        // only for upper diagonal cells
        // to reduce memory space
        size_t size = M*M;
        std::vector<std::vector<std::vector<std::pair<T,T>>>> relevant_traversals (size);
        int count = 0;
        for(size_t i=0; i<M; i++) {
          for(size_t j=0; j<M; j++) {
            if(abs(i-j < 4)) { //change this
              relevant_traversals[i*M+j] = RelevantTraversals(i,j);
              count+=relevant_traversals[i*M+j].size();
            }
          }
        }

        //Generating G matrix with 
        //random values ~N(0,1)
        std::cout << count << std::endl;
        size_t size_G = K*d;
        for(size_t i=0; i<size_G; i++) {
          G[i] = distr(generator);
        }

        //This is dummy gotta fly away
        std::vector<std::pair<T,T>> small_curves;
        std::vector<int> small_length;
        std::vector<int> small_offset;
        for(size_t i=0; i<dataset_lengths.size(); i++) {
          if(dataset_lengths[i] <= 10) {
            small_offset.push_back(small_curves.size());
            small_length.push_back(dataset_lengths[i]);
            for(size_t j=dataset_offsets[i]; j<dataset_offsets[i]+dataset_lengths[i]; j++) {
              small_curves.push_back(dataset_curves[j]);
            }
          }
        }

        //for every dataset curve
        for(size_t i=0; i<small_offset.size(); i++) {
          //for every traversal in l row
          T l = small_length[i]-1;
          for(size_t j=0; j<M; j++) {
            for(auto& tr:relevant_traversals[l*M+j]) {
              //replace pointers with coordinates
              std::vector<std::pair<T,T>> rep_curve;
              for(auto& u:tr) {
                T pos = u.first;
                rep_curve.push_back(std::make_pair(small_curves[small_offset[i]+pos].first,u.second));
                //std::cout << u.first << " ";
                //u.first = small_curves[small_offset[i]+pos].first;
              }
              //CreateVector(rep_curve);
            }
          }
        }

        // for(size_t i=0; i<M; i++) {
        //   for(size_t j=0; j<M; j++) {
        //     for(auto& rt:relevant_traversals[i*M+j]) {
        //       std::vector<std::vector<T>> x;
        //       for(auto& ui:rt) {
        //         std::vector<T> xi; 
        //         for(int k=0; k<size; k+=2) {
        //           xi.push_back(ui.first*(G[k]+G[k+1]));
        //         }
        //         x.push_back(xi);
        //       }
        //     }
        //   }
        // }
      };

      std::vector<T> CreateVector(std::vector<std::pair<T,T>>& traversal) {
        std::vector<std::vector<T>> x;
        for(auto& ui:traversal) {
          std::vector<T> xi; 
          for(int i=0; i<K; i++) {
            for(int j=0; j<d; j++) {
              xi.push_back(ui.first*G[i*d+j]);
            }
          }
          x.push_back(xi);
        }
        //concat
      };

      std::vector<std::vector<std::pair<T,T>>> RelevantTraversals(int i, int j) {
        std::set<std::pair<T,T>> diag_cells;
        std::set<std::pair<T,T>> rel_cells;
        diag_cells = DrawLineSegment(i, j, i+1, j+1);
        rel_cells = FindNeighbors(diag_cells,i,j);
        std::vector<std::pair<T,T>> path;
        std::vector<std::vector<std::pair<T,T>>> paths;
        FindTraversals(path,paths,rel_cells,0,0,i+1,j+1);
        return paths;
      }

      /* \brief
          Find cells that are crossed by main diagonal line segment
      */
      std::set<std::pair<T,T>> DrawLineSegment(int x, int y, int m, int n) { 
        std::set<std::pair<T,T>> diag_cells;
        for (int xi = 0; xi <= x; xi++) {
          int yi = n/m * xi;
          diag_cells.insert(std::make_pair(xi,yi));
        }
        for (int yi = 0; yi <= y; yi++) {
          int xi = m/n * yi;
          diag_cells.insert(std::make_pair(xi,yi));
        }
        return diag_cells;
      }

      /* \brief
          Find cells that are crossed by main diagonal line segment
      */
      std::set<std::pair<T,T>> FindNeighbors(std::set<std::pair<T,T>>& diag_cells, int m, int n) {
        std::set<std::pair<T,T>> rel_cells (diag_cells);
        for(auto& p:diag_cells) {
          if(p.first != 0) {
            rel_cells.insert(std::make_pair(p.first-1,p.second));
          }
          // if(p.first != m) {
          //   rel_cells.insert(std::make_pair(p.first+1,p.second));
          // }
        }
        return rel_cells;
      }

      /* \brief
          A path is relevant when it consists of cells that are either on
          main diagonal line segment or have a distance of 1.
      */
      const bool isRelevant(std::set<std::pair<T,T>>& s, int i, int j, int m, int n) {
        const bool is_in = s.find(std::make_pair(i,j)) != s.end();
        return (i >= 0 && i < m && j >= 0 && j < n && is_in);
      }

      void FindTraversals(std::vector<std::pair<T,T>>& path, std::vector<std::vector<std::pair<T,T>>>& paths,
                     std::set<std::pair<T,T>>& s, int i, int j, int m, int n) {
        //destination point reached
        if ((i == m-1) && (j == n-1)) {
          path.push_back(std::make_pair(i,j));
          paths.push_back(path);
          path.pop_back();
          return;
        }
        //add curr cell to path
        path.push_back(std::make_pair(i,j));
        //move right
        if (isRelevant(s, i+1, j, m, n)) {
          FindTraversals(path, paths, s, i+1, j, m, n);
        }
        //move left
        if (isRelevant(s, i, j+1, m, n)) {
          FindTraversals(path, paths, s, i, j+1, m, n);
        }
        if (isRelevant(s, i+1, j+1, m, n)) {
          FindTraversals(path, paths, s, i+1, j+1, m, n);
        }
        //delete last pair
        path.pop_back();
      }
  };
}

#endif
