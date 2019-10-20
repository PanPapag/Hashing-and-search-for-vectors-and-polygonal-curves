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
      uint32_t M;
      // table
    public:
      /* \brief 
          Just a constructor 
      */
      Projection(std::vector<std::pair<T,T>>& dataset_curves, std::vector<int>& dataset_offsets, 
                std::vector<int>& dataset_lengths, std::vector<U>& dataset_ids) {
        // Get max length from all curves and 
        // store all relevant traversals at an M * M array 
        //M = *max_element(std::begin(dataset_lengths), std::end(dataset_lengths));
        M = 4; //just for testing
        //(i,j) = i*M+j
        size_t size = (M+1)*(M+1);
        std::vector<std::vector<std::pair<int,int>>> relevant_traversals (size);
        // Storing all neighbors of diagonial cells
        for(size_t i=1; i<M+1; i++) {
          for(size_t j=1; j<M+1; j++) {
            size_t max = (i>j) ? i:j;
            relevant_traversals[i*M+j] = ComputeNeighbors(max);
          }
        }
        for(size_t i=1; i<M+1; i++)  {
          for(size_t j=1; j<M+1; j++) {
            std::cout << i << "," << j << std::endl;
            std::cout << "-------------" << std::endl;
            for (auto& i:relevant_traversals[i*M+j]) {
              std::cout << i.first << " " << i.second << std::endl;
            }
            std::cout << std::endl;
          }
        }
      };
      /* \brief 
          Find neighbor cells of diagonal 
        - these are the relevant traversals of dist 1
      */
      std::vector<std::pair<int,int>> ComputeNeighbors(int l) {
        std::vector<std::pair<int,int>> vect;
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
