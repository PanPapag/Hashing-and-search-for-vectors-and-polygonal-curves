#ifndef HASH_FUNCTION
#define HASH_FUNCTION

#include <string>
#include <vector>

#include "../headers/xvector.h"

template <class T>
class HashFunction {
  private:
    int m;
    std::vector<T> s;
  public:
    HashFunction(double, double);
    void RandomVectorInit(int, double);
    void PrintVector();
    std::vector<T>& ProjectX(double, xVector<T>&); 
    ~HashFunction();
};

#endif
