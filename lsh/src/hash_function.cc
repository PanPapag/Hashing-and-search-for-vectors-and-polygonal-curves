#include <iostream>
#include <string>
#include <iterator>
#include <cmath>
#include <vector>

#include "../headers/utils.h"
#include "../headers/xvector.h"
#include "../headers/hash_function.h"

template <class T>
HashFunction<T>::HashFunction(double r, double w, xVector<T>& x) {
  RandomVectorInit(x.getDimensions(),w);
  std::vector<T> a = ProjectX(w,x);
}

template <>
void HashFunction<double>::RandomVectorInit(int n, double w) {
  for(int i=0;i<n;i++) {
    s.push_back(utils::fRand(0,w));
  }
}

template <>
void HashFunction<std::vector<double>>::RandomVectorInit(int n, double w) {
  //TODO(Maria): implementation for curves
}

template <>
void HashFunction<double>::PrintVector() {
  for (const auto &si : s) {
    std::cout << si << std::endl;
  }
}

template <>
void HashFunction<std::vector<double>>::PrintVector() {
  //TODO(Maria): implementation for curves
}

template <>
std::vector<double>& HashFunction<double>::ProjectX(double w, xVector<double>& point) {
  std::vector<double> x = point.getFeatureVector();
  std::vector<double> a;
  for (int i=0; i<x.size(); i++) {
    double j =  (x[i] - s[i])/w; //lower bound
    a.push_back(j);
  }
  return a;
}

template <>
std::vector<std::vector<double>>& HashFunction<std::vector<double>>::ProjectX(double w, xVector<std::vector<double>>& point) {
  std::vector<std::vector<double>> x = point.getFeatureVector();
  std::vector<std::vector<double>> a;
  //TODO(Maria): implementation for curves
  return a;
}


template <class T>
HashFunction<T>::~HashFunction() {
}

template class HashFunction<double>;
template class HashFunction<std::vector<double>>;
