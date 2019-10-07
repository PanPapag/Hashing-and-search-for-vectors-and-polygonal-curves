#include <iostream>
#include <string>
#include <iterator>
#include <vector>

#include "../headers/xvector.h"

template <class T>
xVector<T>::xVector(std::string item, std::vector<T> vect) : item(item) {
  this->feature_vector.swap(vect);
}

template <class T>
xVector<T>::~xVector() {}

template <>
void xVector<double>::PrintVector() {
  for (const auto &xi : feature_vector) {
    std::cout << xi << std::endl;
  }
}

template <>
void xVector<std::vector<double>>::PrintVector() {
  for (const auto &xi : feature_vector) {
    for (const auto &xij : xi) {
      std::cout << xij << ' ';
    }
    std::cout << std::endl;
  }
}

template class xVector<double>;
template class xVector<std::vector<double>>;
