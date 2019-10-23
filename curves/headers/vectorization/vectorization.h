#ifndef VECTORIZATION
#define VECTORIZATION

#include <cmath>
#include <iostream>
#include <tuple>
#include <vector>

namespace vectorization {

  template <typename T, typename U>
  class Grid {
    private:
      uint32_t D;
      uint32_t N;
      const std::vector<std::pair<T,T>>& input_curves;
      const std::vector<U>& input_curves_ids;
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
        const std::vector<U>& ids, const std::vector<int>& lenghts,
        const std::vector<int>& offsets, double delta)
        : delta(delta), distribution(0,delta),
          generator(std::chrono::system_clock::now().time_since_epoch().count()),
          input_curves(curves), input_curves_ids(ids),
          input_curves_lengths(lenghts), input_curves_offsets(offsets) {
          // t is selected uniformly between 0 - delta
          t.first = distribution(generator);
          t.second = distribution(generator);
          // D is the dimension of the equivalent vector which represents a curve
          D = 2 * *max_element(std::begin(input_curves_lengths),std::end(input_curves_lengths));
          // N stands for the number of input curves
          N = input_curves_ids.size();
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
          Vectorize each curve and store the corresponding vector to an 1D array
          Each vector is of dimension D (max curve length) and we have in total_time
          N curves, and so N vectors.
        */
        std::vector<double> result(D * N);
        /* Iterave over each curve to compute its vector */
        for (size_t i = 0; i < input_curves_ids.size(); ++i) {
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
    public:
      /* \brief
          Just a constructor
      */
      Projection(std::vector<std::pair<T,T>>& dataset_curves, std::vector<int>& dataset_offsets,
                std::vector<int>& dataset_lengths, std::vector<U>& dataset_ids) : eps(0.5), d(2) {
        // Get max length from all curves and
        // store all relevant traversals at an M * M array
        M = *max_element(std::begin(dataset_lengths), std::end(dataset_lengths));
        K = d * (-1) * log(eps) / (eps*eps);

        // Computing relevant traversals
        // only for upper diagonal cells
        // to reduce memory space
        size_t size = (M+1)*(M+1);
        std::vector<std::vector<std::pair<int,int>>> relevant_traversals (size);
        for(size_t i=1; i<M+1; i++) {
          for(size_t j=i; j<M+1; j++) {
            size_t max = (i>j) ? i:j;
            relevant_traversals[i*M+j] = ComputeNeighbors(max, relevant_traversals[i*M+j]);
          }
        }
        size = K*d*K*d;
        std::vector<double> G (size);

      };

      /* \brief
          Find neighbor cells of diagonal
        - these are the relevant traversals of dist 1
      */
      std::vector<std::pair<int,int>>& ComputeNeighbors(int l, std::vector<std::pair<int,int>>& vect) {
        if (l==1) {
          vect.push_back(std::make_pair(1, 1));
          return vect;
        }
        for(int i=2; i<=l; i++) {
          vect.push_back(std::make_pair(i-1, i-1));
          vect.push_back(std::make_pair(i-1, i));
          vect.push_back(std::make_pair(i, i-1));
          vect.push_back(std::make_pair(i, i));
        }
        return vect;
      }
  };
}

#endif
