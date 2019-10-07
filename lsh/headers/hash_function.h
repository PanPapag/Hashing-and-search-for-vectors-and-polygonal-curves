#ifndef HASH_FUNCTION
#define HASH_FUNCTION

#include <string>
#include <vector>

#include "../headers/xvector.h"

template <class T>
class HashFunction {
  private:
    double w;
    int m;
    std::vector<T> si;
  public:
    HashFunction(double,xVector<T>&);
    ~HashFunction();
};

#endif
