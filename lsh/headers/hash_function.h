#ifndef HASH_FUNCTION
#define HASH_FUNCTION

#include <iostream>
#include <string>
#include <iterator>
#include <cmath>
#include <vector>

#include "../headers/utils.h"

template <class T>
class HashFunction {
  private:
    std::vector<T> s;
  public:
    HashFunction(double, double, int);
    void RandomVectorInit(int, double);
    uint64_t HashPoint(uint64_t, uint64_t, std::vector<T>&);
    void PrintVector();
    std::vector<T>& ProjectX(double, std::vector<T>&); 
    ~HashFunction();
};

template <class T>
HashFunction<T>::HashFunction(double r, double w, int d) {
  RandomVectorInit(d,w);
}

template <class T>
void HashFunction<T>::RandomVectorInit(int n, double w) {
  for(size_t i=0; i<n; i++) {
    s.push_back(utils::fRand(0,w));
  }
}

template <class T>
void HashFunction<T>::PrintVector() {
  for (const auto &si : s) {
    std::cout << si << std::endl;
  }
}

template <class T>
std::vector<T>& HashFunction<T>::ProjectX(double w, std::vector<T>& x) {
  std::vector<T> a; 
  for (size_t i=0; i<x.size(); i++) {
    T j =  (x[i] - s[i])/w;
    a.push_back(floor(j));
  }
  return a;
}

template <class T>
uint64_t HashFunction<T>::HashPoint(uint64_t m, uint64_t M, std::vector<T>& a) { 
  double hi = 0;
  for (size_t i=0; i<a.size(); i++) {
    double di = i * 1.0;
    double mi = m * 1.0;
    size_t j = a.size() - i - 1; 
    hi +=  a[j] * pow(mi,di) % M;
  }
  return hi;
}

template <class T>
HashFunction<T>::~HashFunction() {}

#endif
