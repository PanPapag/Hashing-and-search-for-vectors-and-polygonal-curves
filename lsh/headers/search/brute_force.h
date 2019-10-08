#ifndef BRUTE_FORCE
#define BRUTE_FORCE

namespace search {

  template <typename T>
  class BruteForce {
    private:
      const int N;
      const int D;
      const std::vector<T>  &input_points;
      const std::vector<T>  &input_ids;
  };
}

#endif
