#include <iostream>
#include <string>
#include <iterator>
#include <vector>

#include "../headers/dpoint.h"

template <class T>
dpoint<T>::dpoint(std::string item, std::vector<T>& vect) {
  this->item = item;
  this->feature_vector = vect;   //TODO (Maria) change to copy constructor
}

template <class T>
dpoint<T>::~dpoint(){}

template <>
void dpoint<double>::print_vector() {
  for (const auto &xi : feature_vector) {
    std::cout << xi << std::endl;
  }
}

template <>
void dpoint<std::vector<double>>::print_vector() {
  for (const auto &xi : feature_vector) {
    for (const auto &xij : xi) {
      std::cout << xij << ' ';
    }
    std::cout << std::endl;
  }
}

template class dpoint<double>;
template class dpoint<std::vector<double>>;
