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
        const std::vector<int>& lenghts, const std::vector<int>& offsets,
        const uint32_t N, const uint32_t D, double delta) :
          N(N), D(D), delta(delta), distribution(0,delta),
          generator(std::chrono::system_clock::now().time_since_epoch().count()),
          input_curves(curves), input_curves_lengths(lenghts),
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
          // Fill with pading coordinates to have eqaal length vectors
          for (size_t j = idx; j < D; ++j) {
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
      size_t K;
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
        std::vector<std::vector<std::vector<std::pair<int,int>>>> relevant_traversals (size);
        int count = 0;
        for(size_t i=0; i<M; i++) {
          for(size_t j=0; j<M; j++) {
            //if(abs(i-j < 4)) { //change this
              relevant_traversals[i*M+j] = RelevantTraversals(i,j);
              count+=relevant_traversals[i*M+j].size();
            //}
          }
        }
        std::cout << count << std::endl;
        size = K*d*K*d;
        std::vector<double> G (size);
        for(size_t i=0; i<size; i++) {
          G[i] = distr(generator);
        }
        for(size_t i=0; i<M; i++) {
          for(size_t j=0; j<M; j++) {
            for(auto& rt:relevant_traversals[i*M+j]) {
              std::vector<std::vector<T>> x;
              for(auto& ui:rt) {
                std::vector<T> xi;
                for(int k=0; k<size; k+=2) {
                  xi.push_back(ui.first*(G[k]+G[k+1]));
                }
                x.push_back(xi);
              }
            }
          }
        }
      };

      std::vector<std::vector<std::pair<int,int>>> RelevantTraversals(int i, int j) {
        std::set<std::pair<int,int>> diag_cells;
        std::set<std::pair<int,int>> rel_cells;
        diag_cells = DrawLineSegment(i, j, i+1, j+1);
        rel_cells = FindNeighbors(diag_cells,i,j);
        std::vector<std::pair<int,int>> path;
        std::vector<std::vector<std::pair<int,int>>> paths;
        FindTraversals(path,paths,rel_cells,0,0,i+1,j+1);
        return paths;
      }

      /* \brief
          Find cells that are crossed by main diagonal line segment
      */
      std::set<std::pair<int,int>> DrawLineSegment(int x, int y, int m, int n) {
        std::set<std::pair<int,int>> diag_cells;
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
      std::set<std::pair<int,int>> FindNeighbors(std::set<std::pair<int,int>>& diag_cells, int m, int n) {
        std::set<std::pair<int,int>> rel_cells (diag_cells);
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
      const bool isRelevant(std::set<std::pair<int,int>>& s, int i, int j, int m, int n) {
        const bool is_in = s.find(std::make_pair(i,j)) != s.end();
        return (i >= 0 && i < m && j >= 0 && j < n && is_in);
      }

      void FindTraversals(std::vector<std::pair<int,int>>& path, std::vector<std::vector<std::pair<int,int>>>& paths,
                     std::set<std::pair<int,int>>& s, int i, int j, int m, int n) {
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
