#ifndef VECTORIZATION
#define VECTORIZATION

#include <iostream>
#include <vector>

namespace vectorization {

  template <typename T, typename U>
  void Grid(void) {

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
