#ifndef BRUTE_FORCE
#define BRUTE_FORCE

namespace search {

  template <typename T, typename U>
  class BruteForce {
    private:
      const int N;
      const int D;
      const int R;
      const std::vector<T>  &feature_vector;
      const std::vector<U>  &feature_vector_ids;
    public:
      BruteForce(const std::vector<T> &points, const std::vector<T> &ids,
        const int N, const int D, const int R)
        : N(N), D(D), R(R), feature_vector(points), feature_vector_ids(ids) {};
      //std::pair<T,U> NearestNeighbor(iterator q_start, iterator q_end);
      //std::vector<std::pair<T,U>> RadiusNearestNeighbor(iterator q_start, iterator q_end);
  };
}

#endif
